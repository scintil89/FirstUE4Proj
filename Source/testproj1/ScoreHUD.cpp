// Fill out your copyright notice in the Description page of Project Settings.

#include "ScoreHUD.h"
#include "Runtime/Engine/Classes/Engine/Canvas.h"
#include "Runtime/Engine/Public/CanvasItem.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"

void AScoreHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D DrawPoint(Canvas->ClipX * 0.2f, Canvas->ClipY * 0.8f);
	FText	score = FText::AsNumber(100);
	
	
	FCanvasTextItem textItem(DrawPoint, score, Font, FLinearColor::White);
	textItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(textItem);
}

