// Fill out your copyright notice in the Description page of Project Settings.

//TODO : ���ϸ� MyHUD�� �ٲٱ�
//		���� �����ϴٰ� ������Ʈ �̻������� �ٲٴ� ��� ������ ��������

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Aim.generated.h"

UCLASS()
class TESTPROJ1_API AAim : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	class UTexture2D* CrosshairTexture;
	
	UPROPERTY(EditDefaultsOnly)
	class UFont* font;

	UPROPERTY(EditDefaultsOnly)
	class UTexture2D* whereTarget;

	class AGameManager* gm;
	class APawn*		pawn;

public:
	virtual void DrawHUD() override;
};
