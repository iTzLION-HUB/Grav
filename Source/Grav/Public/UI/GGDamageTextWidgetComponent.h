// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GGDamageTextWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class GRAV_API UGGDamageTextWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(float Damage);
	
};
