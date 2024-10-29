// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/GGDestructibleBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/GGHealthSet.h"

// Sets default values
AGGDestructibleBase::AGGDestructibleBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMeshComponent;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComp"));
	AbilitySystemComponent->SetIsReplicated(true);

	HealthSet = CreateDefaultSubobject<UGGHealthSet>("HealthSet");
}

// Called when the game starts or when spawned
void AGGDestructibleBase::BeginPlay()
{
	Super::BeginPlay();

	if (!AbilitySystemComponent)
		return;
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthSet->GetHealthAttribute()).AddUObject(this, &AGGDestructibleBase::OnHealthAttributeChange);		
	
	
}

// Called every frame
void AGGDestructibleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UAbilitySystemComponent* AGGDestructibleBase::GetAbilitySystemComponent() const
{
	return  AbilitySystemComponent;
}

void AGGDestructibleBase::OnHealthAttributeChange(const FOnAttributeChangeData& Data)
{
	OnHealthChanged(Data.OldValue, Data.NewValue);
}

