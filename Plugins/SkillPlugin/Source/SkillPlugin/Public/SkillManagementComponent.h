// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillManagementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_UpdateSkillLevel, const TArray<class ASkillBase*>&, skill);


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

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data")
	float LightningLevel = 0;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data")
	float StormLevel = 0;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data")
	float DefenseAreaLevel = 0;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data")
	float WaterBallLevel = 0;

	UFUNCTION()
	void SkillLevelUp(class ASkillBase* targetSkill);

	UFUNCTION(BlueprintCallable)
	float GetSkillLevel(class ASkillBase* targetSkill);

	UFUNCTION()
	TArray<class ASkillBase*> GetRandomSkills();

	UFUNCTION()
	void OnRep_SkillLevel();

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FDele_UpdateSkillLevel Fuc_Dele_UpdateSkillLevel;

};