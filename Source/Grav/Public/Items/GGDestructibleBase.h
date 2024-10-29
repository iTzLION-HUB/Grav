// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GGDestructibleBase.generated.h"

UCLASS()
class GRAV_API AGGDestructibleBase : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = Mesh, meta =(AllowPrivateAccess = true))
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = GAS, meta =(AllowPrivateAccess = true))
	class UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = GAS, meta =(AllowPrivateAccess = true))
	class UGGHealthSet* HealthSet;

	
	
public:	
	// Sets default values for this actor's properties
	AGGDestructibleBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Inherited via IAbilitySystemComponent
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void OnHealthAttributeChange(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintImplementableEvent, Category = GAS)
	void OnHealthChanged(float OldValue, float NewValue);

};
