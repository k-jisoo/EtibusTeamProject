// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimBlueprint.h"
#include "Animation/AnimInstance.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Weapon.h"
#include "TPGameInstance.h"
#include "Particles/ParticleSystem.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -88.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 100.0f);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	IsAttacking = false;
	IsSaveAttack = false;
	AttackCount = 0;
	IsInputPossible = true;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	UTPGameInstance* GI = Cast<UTPGameInstance>(GetWorld()->GetGameInstance());
	
	if (!(GI->MyCharacter))
		return;

	if (HasAuthority())
	{
		RecSetCharacter(GI->MyCharacter->SkeletalMesh, GI->MyCharacter->AnimBP, GI->MyCharacter->FirstAttackMontage, GI->MyCharacter->SecondAttackMontage, GI->MyCharacter->ThirdAttackMontage, GI->MyCharacter->FourthAttackMontage, GI->MyCharacter->LevelStartMontage, GI->MyCharacter->MaxHp, GI->MyCharacter->Damage, GI->MyCharacter->Speed, GI->MyCharacter->CapsuleHeight, GI->MyCharacter->CapsuleRadius, GI->MyCharacter->BoxCollisionExt, GI->MyCharacter->HitParticle);
	}
	else
	{
		ReqSetCharacter(GI->MyCharacter->SkeletalMesh, GI->MyCharacter->AnimBP, GI->MyCharacter->FirstAttackMontage, GI->MyCharacter->SecondAttackMontage, GI->MyCharacter->ThirdAttackMontage, GI->MyCharacter->FourthAttackMontage, GI->MyCharacter->LevelStartMontage, GI->MyCharacter->MaxHp, GI->MyCharacter->Damage, GI->MyCharacter->Speed, GI->MyCharacter->CapsuleHeight, GI->MyCharacter->CapsuleRadius, GI->MyCharacter->BoxCollisionExt, GI->MyCharacter->HitParticle);
	}

	SetWeapon();

	UE_LOG(LogTemp, Warning, TEXT("WorldName : %s, name : %s, %f, %f, %f, %f"), *GetWorld()->GetName(), *GI->MyCharacter->SkeletalMesh->GetName(), MaxHp, Damage, GetCapsuleComponent()->GetScaledCapsuleHalfHeight(), GetCapsuleComponent()->GetScaledCapsuleRadius());

}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ABaseCharacter::Look);

		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);

		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ABaseCharacter::Jump);

		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ABaseCharacter::StopJump);

		EnhancedInputComponent->BindAction(IA_Attack, ETriggerEvent::Started, this, &ABaseCharacter::Attack);
	}
}



float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	return 0.0f;
}

void ABaseCharacter::Look(const FInputActionValue& Value)
{
	if (!IsInputPossible)
		return;

	FVector2D v = Value.Get<FVector2D>();
	AddControllerYawInput(v.X);
	AddControllerPitchInput(v.Y);
}

void ABaseCharacter::Move(const FInputActionValue& Value)
{
	if (!IsInputPossible)
		return;

	FVector2D MoveVector = Value.Get<FVector2D>();

	FRotator controlRot = GetControlRotation();
	FRotator controlYawRot = FRotator(0, controlRot.Yaw, 0);

	FVector PawnForwardVector = UKismetMathLibrary::GetForwardVector(controlYawRot);
	FVector PawnRightVector = UKismetMathLibrary::GetRightVector(controlYawRot);

	AddMovementInput(PawnForwardVector, MoveVector.Y);
	AddMovementInput(PawnRightVector, MoveVector.X);
}

void ABaseCharacter::Jump(const FInputActionValue& Value)
{
	if (!IsInputPossible)
		return;

	ACharacter::Jump();
}

void ABaseCharacter::StopJump(const FInputActionValue& Value)
{
	ACharacter::StopJumping();
}

void ABaseCharacter::Attack(const FInputActionValue& Value)
{
	ReqAttack();
}

void ABaseCharacter::ReqAttack_Implementation()
{
	if (!IsInputPossible)
		return;

	if (IsAttacking)
	{
		IsSaveAttack = true;
		return;
	}

	IsAttacking = true;

	AttackSwitch();
}

void ABaseCharacter::AttackSwitch_Implementation()
{
	switch (AttackCount)
	{
	case 0:
		AttackCount = 1;
		if(FirstAttackMontage)
			PlayAnimMontage(FirstAttackMontage);
		break;
	case 1:
		AttackCount = 2;
		if (SecondAttackMontage)
			PlayAnimMontage(SecondAttackMontage);
		break;
	case 2:
		AttackCount = 3;
		if (ThirdAttackMontage)
			PlayAnimMontage(ThirdAttackMontage);
		break;
	case 3:
		AttackCount = 0;
		if (FourthAttackMontage)
			PlayAnimMontage(FourthAttackMontage);
		break;
	}
}

