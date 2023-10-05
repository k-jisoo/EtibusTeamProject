// Fill out your copyright notice in the Description page of Project Settings.


#include "ActiveSkillStorm.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Texture2D.h"
#include "MonsterStatComponent.h"


AActiveSkillStorm::AActiveSkillStorm()
{
	PrimaryActorTick.bCanEverTick = true;

	SkillArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = SkillArea;

	CollisionSizeVector = FVector(150, 150, 250);
	SkillArea->SetBoxExtent(CollisionSizeVector);
	SkillArea->SetGenerateOverlapEvents(true);
	SkillArea->SetCollisionProfileName("OverlapAllDynamic");

	SkillBody = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	SkillBody->SetupAttachment(SkillArea);

	SkillSizeVector = FVector(1, 1, 1);
	SkillBody->SetRelativeScale3D(SkillSizeVector);
	SkillBody->SetRelativeLocation(FVector(0.0f, 0.0f, -200.0f));

	static ConstructorHelpers::FObjectFinder<UParticleSystemComponent> ParticleAsset(TEXT("/Game/FXVarietyPack/Particles/P_ky_fireStorm.P_ky_fireStorm"));
	if (ParticleAsset.Succeeded())
	{
		SkillBody = ParticleAsset.Object;
	}

	SkillMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("SkillMovement"));
	SkillMovement->SetUpdatedComponent(SkillArea);

	SkillMovement->ProjectileGravityScale = 0;
	SkillMovement->InitialSpeed = 500.0f;
	SkillMovement->MaxSpeed = 500.0f;

	Damage = 5.0f;

	Price = 30;

	Cooldown = 10.0f;

	SkillThumbnail = CreateDefaultSubobject<UTexture2D>(TEXT("SKillTumbnail"));

	SkillName = "Fire Storm";

	SkillDescription = "Summons a flame vortex in front, causing continuous damage to enemies.";

	static ConstructorHelpers::FObjectFinder<UTexture2D> Thumbnail(TEXT("/SkillPlugin/BP_Skills/Thumbnail/Tex_gemstone_09_b.Tex_gemstone_09_b"));
	if (Thumbnail.Succeeded())
	{
		SkillThumbnail = Thumbnail.Object;
	}
	
	bReplicates = true;
}

void AActiveSkillStorm::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AActiveSkillStorm::ProcessBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AActiveSkillStorm::EndOverlap);


	SetLifeSpan(10.0f);
}

void AActiveSkillStorm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AActiveSkillStorm::ProcessBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{

	UE_LOG(LogTemp, Warning, TEXT("MeshBeginOverlap = %s"), *OtherActor->GetName());

	UMonsterStatComponent* monster = Cast<UMonsterStatComponent>(OtherActor->FindComponentByClass<UMonsterStatComponent>());

	if (monster)
	{
		Target = OtherActor;

		ApplySkillDamage();
		UE_LOG(LogClass, Warning, TEXT("ProcessBeginOverlap"));
	}
}

void AActiveSkillStorm::ApplySkillDamage()
{
	UGameplayStatics::ApplyDamage(Target, Damage, GetWorld()->GetFirstPlayerController(), nullptr, UDamageType::StaticClass());
	UE_LOG(LogClass, Warning, TEXT("ApplyDamage"));

	// 틱데미지 구현
	FTimerManager& timerManager = GetWorld()->GetTimerManager();
	timerManager.SetTimer(Th_ProcessBeginOverlap, this, &AActiveSkillStorm::ApplySkillDamage, 0.5f, true);
}

void AActiveSkillStorm::EndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	FTimerManager& timerManager = GetWorld()->GetTimerManager();
	timerManager.ClearTimer(Th_ProcessBeginOverlap);
}