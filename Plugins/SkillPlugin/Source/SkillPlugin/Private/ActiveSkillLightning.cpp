// Fill out your copyright notice in the Description page of Project Settings.


#include "ActiveSkillLightning.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Texture2D.h"
#include "MonsterStatComponent.h"
#include "SkillManagementComponent.h"



AActiveSkillLightning::AActiveSkillLightning()
{
	PrimaryActorTick.bCanEverTick = true;

	SkillArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = SkillArea;

	CollisionSizeVector = FVector(200, 200, 200);
	SkillArea->SetBoxExtent(CollisionSizeVector);
	SkillArea->SetGenerateOverlapEvents(true);
	SkillArea->SetCollisionProfileName("OverlapAllDynamic");

	SkillBody = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	SkillBody->SetupAttachment(SkillArea);
	SkillSizeVector = FVector(1, 1, 1);
	SkillBody->SetRelativeScale3D(SkillSizeVector);
	SkillBody->SetRelativeLocation(FVector(0.0f, 0.0f, -100.0f));

	static ConstructorHelpers::FObjectFinder<UParticleSystemComponent> ParticleAsset(TEXT("/Game/FXVarietyPack/Particles/P_ky_lightning3.P_ky_lightning3"));
	if (ParticleAsset.Succeeded())
	{
		SkillBody = ParticleAsset.Object;
	}

	SkillMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("SkillMovement"));
	SkillMovement->SetUpdatedComponent(SkillArea);

	SkillMovement->ProjectileGravityScale = 0;
	SkillMovement->InitialSpeed = 500.0f;
	SkillMovement->MaxSpeed = 500.0f;

	Damage = 20.0f;

	Price = 30;

	Cooldown = 10.0f;

	SkillThumbnail = CreateDefaultSubobject<UTexture2D>(TEXT("SKillTumbnail"));

	SkillName = "Lightning";

	SkillDescription = "To enemies in a wide area to prevent lightning strikes on the front.";

	static ConstructorHelpers::FObjectFinder<UTexture2D> Thumbnail(TEXT("/SkillPlugin/BP_Skills/Thumbnail/Tex_gemstone_07_b.Tex_gemstone_07_b"));
	if (Thumbnail.Succeeded())
	{
		SkillThumbnail = Thumbnail.Object;
	}

	bReplicates = true;
}

void AActiveSkillLightning::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AActiveSkillLightning::ProcessBeginOverlap);

	SetLifeSpan(2.0f);
}

void AActiveSkillLightning::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AActiveSkillLightning::ProcessBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	//if (HasAuthority() == false)
	//	return;

	UE_LOG(LogTemp, Warning, TEXT("MeshBeginOverlap = %s"), *OtherActor->GetName());

	UMonsterStatComponent* monster = Cast<UMonsterStatComponent>(OtherActor->FindComponentByClass<UMonsterStatComponent>());

	if (monster)
	{
		Target = OtherActor;

		ApplySkillDamage();
		UE_LOG(LogClass, Warning, TEXT("ProcessBeginOverlap"));
	}
}

void AActiveSkillLightning::ApplySkillDamage()
{
	UGameplayStatics::ApplyDamage(Target, Damage, GetWorld()->GetFirstPlayerController(), nullptr, UDamageType::StaticClass());
	UE_LOG(LogClass, Warning, TEXT("ApplyDamage"));
}

