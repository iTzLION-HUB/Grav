// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/GGPlayerState.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/GGHealthSet.h"

AGGPlayerState::AGGPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComp");
	AbilitySystemComponent->SetIsReplicated(true);

	HealthSet = CreateDefaultSubobject<UGGHealthSet>("HealthSet");
}

UAbilitySystemComponent* AGGPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

float AGGPlayerState::GetHealth() const
{
	return HealthSet->GetHealth();
}

float AGGPlayerState::GetMaxHealth() const
{
	return HealthSet->GetMaxHealth();
}

float AGGPlayerState::GetShield() const
{
	return HealthSet->GetShield();
}

float AGGPlayerState::GetMaxShield() const
{
	return HealthSet->GetMaxShield();
}

