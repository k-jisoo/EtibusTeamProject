// Fill out your copyright notice in the Description page of Project Settings.


#include "EntryPawn.h"

// Sets default values
AEntryPawn::AEntryPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEntryPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEntryPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEntryPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

