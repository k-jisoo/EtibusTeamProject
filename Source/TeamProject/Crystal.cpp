// Fill out your copyright notice in the Description page of Project Settings.


#include "Crystal.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Enemy.h"

// Sets default values
ACrystal::ACrystal()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//
	
	/*Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(130.f, 110.f, 250.f));*/	

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Capsule->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Capsule->SetCollisionProfileName(TEXT("Pawn"));
	Capsule->SetCapsuleRadius(150.f);
	Capsule->SetCapsuleHalfHeight(300.f);

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Box);
	Body->SetWorldScale3D(FVector(0.1f,0.1f,0.1f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/MyContent/Blueprints/Crystal/source/Fantasy_crystal_stone.Fantasy_crystal_stone'"));
	if (TempMesh.Succeeded())
	{
		Body->SetStaticMesh(TempMesh.Object);
	}

	bLive = true;
	MaxHp = 100.0f;
	CurHp = MaxHp;
	

}

// Called when the game starts or when spawned
void ACrystal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACrystal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACrystal::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//데미지를 받는 함수
float ACrystal::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (EventInstigator == nullptr)
		return 0.0f;

	UpdateHp(Damage * -1);

	//UE_LOG(LogTemp, Warning, TEXT("%s took %f damage"), *GetName(), Damage);
	
	return Damage;
}

void ACrystal::Die()
{
	bLive = false;

	Destroy();
}

void ACrystal::UpdateHp(float Amount)
{

	CurHp += Amount;
	CurHp = FMath::Clamp(CurHp, 0.0f, MaxHp);

	UE_LOG(LogTemp, Warning, TEXT("%f"), CurHp);

	if (CurHp == 0)
	{
		Die();
	}

}







