// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AbilitySystemInterface.h"
#include "InputActionValue.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "GravCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AGravCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	//Scene Component For GravityGun
	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* GrabbedObjectLocation;
	
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Jump to Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
public:
	AGravCharacter();

protected:
	virtual void BeginPlay();

public:

	//Gravity GunParameters
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GravityGun")
	float TraceRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GravityGun")
	float ShootStrength;
	
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PrimaryAbilityAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SecondaryAbilityAction;

	
protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface
	void OnPrimaryAbility(const FInputActionValue& Value);
	void OnSecondaryAbility(const FInputActionValue& Value);

	virtual void SendAbilityLocalInput(const FInputActionValue& Value, int32 InputID);
	
public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

	//Returns the GravityGun Parameters
	USceneComponent* GetGrabbedLocation() const { return GrabbedObjectLocation; }
	
	float GetShootForce() const {return ShootStrength;}
	float GetTraceRange() const {return TraceRange;}


	
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	
	//-------AbilitySystemCompoment

	//Inherited from IAbilitySytstemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual  void InitializeAbilities();
	virtual  void InitializeEffects();

	void InitAbilitySystemComponent();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual  void OnDamageTakenChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer, float Damage);

protected:
	UPROPERTY()
	TWeakObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<class UGGGameplayAbility>> DefaultAbilities;
	
	TArray<FGameplayAbilitySpecHandle> GivenAbilities;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<class UGameplayEffect>> DefaultEffects;

	virtual  void PostInitializeComponents() override;
	virtual  void ClearGivenAbilities();

	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void OnDamageTaken(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer, float Damage);

	virtual void OnHealthAttributeChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void OnHealthChanged(float OldValue, float NewValue);

	virtual  void OnShieldAttributeChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void OnShieldChanged(float OldValue, float NewValue);

	
};

