// Fill out your copyright notice in the Description page of Project Settings.

#include "EnhancementMaxMp.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Texture2D.h"
#include "Particles/ParticleSystemComponent.h"
#include "StatManagementComponent.h"

AEnhancementMaxMp::AEnhancementMaxMp()
{
	SkillThumbnail = CreateDefaultSubobject<UTexture2D>(TEXT("SKillTumbnail"));

	ObjName = "EnhancementMaxMp";

	ObjDescription = "Increases maximum Mana by 10.";

	price = 10.0f;

	static ConstructorHelpers::FObjectFinder<UTexture2D> Thumbnail(TEXT("/ItemPlugin/BP_Items/Thumbnail/Tex_seeds_09_b.Tex_seeds_09_b"));
	if (Thumbnail.Succeeded())
	{
		SkillThumbnail = Thumbnail.Object;
	}
}

void AEnhancementMaxMp::BeginPlay()
{
	Super::BeginPlay();
}

void AEnhancementMaxMp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnhancementMaxMp::EnhancementPlayerStat()
{
	UStatManagementComponent* StatManager = Cast<UStatManagementComponent>(GetWorld()->GetFirstPlayerController()->FindComponentByClass<UStatManagementComponent>());

	if (StatManager)
	{
		StatManager->IncreaseMaxMp();
	}
}
