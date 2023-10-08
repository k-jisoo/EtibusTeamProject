// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DropItemActor.generated.h"

UCLASS()
class TEAMPROJECT_API ADropItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADropItemActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AItemBase> GoldItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AItemBase> EnhancementMaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AItemBase> EnhancementMaxMp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AItemBase> EnhancementPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AItemBase> EnhancementSpeed;

};
