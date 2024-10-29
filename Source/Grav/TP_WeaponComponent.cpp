// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "GravCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"  // Include this for debugging
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/SceneComponent.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	
}


void UTP_WeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	//LineTrace for Gravity Gun
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		const FVector StartTrace = Character->GetFirstPersonCameraComponent()->GetComponentLocation();
		const FVector EndTrace = StartTrace + (Character->GetFirstPersonCameraComponent()->GetForwardVector() *
												Character->GetTraceRange());

		FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(GravityGunTrace), false, Character);
		FHitResult GV_Hit(ForceInit);

		if (World->LineTraceSingleByChannel(GV_Hit, StartTrace, EndTrace, ECC_Visibility, QueryParams))
		{
			if (UPrimitiveComponent* Prim = GV_Hit.GetComponent())
			{
				if (Prim->IsSimulatingPhysics())
				{
					SetGrabbedObject(Prim);
				}
			}
		}
	}
}

void UTP_WeaponComponent::EndFire()
{
	if (GrabbedOject)
	{
		const FVector ShootVeloctiy = Character->GetFirstPersonCameraComponent()->GetForwardVector()* Character->GetShootForce();

		GrabbedOject->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		GrabbedOject->SetSimulatePhysics(true);
		GrabbedOject->AddImpulse(ShootVeloctiy, NAME_None, true);

		SetGrabbedObject(nullptr);

		// Try and play the sound if specified
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
		}
	
		// Try and play a firing animation if specified
		if (FireAnimation != nullptr)
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
			if (AnimInstance != nullptr)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}
	}
	
}

bool UTP_WeaponComponent::AttachWeapon(AGravCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UTP_WeaponComponent>())
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &UTP_WeaponComponent::Fire);
			EnhancedInputComponent->BindAction(ReleaseAction, ETriggerEvent::Started, this, &UTP_WeaponComponent::EndFire);
		}
	}

	return true;
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}

void UTP_WeaponComponent::SetGrabbedObject(UPrimitiveComponent* ObjectToGrab)
{
	GrabbedOject = ObjectToGrab;
	if(GrabbedOject)
	{
		GrabbedOject->SetSimulatePhysics(false);
		GrabbedOject->AttachToComponent(Character->GetGrabbedLocation(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}
