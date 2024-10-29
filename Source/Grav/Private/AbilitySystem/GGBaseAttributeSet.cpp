// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GGBaseAttributeSet.h"

void UGGBaseAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttributeOnChange(Attribute, NewValue);
}

void UGGBaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	ClampAttributeOnChange(Attribute, NewValue);
}

void UGGBaseAttributeSet::ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
}
