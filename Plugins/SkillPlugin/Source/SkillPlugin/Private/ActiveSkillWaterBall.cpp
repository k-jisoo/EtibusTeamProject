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

	CollisionSizeVector = FVector(100, 100, 100);
	SkillArea->SetBoxExtent(CollisionSizeVector);
	SkillArea->SetGenerateOverlapEvents(true);
	SkillArea->SetCollisionProfileName("OverlapAllDynamic");

	SkillBody = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	SkillBody->SetupAttachment(SkillArea);
	SkillSizeVector = FVector(1, 1, 1);
	SkillBody->SetRelativeScale3D(SkillSizeVector);
	SkillBody->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	SkillMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("SkillMovement"));
	SkillMovement->SetUpdatedComponent(SkillArea);

	SkillMovement->ProjectileGravityScale = 0;
	SkillMovement->InitialSpeed = 800.0f;
	SkillMovement->MaxSpeed = 800.0f;

	Damage = 60.0f;

	Price = 40;

	Cooldown = 10.0f;

	SkillThumbnail = CreateDefaultSubobject<UTexture2D>(TEXT("SKillTumbnail"));

	SkillName = "Water Ball";

	SkillDescription = "Blows a spherical ball of water forward, causing great damage to the target.";

	static ConstructorHelpers::FObjectFinder<UTexture2D> Thumbnail(TEXT("/SkillPlugin/BP_Skills/Thumbnail/Tex_gemstone_10_b.Tex_gemstone_10_b"));
	if (Thumbnail.Succeeded())
	{
		SkillThumbnail = Thumbnail.Object;
	}
	
	bReplicates = true;
}

void AActiveSkillWaterBall::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AActiveSkillWaterBall::ProcessBeginOverlap);

	SetLifeSpan(5.0f);
}

void AActiveSkillWaterBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AActiveSkillWaterBall::ProcessBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	/*if (HasAuthority() == false)
		return;*/

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
}

void AActiveSkillWaterBall::ApplySkillDamage()
{
	UGameplayStatics::ApplyDamage(Target, Damage, GetWorld()->GetFirstPlayerController(), nullptr, UDamageType::StaticClass());
	UE_LOG(LogClass, Warning, TEXT("ApplyDamage"));
}
