// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GGBaseAttributeSet.generated.h"
// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class GRAV_API UGGBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
protected:
	virtual void  PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void  PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const;
};
