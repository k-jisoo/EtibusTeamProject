// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillBase.generated.h"

UCLASS()
class SKILLPLUGIN_API ASkillBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkillBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UBoxComponent* SkillArea;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "component")
	class UParticleSystemComponent* SkillBody;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	int32 Price;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	class UTexture2D* SkillThumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	double PartX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	double PartY;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	double PartZ;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	double SkillSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FString SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FString SkillDescription;

public:
	UFUNCTION()
	virtual void ProcessBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
