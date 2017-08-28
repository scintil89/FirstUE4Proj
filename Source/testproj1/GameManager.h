// Fill out your copyright notice in the Description page of Project Settings.
//	TODO : �̱������� �����
//		   GameInstance �����ϱ�

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "GameManager.generated.h" 

UCLASS()
class TESTPROJ1_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, Category = Target)
	TSubclassOf<class ATarget> TargetClass;

private:
	ATarget* nowTarget;

	//TArray<ATarget*> targetArray;
	//int32			numTargetArray = 10;
	//int32			idx;

	bool			isGameRunning;
	int32			score;
	
	FTimerHandle	timer;
	int32			gametime;
	void			GameTimer();

public:
	bool			GetisGameRunning();
	void			ScoreUp(int sc);
	int32			GetScore();

	int32			GetGameTime();

	void			SpawnTarget();
	ATarget*		GetNowTarget();
	FVector			GetNowTargetLocation();


	//TODO : ���� ���� �����
};
