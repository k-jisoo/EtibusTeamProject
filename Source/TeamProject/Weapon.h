// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class TEAMPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void SetSphereCollisionState(bool state);

	void SetOwnChar(class ABaseCharacter* Char);


public:
	UFUNCTION(Server, Reliable)
	void ReqOnBoxComponentBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(NetMulticast, Reliable)
	void RecOnBoxComponentBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	class UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* Cube;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character")
	class ABaseCharacter* OwnChar;

};
