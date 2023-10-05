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

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", Replicated)
	class UBoxComponent* SkillArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", Replicated)
	class UParticleSystemComponent* SkillBody;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data", Replicated)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data", Replicated)
	int32 Price;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data", Replicated)
	class UTexture2D* SkillThumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data", Replicated)
	FVector SkillSizeVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data", Replicated)
	FVector CollisionSizeVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data", Replicated)
	FString SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data", Replicated)
	FString SkillDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data", Replicated)
	class AActor* OwnChar;


public:
	UFUNCTION()
	virtual void ProcessBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
