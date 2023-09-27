// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StatEnhancementObjectBase.generated.h"

UCLASS()
class ITEMPLUGIN_API AStatEnhancementObjectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStatEnhancementObjectBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	class UTexture2D* SkillThumbnail;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	FString ObjName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	FString ObjDescription;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	float price;

public:
	UFUNCTION()
	virtual void EnhancementPlayerStat();
};
