// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GGPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class GRAV_API AGGPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AGGPlayerState();

	//Inherited from IAbilitySytstemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//healthSet
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = true))
	class UGGHealthSet* HealthSet;

	UFUNCTION(BlueprintCallable, Category = "GAS|GGPlayerState|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|GGPlayerState|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|GGPlayerState|Attributes")
	float GetShield() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|GGPlayerState|Attributes")
	float GetMaxShield() const;


protected:
	UPROPERTY()
	class UAbilitySystemComponent* AbilitySystemComponent;
	
};
