// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Grav/Grav.h"
#include "GGGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class GRAV_API UGGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Ability")
	EAbilityInputID AbilityInputID{EAbilityInputID::None};

public:
	EAbilityInputID GetAbilityInputID() {return AbilityInputID;}
};
