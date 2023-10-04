// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Crystal.generated.h"

UCLASS()
class TEAMPROJECT_API ACrystal : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACrystal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

//뷰포트 관련
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Body;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UBoxComponent* Box;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UCapsuleComponent* Capsule;


//데미지 관련
public:

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;



protected:

	float CurHp;

	float MaxHp;

	bool bLive;

	void UpdateHp(float Amount);

	void Die();

};
