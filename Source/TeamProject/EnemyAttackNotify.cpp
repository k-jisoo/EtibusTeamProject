// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAttackNotify.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"

void UEnemyAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    
	FVector SocketVector = MeshComp->GetSocketLocation(Socket);

    // 스피어 트레이스의 시작 위치
    FVector TraceStart = MeshComp->GetSocketLocation(Socket);

    // 스피어 트레이스의 방향
    FVector TraceEnd = MeshComp->GetSocketLocation(Socket); // TraceDistance를 원하는 길이로 설정하세요.

    // 스피어 트레이스 반지름
    float TraceRadius = 75.0f; // 원하는 스피어 반지름으로 설정하세요.

    // 스피어 트레이스 결과를 저장할 변수
    FHitResult HitResult;

    // 트레이스할 객체의 콜리전 채널 목록 (ECollisionChannel을 포함한 배열)
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)); // 예: Pawn과 충돌하는 객체를 검출하려면 ECC_Pawn을 사용

    // 무시할 액터 (옵션) - 무시하려는 액터가 없으면 nullptr
    AActor* IgnoreActor = MeshComp->GetOwner();

    // SphereTrace를 실행
    bool bHit = GetWorld()->SweepSingleByObjectType(
        HitResult,          // 결과를 저장할 변수
        TraceStart,         // 시작 위치
        TraceEnd,           // 끝 위치
        FQuat::Identity,    // 회전 없음
        ObjectTypes,        // 검출할 객체의 콜리전 채널 목록
        FCollisionShape::MakeSphere(TraceRadius), // 스피어 형태의 콜리전 모양
        FCollisionQueryParams(FName(TEXT("SphereTrace")), false, IgnoreActor) // 쿼리 파라미터
    );
    FVector HitLocation;
    if (bHit)
    {
        // 충돌한 객체가 있을 경우 처리
        AActor* HitActor = HitResult.GetActor();
        HitLocation = HitResult.ImpactPoint;

       TArray<FName> ActorTags = HitActor->Tags;
        for (FName Tag : ActorTags)
        {
            if (Tag == "Player" || Tag == "Crystal")
            {
                UE_LOG(LogTemp, Warning, TEXT("apply Damage"));

                AEnemy* OwnChar = Cast<AEnemy>(MeshComp->GetOwner());

                UGameplayStatics::ApplyDamage(HitActor, OwnChar->Damage, OwnChar->GetController(), OwnChar, UDamageType::StaticClass());

                UE_LOG(LogTemp, Warning, TEXT("Enemy Apply Damage %f"), OwnChar->Damage);
            }
            else
            {
                return;
            }
        }

        

        // ApplyDamage
        // 여기에서 충돌한 객체와 위치에 대한 처리를 수행할 수 있습니다.
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Not Attack"));
        // 충돌하지 않은 경우 처리
    }

    bool bDrawDebug = false;
    // (디버깅 용) 스피어 트레이스를 시각적으로 표시
    if (bDrawDebug)
    {
        DrawDebugSphere(
            GetWorld(),
            TraceStart,
            TraceRadius,
            32,
            FColor::Red,
            false,
            1.0f,
            0,
            1.0f
        );

        if (bHit)
        {
            DrawDebugPoint(
                GetWorld(),
                HitLocation,
                10.0f,
                FColor::Green,
                false,
                1.0f,
                0
            );
        }
    }
}
