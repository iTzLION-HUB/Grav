// Copyright Epic Games, Inc. All Rights Reserved.

#include "GravCharacter.h"
#include "Animation/AnimInstance.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Gameplay/GGPlayerState.h"
#include "AbilitySystem/Attributes/GGHealthSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpecHandle.h"
#include "AbilitySystem/GGGameplayAbility.h"
#include "Grav.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AGravCharacter

AGravCharacter::AGravCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	//GravityGun location to hold.
	GrabbedObjectLocation = CreateDefaultSubobject<USceneComponent>(TEXT("GrabbedObjectLocation"));
	GrabbedObjectLocation->SetupAttachment(RootComponent);
	GrabbedObjectLocation->SetRelativeLocation(FVector(300.0f, 0.0f, 50.0f));

	//GravityGun parameters.
	ShootStrength = 5000.f;
	TraceRange = 2000.0f;
	
	//AbilitySystem Component
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
}

void AGravCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	AGGPlayerState* PS = GetPlayerState<AGGPlayerState>();
	if (PS)
	{
		//Heatlh Set
		
		if(UGGHealthSet* HealthSet = PS->HealthSet)
		{
			HealthSet->OnDamageTaken.AddUObject(this, &ThisClass::OnDamageTakenChanged);
                                                         	
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthSet->GetHealthAttribute()).AddUObject(this, &AGravCharacter::OnHealthAttributeChanged);
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthSet->GetShieldAttribute()).AddUObject(this, &AGravCharacter::OnShieldAttributeChanged);
		}
	}
	
}

//////////////////////////////////////////////////////////////////////////// Input

void AGravCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGravCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGravCharacter::Look);

		//Actions
		EnhancedInputComponent->BindAction(PrimaryAbilityAction, ETriggerEvent::Triggered, this, &AGravCharacter::OnPrimaryAbility);
		EnhancedInputComponent->BindAction(SecondaryAbilityAction, ETriggerEvent::Triggered, this, &AGravCharacter::OnSecondaryAbility);
		
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

//Input
void AGravCharacter::OnPrimaryAbility(const FInputActionValue& Value)
{
	SendAbilityLocalInput(Value, static_cast<int32>(EAbilityInputID::PrimaryAbility));
}

void AGravCharacter::OnSecondaryAbility(const FInputActionValue& Value)
{
	SendAbilityLocalInput(Value, static_cast<int32>(EAbilityInputID::SecordaryAbility));
}

void AGravCharacter::SendAbilityLocalInput(const FInputActionValue& Value, int32 InputID)
{
	if(AbilitySystemComponent.Get())
	{
		if(Value.Get<bool>())
		{
			AbilitySystemComponent->AbilityLocalInputPressed(InputID);
		}
		else
		{
			AbilitySystemComponent->AbilityLocalInputReleased(InputID);
		}
	}
}

UAbilitySystemComponent* AGravCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

//Initializing Abilities
void AGravCharacter::InitializeAbilities()
{
	//Give Abilities, Server Only
	if (!HasAuthority() || !AbilitySystemComponent.Get())
		return;
	for (TSubclassOf<UGGGameplayAbility>& Ability : DefaultAbilities)
	{
		FGameplayAbilitySpecHandle SpecHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(
			Ability,
			1,
			static_cast<int32>(Ability.GetDefaultObject()->GetAbilityInputID()),
			this));
	}
}
//Initializing Effects
void AGravCharacter::InitializeEffects()
{
	if(!AbilitySystemComponent.Get())
		return;

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect>& Effect : DefaultEffects)
	{
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1, EffectContext);
		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void AGravCharacter::OnDamageTakenChanged(AActor* DamageInstigator, AActor* DamageCauser,
	const FGameplayTagContainer& GameplayTagContainer, float Damage)
{
	OnDamageTaken(DamageInstigator,DamageCauser, GameplayTagContainer, Damage);
}

void AGravCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if(!AbilitySystemComponent.Get())
		return;

	AbilitySystemComponent->InitAbilityActorInfo(this,this);

	InitializeAbilities();
	InitializeEffects();
}

void AGravCharacter::ClearGivenAbilities()
{
	if(HasAuthority() || AbilitySystemComponent.IsValid())
	{
		for(FGameplayAbilitySpecHandle AbilitySpecHandle : GivenAbilities)
		{
			AbilitySystemComponent->ClearAbility(AbilitySpecHandle);
		};
	}
}

void AGravCharacter::OnHealthAttributeChanged(const FOnAttributeChangeData& Data)
{
	OnHealthChanged(Data.OldValue, Data.NewValue);
}

void AGravCharacter::OnShieldAttributeChanged(const FOnAttributeChangeData& Data)
{
	OnShieldChanged(Data.OldValue, Data.NewValue);
}

void AGravCharacter::InitAbilitySystemComponent()
{
	AGGPlayerState* PS = GetPlayerState<AGGPlayerState>();
	if (PS)
	{
		AbilitySystemComponent = PS->GetAbilitySystemComponent();
		AbilitySystemComponent->InitAbilityActorInfo(PS,this);
	}
}

//When player is possessed, Abilities are given.
void AGravCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	InitAbilitySystemComponent();
	InitializeAbilities();
	InitializeEffects();
}

//When player state is initialized, Abilities are given.
void AGravCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	InitAbilitySystemComponent();
	InitializeAbilities();
	InitializeEffects();
}


void AGravCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AGravCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}