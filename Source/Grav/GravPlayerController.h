// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GravCharacter.h"
#include "GameFramework/PlayerController.h"
#include "GravPlayerController.generated.h"

class UInputMappingContext;

/**
 *
 */
UCLASS()
class GRAV_API AGravPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	//UI

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UGGDamageTextWidgetComponent> DamageNumberClass;

	//HUD
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	TSubclassOf<class UGGHUDWidget> UIHUDWidgetClass;

	//DamageUI
	UFUNCTION(Client, Reliable, WithValidation)
	void ShowDamageNumber(float DamageAmount, AGravCharacter* TargetCharacter);
	void ShowDamageNumber_Implementation(float DamageAmount, AGravCharacter* TargetCharacter);
	bool ShowDamageNumber_Validate(float DamageAmount, AGravCharacter* TargetCharacter);

	void CreateHUD();
	
protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	class UGGHUDWidget* UIHUDWidget;


protected:

	virtual void BeginPlay() override;

	// End Actor interface
};
