// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimBlueprint.h"
#include "Animation/AnimInstance.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* LocalPlayerSystem
		= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer());

	if (LocalPlayerSystem && DefaultMappingContext)
	{
		LocalPlayerSystem->AddMappingContext(DefaultMappingContext, 0);
	}
	else
	{
		if (!LocalPlayerSystem)
		{
			UE_LOG(LogTemp, Warning, TEXT("Not Found LocalPlayerSystem"));
		}
	}


	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
}

void AMainPlayerController::ReqSetMainCharacter_Implementation(USkeletalMesh* skeletalMesh, UAnimBlueprint* animBp, UAnimMontage* firstAttackMontage, UAnimMontage* secondAttackMontage, UAnimMontage* thirdAttackMontage, UAnimMontage* fourthAttackMontage, UAnimMontage* levelStartMontage, float maxHp, float damage, float speed, float capsuleHeight, float capsuleRadius, FVector boxCollisionExt, UParticleSystem* hitParticle)
{
	RecSetMainCharacter(skeletalMesh, animBp, firstAttackMontage, secondAttackMontage, thirdAttackMontage, fourthAttackMontage, levelStartMontage, maxHp, damage, speed, capsuleHeight, capsuleRadius, boxCollisionExt, hitParticle);
}

void AMainPlayerController::RecSetMainCharacter_Implementation(USkeletalMesh* skeletalMesh, UAnimBlueprint* animBp, UAnimMontage* firstAttackMontage, UAnimMontage* secondAttackMontage, UAnimMontage* thirdAttackMontage, UAnimMontage* fourthAttackMontage, UAnimMontage* levelStartMontage, float maxHp, float damage, float speed, float capsuleHeight, float capsuleRadius, FVector boxCollisionExt, UParticleSystem* hitParticle)
{
	ABaseCharacter* MyChar = Cast<ABaseCharacter>(GetPawn());

	if (!MyChar)
		return;

	MyChar->GetMesh()->SetSkeletalMesh(skeletalMesh);
	MyChar->GetMesh()->SetAnimInstanceClass(animBp->GetAnimBlueprintGeneratedClass());
	MyChar->GetMesh()->SetRelativeLocation(FVector(0, 0, capsuleHeight * -1));

	MyChar->FirstAttackMontage = firstAttackMontage;
	MyChar->SecondAttackMontage = secondAttackMontage;
	MyChar->ThirdAttackMontage = thirdAttackMontage;
	MyChar->FourthAttackMontage = fourthAttackMontage;
	MyChar->LevelStartMontage = levelStartMontage;
	MyChar->MaxHp = maxHp;
	MyChar->Damage = damage;
	MyChar->HitParticle = hitParticle;
	MyChar->CurHp = maxHp;

	UCapsuleComponent* CapsuleComp = MyChar->GetCapsuleComponent();

	if (!CapsuleComp)
		return;

	CapsuleComp->SetCapsuleHalfHeight(capsuleHeight);
	CapsuleComp->SetCapsuleRadius(capsuleRadius);


	UCharacterMovementComponent* CharacterMovementComponent = MyChar->GetCharacterMovement();

	if (!CharacterMovementComponent)
		return;

	CharacterMovementComponent->MaxWalkSpeed = speed;
}
