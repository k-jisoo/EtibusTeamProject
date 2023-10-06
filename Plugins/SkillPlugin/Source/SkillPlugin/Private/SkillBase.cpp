// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillBase.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASkillBase::ASkillBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void ASkillBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASkillBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASkillBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASkillBase, SkillArea);
	DOREPLIFETIME(ASkillBase, Damage);
	DOREPLIFETIME(ASkillBase, SkillBody);
	DOREPLIFETIME(ASkillBase, SkillSizeVector);
	DOREPLIFETIME(ASkillBase, CollisionSizeVector);
	DOREPLIFETIME(ASkillBase, SkillName);
	DOREPLIFETIME(ASkillBase, Price);
	DOREPLIFETIME(ASkillBase, SkillDescription);
	DOREPLIFETIME(ASkillBase, SkillThumbnail);
	DOREPLIFETIME(ASkillBase, OwnChar);
}

void ASkillBase::ProcessBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
}


