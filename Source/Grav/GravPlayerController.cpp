// Copyright Epic Games, Inc. All Rights Reserved.


#include "GravPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "UI/GGDamageTextWidgetComponent.h"
#include "Engine/LocalPlayer.h"
#include "GravCharacter.h"
#include "Gameplay/GGPlayerState.h"
#include "UI/GGHUDWidget.h"

void AGravPlayerController::CreateHUD()
{
	// Need a valid PlayerState to get attributes from
	AGGPlayerState* PS = GetPlayerState<AGGPlayerState>();
	if (!PS)
	{
		return;
	}

	//Create Once Only
	if (UIHUDWidget)
	{
		return;
	}

	if (!UIHUDWidgetClass)
	{
		UE_LOG(LogTemp, Error,
		       TEXT("%s() Missing UIHUDWidgetClass. Please fill in on the Blueprint of the PlayerController."),
		       *FString(__FUNCTION__));
		return;
	}

	UIHUDWidget = CreateWidget<UGGHUDWidget>(this, UIHUDWidgetClass);
	UIHUDWidget->AddToViewport();
	
	UIHUDWidget->SetCurrentHealth(PS->GetHealth());
	UIHUDWidget->SetMaxHealth(PS->GetMaxHealth());
	UIHUDWidget->SetHealthPercentage(PS->GetHealth()/ FMath::Max<float>(PS->GetMaxHealth(), 1.f));
	UIHUDWidget->SetCurrentShield(PS->GetShield());
	UIHUDWidget->SetMaxShield(PS->GetMaxShield());
	UIHUDWidget->SetShieldPercentage(PS->GetShield()/ FMath::Max<float>(PS->GetMaxShield(), 1.f));
}

//DamageNum UI
void AGravPlayerController::ShowDamageNumber_Implementation(float DamageAmount, AGravCharacter* TargetCharacter)
{
	if (TargetCharacter && DamageNumberClass)
	{
		UGGDamageTextWidgetComponent* DamageText = NewObject<UGGDamageTextWidgetComponent>(TargetCharacter, DamageNumberClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->SetDamageText(DamageAmount);
	}
}

bool AGravPlayerController::ShowDamageNumber_Validate(float DamageAmount, AGravCharacter* TargetCharacter)
{
	return true;
}


void AGravPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
	CreateHUD();
}


