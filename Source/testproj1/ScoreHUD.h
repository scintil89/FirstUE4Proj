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
	// 화면 왼쪽 상단에 그려질 것입니다.
	//UPROPERTY(EditDefaultsOnly)
	//UTexture2D* CrosshairTexture;

	//사용될 폰트입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD Font")
	UFont* Font;
	
public:
	// HUD 에 대한 주요 드로 콜입니다.
	virtual void DrawHUD() override;
	
};
