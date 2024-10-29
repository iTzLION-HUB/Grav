// Copyright Epic Games, Inc. All Rights Reserved.

#include "GravGameMode.h"
#include "GravCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGravGameMode::AGravGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
