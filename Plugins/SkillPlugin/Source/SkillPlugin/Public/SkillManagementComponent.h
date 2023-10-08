// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillManagementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_UpdateSkillLevel, const TArray<class ASkillBase*>&, skill);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(Fuc_Dele_UpdateDefenseAreaCooldown_ThreeParams, float, skillColldown, const TArray<class ASkillBase*>&, skill, ASkillBase*, defenseArea);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(Fuc_Dele_UpdateStormCooldown_ThreeParams, float, skillColldown, const TArray<class ASkillBase*>&, skill, ASkillBase*, storm);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(Fuc_Dele_UpdateLightningCooldown_ThreeParams, float, skillColldown, const TArray<class ASkillBase*>&, skill, ASkillBase*, lightning);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(Fuc_Dele_UpdateWaterBallCooldown_ThreeParams, float, skillColldown, const TArray<class ASkillBase*>&, skill, ASkillBase*, waterBall);


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SKILLPLUGIN_API USkillManagementComponent : public UActorComponent
{
	GENERATED_BODY()

	public:
	// Sets default values for this component's properties
	USkillManagementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	TArray<class ASkillBase*> SkillDatas;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	TArray<class ASkillBase*> PlayerSkills;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data")
	TArray<class ASkillBase*> RandomSkills;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data")
	class AActiveSkillLightning* Lightning;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data")
	class AActiveSkillStorm* Storm;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data")
	class AActiveSkillWaterBall* WaterBall;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data")
	class APassiveSkillDefenseArea* DefenseArea;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data", Replicated)
	float LightningLevel;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data", Replicated)
	float StormLevel;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data", Replicated)
	float DefenseAreaLevel;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data", Replicated)
	float WaterBallLevel;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data", Replicated)
	float LightningCooldown;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data", Replicated)
	float StormCooldown;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data", Replicated)
	float DefenseAreaCooldown;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data", Replicated)
	float WaterBallCooldown;

	UFUNCTION()
	void SkillLevelUp(class ASkillBase* targetSkill);

	UFUNCTION(BlueprintCallable)
	float GetSkillLevel(class ASkillBase* targetSkill);

	UFUNCTION(BlueprintCallable)
	bool GetSkillColldown(class ASkillBase* targetSkill);

	UFUNCTION(BlueprintCallable)
	void UsingSkill(class ASkillBase* targetSkill);

	UFUNCTION(BlueprintCallable)
	void LightningCooldownCounter();

	UFUNCTION(BlueprintCallable)
	void StormCooldownCounter();

	UFUNCTION(BlueprintCallable)
	void WaterBallCooldownCounter();

	UFUNCTION(BlueprintCallable)
	void DefenseAreaCooldownCounter();

	FTimerHandle LightningCooldownHandle;

	FTimerHandle StormCooldownHandle;

	FTimerHandle WaterBallCooldownHandle;

	FTimerHandle DefenseAreaCooldownHandle;

	UFUNCTION()
	TArray<class ASkillBase*> GetRandomSkills();

	UFUNCTION()
	void OnRep_SkillLevel();

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FDele_UpdateSkillLevel Fuc_Dele_UpdateSkillLevel;

	UFUNCTION()
	void OnRep_DefenseAreaCooldown();

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	Fuc_Dele_UpdateDefenseAreaCooldown_ThreeParams Fuc_Dele_UpdateDefenseAreaCooldown;

	UFUNCTION()
	void OnRep_LightningCooldown();

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	Fuc_Dele_UpdateLightningCooldown_ThreeParams Fuc_Dele_UpdateLightningCooldown;

	UFUNCTION()
	void OnRep_WaterBallCooldown();

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	Fuc_Dele_UpdateWaterBallCooldown_ThreeParams Fuc_Dele_UpdateWaterBallCooldown;

	UFUNCTION()
	void OnRep_StormCooldown();

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	Fuc_Dele_UpdateStormCooldown_ThreeParams Fuc_Dele_UpdateStormCooldown;

};