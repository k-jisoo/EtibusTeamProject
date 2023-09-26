// Fill out your copyright notice in the Description page of Project Settings.


#include "ActiveSkillWaterBall.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Texture2D.h"
#include "MonsterStatComponent.h"


AActiveSkillWaterBall::AActiveSkillWaterBall()
{
	PrimaryActorTick.bCanEverTick = true;

	SkillArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = SkillArea;
	PartX = 100;
	PartY = 100;
	PartZ = 100;
	SkillArea->SetBoxExtent(FVector(PartX, PartY, PartZ));
	SkillArea->SetGenerateOverlapEvents(true);
	SkillArea->SetCollisionProfileName("OverlapAllDynamic");

	SkillBody = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	SkillBody->SetupAttachment(SkillArea);
	SkillSize = 0.5;
	SkillBody->SetRelativeScale3D(FVector(SkillSize, SkillSize, SkillSize));
	SkillBody->SetRelativeLocation(FVector(0.0f, 0.0f, -100.0f));

	SkillMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("SkillMovement"));
	SkillMovement->SetUpdatedComponent(SkillArea);

	SkillMovement->ProjectileGravityScale = 0;
	SkillMovement->InitialSpeed = 500.0f;
	SkillMovement->MaxSpeed = 500.0f;

	Damage = 30.0f;

	Price = 30;

	Cooldown = 10.0f;

	SkillThumbnail = CreateDefaultSubobject<UTexture2D>(TEXT("SKillTumbnail"));

	SkillName = "Water Ball";

	SkillDescription = "Blows a spherical ball of water forward, causing great damage to the target.";

	static ConstructorHelpers::FObjectFinder<UTexture2D> Thumbnail(TEXT("/Game/CraftResourcesIcons/Textures/Tex_gemstone_10_b.Tex_gemstone_10_b"));
	if (Thumbnail.Succeeded())
	{
		SkillThumbnail = Thumbnail.Object;
	}
	
}

void AActiveSkillWaterBall::BeginPlay()
{
	Super::BeginPlay();

	
}

void AActiveSkillWaterBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AActiveSkillWaterBall::ProcessBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (HasAuthority() == false)
		return;

	UE_LOG(LogTemp, Warning, TEXT("MeshBeginOverlap = %s"), *OtherActor->GetName());

	UMonsterStatComponent* monster = Cast<UMonsterStatComponent>(OtherActor->FindComponentByClass<UMonsterStatComponent>());

	if (monster)
	{
		Target = OtherActor;

		ApplySkillDamage();
		UE_LOG(LogClass, Warning, TEXT("ProcessBeginOverlap"));
		Destroy();
		// 사라지기 전 파티클 발생 후 사라지기
	}

	// 스킬 레벨업 확인 용
	SkillLevelUp();
}

void AActiveSkillWaterBall::SkillLevelUp()
{
	Level += 1;
	Damage += 30.0f;
	PartX += 100;
	PartY += 100;
	PartZ += 200;
	SkillSize += 0.5;
	SkillArea->SetBoxExtent(FVector(PartX, PartY, PartZ));
	SkillBody->SetRelativeScale3D(FVector(SkillSize, SkillSize, SkillSize));
}

void AActiveSkillWaterBall::ApplySkillDamage()
{
	UGameplayStatics::ApplyDamage(Target, Damage, GetWorld()->GetFirstPlayerController(), nullptr, UDamageType::StaticClass());
	UE_LOG(LogClass, Warning, TEXT("ApplyDamage"));
}
