// Fill out your copyright notice in the Description page of Project Settings.

#include "Target.h"
#include "BulletActor.h"
#include "GameManager.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"

// Sets default values
ATarget::ATarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitBoxExtent(FVector(1.0f, 1.0f, 1.0f));
	CollisionComponent->AddLocalRotation(FRotator(0.0f, 0.0f, -.0f), false, nullptr, ETeleportType::None);
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Target"));
	//CollisionComponent->OnComponentHit.AddDynamic(this, &ATarget::OnHit);

	RootComponent = CollisionComponent;

	UE_LOG(LogTemp, Warning, TEXT("Target Spawned"));
}

// Called when the game starts or when spawned
void ATarget::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(2.0f, 2.f, 2.f));

	if (hp == 0)
	{
		//find GM
		//일단 유니티의 GameObject.Find() 처럼 구현
		//TODO : GameManager를 싱글톤 형식으로 구현후 gm을 쉽게 받아올 수 있도록 하자
		for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
			//AStaticMeshActor *A = *ActorItr;
			
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *(ActorItr->GetName()) );
			
			if (ActorItr->GetName().Compare("GameManager_1" ) == 0)
			{
				//UE_LOG(LogTemp, Warning, TEXT("A->IsA"));
				//점수 추가
				auto gm = Cast<AGameManager>(*ActorItr);
				
				gm->ScoreUp(score);
				gm->SpawnTarget();

				break;
			}
		}

		//effect

		//destroy
		UE_LOG(LogTemp, Warning, TEXT("Target Destroy"));
		this->Destroy();
	}
}

void ATarget::NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	//UE_LOG(LogTemp, Warning, TEXT("Target Ntfy Hit"));
}
