// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ScoreHUD.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJ1_API AScoreHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	// ȭ�� ���� ��ܿ� �׷��� ���Դϴ�.
	//UPROPERTY(EditDefaultsOnly)
	//UTexture2D* CrosshairTexture;

	//���� ��Ʈ�Դϴ�.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD Font")
	UFont* Font;
	
public:
	// HUD �� ���� �ֿ� ��� ���Դϴ�.
	virtual void DrawHUD() override;
	
};
