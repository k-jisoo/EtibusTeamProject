// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

class UAnimBlueprint;
class UAnimMontage;
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

public:
	//UFUNCTION(Server, Unreliable)
	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_SendMessage(FText const& Message); //호출용 -> Network -> RPC
	bool C2S_SendMessage_Validate(FText const& Message);
	void C2S_SendMessage_Implementation(FText const& Message);

	UFUNCTION(Client, Reliable)
	void S2C_SendMessage(FText const& Message);  //호출용 -> Network -> RPC
	void S2C_SendMessage_Implementation(FText const& Message);


	UFUNCTION(Server, Reliable)
	void ReqSetLobbyCharacter(USkeletalMesh* skeletalMesh, UAnimBlueprint* animBp);

	void ReqSetLobbyCharacter_Implementation(USkeletalMesh* skeletalMesh, UAnimBlueprint* animBp);

	UFUNCTION(Client, Reliable)
	void RecSetLobbyCharacter(USkeletalMesh* skeletalMesh, UAnimBlueprint* animBp);

	void RecSetLobbyCharacter_Implementation(USkeletalMesh* skeletalMesh, UAnimBlueprint* animBp);

	UFUNCTION(Server, Reliable)
	void ReqPlayAnimMontage(UAnimMontage* levelStartMontage);

	void ReqPlayAnimMontage_Implementation(UAnimMontage* levelStartMontage);

	UFUNCTION(Client, Reliable)
	void RecPlayAnimMontage(UAnimMontage* levelStartMontage);

	void RecPlayAnimMontage_Implementation(UAnimMontage* levelStartMontage);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> LobbyWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* LobbyWidget;
};
