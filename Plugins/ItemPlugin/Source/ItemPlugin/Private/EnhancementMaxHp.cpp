// Fill out your copyright notice in the Description page of Project Settings.


#include "EnhancementMaxHp.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Texture2D.h"
#include "Particles/ParticleSystemComponent.h"
#include "StatManagementComponent.h"

AEnhancementMaxHp::AEnhancementMaxHp()
{
	SkillThumbnail = CreateDefaultSubobject<UTexture2D>(TEXT("SKillTumbnail"));

	ObjName = "EnhancementMaxHp";

	ObjDescription = "Increases maximum Health by 10.";

	price = 10.0f;

	static ConstructorHelpers::FObjectFinder<UTexture2D> Thumbnail(TEXT("/ItemPlugin/BP_Items/Thumbnail/Tex_seeds_10_b.Tex_seeds_10_b"));
	if (Thumbnail.Succeeded())
	{
		SkillThumbnail = Thumbnail.Object;
	}
}

void AEnhancementMaxHp::BeginPlay()
{
	Super::BeginPlay();
}

void AEnhancementMaxHp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnhancementMaxHp::EnhancementPlayerStat()
{
	UStatManagementComponent* StatManager = Cast<UStatManagementComponent>(GetWorld()->GetFirstPlayerController()->FindComponentByClass<UStatManagementComponent>());

	if (StatManager)
	{
		StatManager->IncreaseMaxHp();
	}
}
