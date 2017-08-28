// Fill out your copyright notice in the Description page of Project Settings.

#include "GameManager.h"
#include "Target.h"


// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	
	isGameRunning = true;
	
	//FTimerDelegate Funcer;
	//Funcer.BindLambda([gametime]() { gametime++; });

	gametime = 180;

	GetWorldTimerManager().SetTimer(timer, this, &AGameManager::GameTimer, 1.0f, true);

	// ...
	//numTargetArray = 10;
	//idx = 0;

	//TODO : ObjectPool Set Up
	/*
	for (int32 i = 0; i < numTargetArray; i++)
	{
	ATarget* newTarget = new ATarget();

	//Hide Code
	newTarget->SetActorHiddenInGame(true);
	newTarget->SetActorEnableCollision(false);
	newTarget->SetActorTickEnabled(false);
	//Components Enable
	TInlineComponentArray<UActorComponent*> Components;
	newTarget->GetComponents(Components);

	for (int32 CompIdx = 0; CompIdx<Components.Num(); CompIdx++)
	{
	Components[CompIdx]->SetComponentTickEnabled(false);
	}

	targetArray.Add(newTarget);
	}
	*/
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (nowTarget)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *nowTarget->GetName());
	}


	if (!nowTarget)// TODO : 왜 안될까...
	{
		UE_LOG(LogTemp, Warning, TEXT("GM::Tick  if (nowTarget)"));
		SpawnTarget();
	}

	if (!isGameRunning)
	{
		APlayerController* const player = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
		if (player != nullptr)
		{
			player->SetPause(true);
		}
	}

}

void AGameManager::GameTimer()
{
	//UE_LOG(LogTemp, Warning, TEXT("timer"));//FText::AsNumber(score));

	gametime--;

	if (gametime < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Over"));//FText::AsNumber(score));

		GetWorldTimerManager().ClearTimer(timer);

		//game over
		isGameRunning = false;
	}
}


bool AGameManager::GetisGameRunning()
{
	return isGameRunning;
}

void AGameManager::ScoreUp(int sc)
{
	UE_LOG(LogTemp, Warning, TEXT("score up"));//FText::AsNumber(score));

	score += sc;
}

int32 AGameManager::GetScore()
{
	return score;
}

int32 AGameManager::GetGameTime()
{
	return gametime;
}

void AGameManager::SpawnTarget()
{
	UE_LOG(LogTemp, Warning, TEXT("Spawning Target"));

	auto loc = FVector(FMath::RandRange(0.f, 7000.f),
		FMath::RandRange(-12000.f, 12000.f),
		FMath::RandRange(2000.f, 10000.f));

	UWorld* world = GetWorld();

	if (world)
	{
		auto spawn = world->SpawnActor<ATarget>(TargetClass, loc, FRotator::ZeroRotator);

		nowTarget = spawn;
	}
}

ATarget * AGameManager::GetNowTarget()
{
	return nowTarget;
}

FVector AGameManager::GetNowTargetLocation()
{
	return nowTarget->GetActorLocation();
}
