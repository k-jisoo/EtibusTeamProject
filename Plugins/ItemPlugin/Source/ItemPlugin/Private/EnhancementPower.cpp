// Fill out your copyright notice in the Description page of Project Settings.


#include "EnhancementPower.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Texture2D.h"
#include "Particles/ParticleSystemComponent.h"
#include "StatManagementComponent.h"

AEnhancementPower::AEnhancementPower()
{
	SkillThumbnail = CreateDefaultSubobject<UTexture2D>(TEXT("SKillTumbnail"));

	ObjName = "EnhancementPower";

	ObjDescription = "Increases maximum Power by 10.";

	price = 10.0f;

	static ConstructorHelpers::FObjectFinder<UTexture2D> Thumbnail(TEXT("/ItemPlugin/BP_Items/Thumbnail/Tex_seeds_07_b.Tex_seeds_07_b"));
	if (Thumbnail.Succeeded())
	{
		SkillThumbnail = Thumbnail.Object;
	}
}

void AEnhancementPower::BeginPlay()
{
	Super::BeginPlay();
}

void AEnhancementPower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnhancementPower::EnhancementPlayerStat()
{
	UStatManagementComponent* StatManager = Cast<UStatManagementComponent>(GetWorld()->GetFirstPlayerController()->FindComponentByClass<UStatManagementComponent>());

	if (StatManager)
	{
		StatManager->IncreasePower();
	}
}
