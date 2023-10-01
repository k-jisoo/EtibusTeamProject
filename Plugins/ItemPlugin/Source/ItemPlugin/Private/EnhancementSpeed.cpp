// Fill out your copyright notice in the Description page of Project Settings.


#include "EnhancementSpeed.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Texture2D.h"
#include "Particles/ParticleSystemComponent.h"
#include "StatManagementComponent.h"

AEnhancementSpeed::AEnhancementSpeed()
{
	SkillThumbnail = CreateDefaultSubobject<UTexture2D>(TEXT("SKillTumbnail"));

	ObjName = "EnhancementSpeed";

	ObjDescription = "Increases maximum Power by 10.";

	price = 10.0f;

	static ConstructorHelpers::FObjectFinder<UTexture2D> Thumbnail(TEXT("/ItemPlugin/BP_Items/Thumbnail/Tex_seeds_06_b.Tex_seeds_06_b"));
	if (Thumbnail.Succeeded())
	{
		SkillThumbnail = Thumbnail.Object;
	}
}

void AEnhancementSpeed::BeginPlay()
{
	Super::BeginPlay();
}

void AEnhancementSpeed::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnhancementSpeed::EnhancementPlayerStat()
{
	UStatManagementComponent* StatManager = Cast<UStatManagementComponent>(GetWorld()->GetFirstPlayerController()->FindComponentByClass<UStatManagementComponent>());

	if (StatManager)
	{
		StatManager->IncreaseSpeed();
	}
}
