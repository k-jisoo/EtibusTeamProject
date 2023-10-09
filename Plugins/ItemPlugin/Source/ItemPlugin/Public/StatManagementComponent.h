// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatManagementComponent.generated.h"

enum class EItemType : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDele_UpdateHp_TwoParams, float, CurHp, float, MaxHp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDele_UpdateMp_TwoParams, float, CurMp, float, MaxMp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_UpdateSpeed_OneParam, float, Speed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_UpdatePower_OneParam, float, Power);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ITEMPLUGIN_API UStatManagementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatManagementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION()
	void OnRep_CurHp();

	UFUNCTION()
	void OnRep_MaxHp();

	UFUNCTION()
	void OnRep_CurMp();

	UFUNCTION()
	void OnRep_MaxMp();

	UFUNCTION()
	void OnRep_Speed();

	UFUNCTION()
	void OnRep_Power();

public:
	UPROPERTY(ReplicatedUsing = OnRep_CurHp)
	float CurHp = 100.0f;

	UPROPERTY(ReplicatedUsing = OnRep_MaxHp)
	float MaxHp = 100.0f;

	UPROPERTY(ReplicatedUsing = OnRep_CurHp)
	float CurMp = 100.0f;

	UPROPERTY(ReplicatedUsing = OnRep_MaxMp)
	float MaxMp = 100.0f;

	UPROPERTY(ReplicatedUsing = OnRep_Speed)
	float Speed = 60.0f;

	UPROPERTY(ReplicatedUsing = OnRep_Power)
	float Power = 100.0f;

public:
	UFUNCTION()
	void ComparisonObjType(class AStatEnhancementObjectBase* target);

	UFUNCTION()
	void IncreaseMaxHp();

	UFUNCTION()
	void RecurberyHp();

	UFUNCTION()
	void IncreaseMaxMp();

	UFUNCTION()
	void IncreaseSpeed();

	UFUNCTION()
	void IncreasePower();

	UFUNCTION()
	void RecurberyMp();

	UFUNCTION()
	void BurfPower();

	UFUNCTION()
	void BurfSpeed();

	UFUNCTION()
	void GetBackSpeedVal();

	UFUNCTION()
	void GetBackPowerVal();

public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FDele_UpdateHp_TwoParams Fuc_Dele_UpdateHp;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FDele_UpdateMp_TwoParams Fuc_Dele_UpdateMp;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FDele_UpdateSpeed_OneParam Fuc_Dele_UpdateSpeed;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FDele_UpdatePower_OneParam Fuc_Dele_UpdatePower;

	FTimerHandle Th_SpeedHandle;

	FTimerHandle Th_PowerHandle;
};
