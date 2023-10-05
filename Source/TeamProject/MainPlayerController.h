// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMainPlayerController();

	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	void InitCharacter();

	struct FTimerHandle MyHandle;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class USkillManagementComponent* SkillManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UStatManagementComponent* StatManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> SkillShopWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* SkillShopWidget;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data")
	TArray<class ASkillBase*> AllSkillDatas;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data")
	TArray<class ASkillBase*> PlayerSkills;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data")
	class AActiveSkillLightning* Lightning;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data")
	class AActiveSkillStorm* Storm;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data")
	class AActiveSkillWaterBall* WaterBall;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data")
	class APassiveSkillDefenseArea* DefenseArea;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data")
	TArray<class AStatEnhancementObjectBase*> StatEnhancementObjs;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data")
	class AEnhancementMaxHp* EnhancementMaxHp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data")
	class AEnhancementMaxMp* EnhancementMaxMp;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data")
	class AEnhancementSpeed* EnhancementSpeed;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data")
	class AEnhancementPower* EnhancementPower;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Data")
	int32 Gold = 200;

public:
	UFUNCTION()
	void CreateSkillShopWidget();

	UFUNCTION()
	void CloseSkillShopWidget();

	UFUNCTION()
	void BindSkillSData();

	UFUNCTION()
	void BindEnhancedObjData();

	UFUNCTION()
	void BindPlayerInfo();

	UFUNCTION()
	void BindStatManagers();

	UFUNCTION()
	void AddSkillDataToSkillManager(TArray<class ASkillBase*>& skillDatas);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUpdateSkills(const TArray<class ASkillBase*>& skillDatas);

	void OnUpdateSkills_Implementation(const TArray<class ASkillBase*>& skillDatas);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUpdateEnhancementObjs(const TArray<class AStatEnhancementObjectBase*>& objDatas);

	void OnUpdateEnhancementObjs_Implementation(const TArray<class AStatEnhancementObjectBase*>& objDatas);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUpdateMySkillLevel(const TArray<class ASkillBase*>& skillDatas);

	void OnUpdateMySkillLevel_Implementation(const TArray<class ASkillBase*>& skillDatas);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUpdateMyMaxHp(float CurHp, float MaxHp);

	void OnUpdateMyMaxHp_Implementation(float CurHp, float MaxHp);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUpdateMyMaxMp(float CurMp, float MaxMp);

	void OnUpdateMyMaxMp_Implementation(float CurHp, float MaxHp);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUpdateMySpeed(float Speed);

	void OnUpdateMySpeed_Implementation(float Speed);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUpdateMyPower(float Power);

	void OnUpdateMyPower_Implementation(float Power);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUpdateMyGold(int32 coin);

	void OnUpdateMyGold_Implementation(int32 coin);

	UFUNCTION(BlueprintCallable, Category = "Data")
	void GetSkill(ASkillBase* Skill);

	UFUNCTION()
	bool IsCanUseSkill(ASkillBase* Skill);

	// 클라이언트에서 위젯을 생성하고 서버와 동기화하는 함수
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ClientCreateAndSyncWidget();

	// 위젯이 생성되었음을 클라이언트에 동기화하는 멀티캐스트 RPC 함수
	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnWidgetCreated();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerCreateAndSyncWidget();


	FTimerHandle th_BindMyStatManager;
};

