// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "Components/SphereComponent.h"
#include "ItemInterface.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMesh");
	ItemSphere = CreateDefaultSubobject<USphereComponent>("ItemSphere");
	ItemSphere->SetSphereRadius(20.0f);
	ItemMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -30.0f));

	SetRootComponent(ItemSphere);
	ItemMesh->SetupAttachment(RootComponent);

	bReplicates = true;

	ItemMesh->SetCollisionProfileName("OverlapAllDynamic");

	ItemSphere->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::MeshBeginOverlap);

	
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	this->AddActorLocalRotation(FRotator(
		0,
		400.0 * DeltaTime,
		0));

}

void AItemBase::MeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority() == false)
		return;

	UE_LOG(LogTemp, Warning, TEXT("MeshBeginOverlap"));

	IItemInterface* InterfaceObj = Cast<IItemInterface>(OtherActor);

	if (InterfaceObj == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("InterfaceObj null"));
		return;
	}

	InterfaceObj->Execute_EventGetItem(OtherActor, eItemType);

	UE_LOG(LogTemp, Warning, TEXT("CallEvent"));

	Destroy();
}

