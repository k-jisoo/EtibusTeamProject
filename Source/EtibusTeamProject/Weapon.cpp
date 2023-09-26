// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "BaseCharacter.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "DrawDebugHelpers.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Sphere"));
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Box->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Box->SetCollisionProfileName(TEXT("Custom"));
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &AWeapon::ReqOnBoxComponentBeginOverlap);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::SetSphereCollisionState(bool state)
{
	if (state)
	{
		Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::SetOwnChar(ABaseCharacter* Char)
{
	OwnChar = Char;
}

void AWeapon::ReqOnBoxComponentBeginOverlap_Implementation(AActor* OverlappedActor, AActor* OtherActor)
{
	RecOnBoxComponentBeginOverlap(OverlappedActor, OtherActor);
}


void AWeapon::RecOnBoxComponentBeginOverlap_Implementation(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!OtherActor || Cast<ABaseCharacter>(OtherActor))
		return;

	AEnemy* HitChar = Cast<AEnemy>(OtherActor);

	if (!HitChar)
		return;

	UGameplayStatics::ApplyDamage(HitChar, OwnChar->Damage, OwnChar->GetController(), OwnChar, UDamageType::StaticClass());

	TArray<FHitResult> HitResults;
	FVector StartLocation = OwnChar->GetActorLocation();
	FVector EndLocation = OtherActor->GetActorLocation();

	FCollisionObjectQueryParams collisionObjectQuery;
	collisionObjectQuery.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	collisionObjectQuery.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	collisionObjectQuery.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	collisionObjectQuery.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);
	collisionObjectQuery.AddObjectTypesToQuery(ECollisionChannel::ECC_Vehicle);
	collisionObjectQuery.AddObjectTypesToQuery(ECollisionChannel::ECC_Destructible);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(OwnChar);
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceMultiByObjectType(HitResults, StartLocation, EndLocation, collisionObjectQuery, CollisionParams);

	if (!bHit)
		return;

	for (int i = 0; i < HitResults.Num(); i++)
	{
		FVector ParticlePoint = HitResults[i].ImpactPoint;
		FVector HitNormal = HitResults[i].ImpactNormal;
		FRotator Rotation = HitNormal.Rotation();

		if (!(OwnChar->HitParticle))
			return;
		UParticleSystemComponent* ParticleSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OwnChar->HitParticle, ParticlePoint, Rotation, true);
	}
}
