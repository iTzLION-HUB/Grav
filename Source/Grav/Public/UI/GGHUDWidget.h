// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GGHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class GRAV_API UGGHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMaxHealth(float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCurrentHealth(float CurrentHealth);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHealthPercentage(float HealthPercentage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMaxShield(float MaxShield);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCurrentShield(float CurrentShield);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetShieldPercentage(float ShieldPercentage);


};
