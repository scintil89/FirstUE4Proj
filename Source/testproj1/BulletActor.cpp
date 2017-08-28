// Fill out your copyright notice in the Description page of Project Settings.

#include "BulletActor.h"
#include "Target.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"

// Sets default values
ABulletActor::ABulletActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitCapsuleSize(2.0f, 2.0f);
	CollisionComponent->AddLocalRotation(FRotator(0.0f, 0.0f, -90.0f), false, nullptr, ETeleportType::None);
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Bullet"));
	CollisionComponent->OnComponentHit.AddDynamic(this, &ABulletActor::OnHit);

	RootComponent = CollisionComponent;

	// ProjectileMovementComponent 를 사용하여 이 발사체의 운동을 관장합니다.
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 12000.0f;
	ProjectileMovementComponent->MaxSpeed = 12000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;	

	InitialLifeSpan = 5.0f;
}

// Called when the game starts or when spawned
void ABulletActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABulletActor::FireInDirection(const FVector & ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void ABulletActor::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	/* Ground9 Obj 충돌시 반응 체크
	if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
	}
	*/

	if ( OtherActor->IsA(ATarget::StaticClass()) )
	{
		//Log
		UE_LOG(LogTemp, Warning, TEXT("Collision"));

		//effect	

		//Damage Process
		Cast<ATarget>(OtherActor)->DamageProcess(damage);
	}

	//충돌이 일어나면 소멸
	this->Destroy();
}

