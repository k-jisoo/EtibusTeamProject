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
	PartX = 150;
	PartY = 150;
	PartZ = 150;
	SkillArea->SetBoxExtent(FVector(PartX, PartY, PartZ));
	SkillArea->SetGenerateOverlapEvents(true);
	SkillArea->SetCollisionProfileName("OverlapAllDynamic");

	SkillBody = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	SkillBody->SetupAttachment(SkillArea);
	SkillBody->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	SkillBody->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

	Damage = 50.0f;

	Price = 50;

	SkillThumbnail = CreateDefaultSubobject<UTexture2D>(TEXT("SKillTumbnail"));

	SkillName = "Defense Area";

	SkillDescription = "It is created around the player and inflicts continuous damage to enemies that come within range.";

	static ConstructorHelpers::FObjectFinder<UTexture2D> Thumbnail(TEXT("/Game/CraftResourcesIcons/Textures/Tex_gemstone_08_b.Tex_gemstone_08_b"));
	if (Thumbnail.Succeeded())
	{
		SkillThumbnail = Thumbnail.Object;
	}
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
	if (HasAuthority() == false)
		return;

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

	// ��ų ������ Ȯ�� ��
	SkillLevelUp();
}

void APassiveSkillDefenseArea::ApplySkillDamage()
{
	UGameplayStatics::ApplyDamage(Target, Damage, GetWorld()->GetFirstPlayerController(), nullptr, UDamageType::StaticClass());
	UE_LOG(LogClass, Warning, TEXT("ApplyDamage"));

	// ƽ������ ����
	/*FTimerManager& timerManager = GetWorld()->GetTimerManager();
	timerManager.SetTimer(Th_ProcessBeginOverlap, this, &APassiveSkillDefenseArea::ApplySkillDamage, 0.5f, false);*/
}

void APassiveSkillDefenseArea::SkillLevelUp()
{
	Level += 1;
	Damage += 30.0f;
	PartX += 150;
	PartY += 150;
	SkillSize += 0.5;
	SkillArea->SetBoxExtent(FVector(PartX, PartY, PartZ));
	SkillBody->SetRelativeScale3D(FVector(SkillSize, SkillSize, SkillSize));
}