void ABaseCharacter::ComboAttackSave()
{
	if (!IsSaveAttack)
		return;

	IsSaveAttack = false;
	
	AttackSwitch();
}

void ABaseCharacter::ResetCombo()
{
	AttackCount = 0;
	IsSaveAttack = false;
	IsAttacking = false;
}

void ABaseCharacter::SetInputPossible()
{
	IsInputPossible = true;
}

void ABaseCharacter::ReqSetLobbyCharacter_Implementation(USkeletalMesh* SkeletalMesh, UAnimBlueprint* AnimBP)
{
	SetLobbyCharacter(SkeletalMesh, AnimBP);
}

void ABaseCharacter::SetLobbyCharacter_Implementation(USkeletalMesh* SkeletalMesh, UAnimBlueprint* AnimBP)
{
	GetMesh()->SetSkeletalMesh(SkeletalMesh);
	GetMesh()->SetAnimInstanceClass(AnimBP->GetAnimBlueprintGeneratedClass());
}

void ABaseCharacter::ReqSetCharacter_Implementation(USkeletalMesh* skeletalMesh, UAnimBlueprint* animBp, UAnimMontage* firstAttackMontage, UAnimMontage* secondAttackMontage, UAnimMontage* thirdAttackMontage, UAnimMontage* fourthAttackMontage, UAnimMontage* levelStartMontage, float maxHp, float damage, float speed, float capsuleHeight, float capsuleRadius, FVector boxCollisionExt, UParticleSystem* hitParticle)
{
	RecSetCharacter(skeletalMesh, animBp, firstAttackMontage, secondAttackMontage, thirdAttackMontage, fourthAttackMontage, levelStartMontage, maxHp, damage, speed, capsuleHeight, capsuleRadius, boxCollisionExt, hitParticle);
	
}

void ABaseCharacter::RecSetCharacter_Implementation(USkeletalMesh* skeletalMesh, UAnimBlueprint* animBp, UAnimMontage* firstAttackMontage, UAnimMontage* secondAttackMontage, UAnimMontage* thirdAttackMontage, UAnimMontage* fourthAttackMontage, UAnimMontage* levelStartMontage, float maxHp, float damage, float speed, float capsuleHeight, float capsuleRadius, FVector boxCollisionExt, UParticleSystem* hitParticle)
{
	GetMesh()->SetSkeletalMesh(skeletalMesh);
	GetMesh()->SetAnimInstanceClass(animBp->GetAnimBlueprintGeneratedClass());
	GetMesh()->SetRelativeLocation(FVector(0, 0, capsuleHeight * -1));

	this->FirstAttackMontage = firstAttackMontage;
	this->SecondAttackMontage = secondAttackMontage;
	this->ThirdAttackMontage = thirdAttackMontage;
	this->FourthAttackMontage = fourthAttackMontage;
	this->LevelStartMontage = levelStartMontage;
	this->MaxHp = maxHp;
	this->Damage = damage;
	this->HitParticle = hitParticle;
	CurHp = maxHp;

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	if (!CapsuleComp)
		return;

	CapsuleComp->SetCapsuleHalfHeight(capsuleHeight);
	CapsuleComp->SetCapsuleRadius(capsuleRadius);


	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();

	if (!CharacterMovementComponent)
		return;

	CharacterMovementComponent->MaxWalkSpeed = speed;
}

void ABaseCharacter::SetWeapon()
{
	UTPGameInstance* GI = Cast<UTPGameInstance>(GetWorld()->GetGameInstance());

	if (!GI || !GI->MyCharacter)
		return;

	Weapon = GetWorld()->SpawnActor<AWeapon>(AWeapon::StaticClass(), FVector(0, 0, 0), FRotator::ZeroRotator);

	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("weaponCollision"));

	FVector Ext = GI->MyCharacter->BoxCollisionExt;
	Weapon->Box->SetBoxExtent(Ext);
	Weapon->Box->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	Weapon->Box->SetRelativeLocation(FVector(0.0f, Ext.X, 0.0f));

	Weapon->OwnChar = this;
}

void ABaseCharacter::ReqPlayAnimMontage_Implementation(UAnimMontage* animMontage)
{
	ClientPlayAnimMontage(animMontage);
}

void ABaseCharacter::ClientPlayAnimMontage_Implementation(UAnimMontage* animMontage)
{
	this->PlayAnimMontage(animMontage);
}






