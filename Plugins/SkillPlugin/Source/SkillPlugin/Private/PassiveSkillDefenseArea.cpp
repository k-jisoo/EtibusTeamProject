// Fill out your copyright notice in the Description page of Project Settings.


#include "PassiveSkillDefenseArea.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Texture2D.h"
#include "MonsterStatComponent.h"




APassiveSkillDefenseArea::APassiveSkillDefenseArea()
{
	PrimaryActorTick.bCanEverTick = true;

	SkillArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = SkillArea;

	CollisionSizeVector = FVector(150, 150, 150);
	SkillArea->SetBoxExtent(CollisionSizeVector);
	SkillArea->SetGenerateOverlapEvents(true);
	SkillArea->SetCollisionProfileName("OverlapAllDynamic");

	SkillBody = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	SkillBody->SetupAttachment(SkillArea);
	
	SkillSizeVector = FVector(1, 1, 1);
	SkillBody->SetRelativeScale3D(SkillSizeVector);
	SkillBody->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));

	Damage = 3.0f;

	Price = 50;

	SkillThumbnail = CreateDefaultSubobject<UTexture2D>(TEXT("SKillTumbnail"));

	SkillName = "Defense Area";

	SkillDescription = "It is created around the player and inflicts continuous damage to enemies that come within range.";

	static ConstructorHelpers::FObjectFinder<UTexture2D> Thumbnail(TEXT("/SkillPlugin/BP_Skills/Thumbnail/Tex_gemstone_08_b.Tex_gemstone_08_b"));
	if (Thumbnail.Succeeded())
	{
		SkillThumbnail = Thumbnail.Object;
	}

	bReplicates = true;
}

void APassiveSkillDefenseArea::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &APassiveSkillDefenseArea::ProcessBeginOverlap);
}

void APassiveSkillDefenseArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APassiveSkillDefenseArea::ProcessBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{

	UE_LOG(LogTemp, Warning, TEXT("MeshBeginOverlap = %s"), *OtherActor->GetName());

	UMonsterStatComponent* monster = Cast<UMonsterStatComponent>(OtherActor->FindComponentByClass<UMonsterStatComponent>());

	if (monster)
	{
		Target = OtherActor;

		SkillArea->SetSimulatePhysics(false);

		AttachToActor(OtherActor, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("spine_01"));

		ApplySkillDamage();
		UE_LOG(LogClass, Warning, TEXT("ProcessBeginOverlap"));
	}
}

void APassiveSkillDefenseArea::ApplySkillDamage()
{
	UGameplayStatics::ApplyDamage(Target, Damage, GetWorld()->GetFirstPlayerController(), nullptr, UDamageType::StaticClass());
	UE_LOG(LogClass, Warning, TEXT("ApplyDamage"));

	// 틱데미지 구현
	FTimerManager& timerManager = GetWorld()->GetTimerManager();
	timerManager.SetTimer(Th_ProcessBeginOverlap, this, &APassiveSkillDefenseArea::ApplySkillDamage, 0.5f, true);
}

void APassiveSkillDefenseArea::EndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	FTimerManager& timerManager = GetWorld()->GetTimerManager();
	timerManager.ClearTimer(Th_ProcessBeginOverlap);
}
