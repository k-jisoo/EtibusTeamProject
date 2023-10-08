// Fill out your copyright notice in the Description page of Project Settings.


#include "DropItemActor.h"
#include "ItemBase.h"


// Sets default values
ADropItemActor::ADropItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADropItemActor::BeginPlay()
{
	Super::BeginPlay();

	int32 RandomNumber = FMath::RandRange(1, 10);

	UE_LOG(LogTemp, Warning, TEXT("%d"), RandomNumber);

	if (RandomNumber > 0 && RandomNumber <= 6)
	{
		AItemBase* SpawnedActor = GetWorld()->SpawnActor<AItemBase>(GoldItem, this->GetActorLocation(), this->GetActorRotation());
		if (!SpawnedActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("ADropItemActor::BeginPlay Not SpawnedActor "));
		}
	}
	else if (RandomNumber == 7)
	{
		AItemBase* SpawnedActor = GetWorld()->SpawnActor<AItemBase>(EnhancementMaxHp, this->GetActorLocation(), this->GetActorRotation());
		if (!SpawnedActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("ADropItemActor::BeginPlay Not SpawnedActor "));
		}
	}
	else if (RandomNumber == 8)
	{
		AItemBase* SpawnedActor = GetWorld()->SpawnActor<AItemBase>(EnhancementMaxMp, this->GetActorLocation(), this->GetActorRotation());
		if (!SpawnedActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("ADropItemActor::BeginPlay Not SpawnedActor "));
		}
	}
	else if (RandomNumber == 9)
	{
		AItemBase* SpawnedActor = GetWorld()->SpawnActor<AItemBase>(EnhancementPower, this->GetActorLocation(), this->GetActorRotation());
		if (!SpawnedActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("ADropItemActor::BeginPlay Not SpawnedActor "));
		}
	}
	else if (RandomNumber == 10)
	{
		AItemBase* SpawnedActor = GetWorld()->SpawnActor<AItemBase>(EnhancementSpeed, this->GetActorLocation(), this->GetActorRotation());
		if (!SpawnedActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("ADropItemActor::BeginPlay Not SpawnedActor "));
		}
	}

	Destroy();
}

// Called every frame
void ADropItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

