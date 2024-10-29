// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GGBaseAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "GGHealthSet.generated.h"

// Delegate used to broadcast attribute events.
DECLARE_MULTICAST_DELEGATE_FourParams( FDamageTakenEvent,  AActor* /*EffectInstigator*/, AActor* /*EffectCauser*/,
											  const FGameplayTagContainer& /*GameplayTags*/, float /*EffectMagnitude*/);

/*
 * 
 */
UCLASS()
class GRAV_API UGGHealthSet : public UGGBaseAttributeSet
{
	GENERATED_BODY()

public:
	UGGHealthSet();

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Health, Category = "Attributes", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGGHealthSet, Health);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_MaxHealth ,Category = "Attributes", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGGHealthSet, MaxHealth);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Shield ,Category = "Attributes", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UGGHealthSet, Shield);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_MaxShield ,Category = "Attributes", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxShield;
	ATTRIBUTE_ACCESSORS(UGGHealthSet, MaxShield);
	
	UPROPERTY(BlueprintReadOnly,Category = "Attributes", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ShieldRegen;
	ATTRIBUTE_ACCESSORS(UGGHealthSet, ShieldRegen);
	
	//Damage
	UPROPERTY(BlueprintReadOnly,Category = "Attributes", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData InDamage;
	ATTRIBUTE_ACCESSORS(UGGHealthSet, InDamage);

	// Delegate to broadcast when the Damage Taken
	mutable FDamageTakenEvent OnDamageTaken;

protected:
	virtual  void ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual	void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual  void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	virtual  void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual  void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual  void OnRep_Shield(const FGameplayAttributeData& OldShield);

	UFUNCTION()
	virtual  void OnRep_MaxShield(const FGameplayAttributeData& OldMaxShield);
	
	
};
