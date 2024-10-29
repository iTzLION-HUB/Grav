// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

//Abilities Input
UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	PrimaryAbility,
	SecordaryAbility
};