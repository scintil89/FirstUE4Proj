// Fill out your copyright notice in the Description page of Project Settings.

#include "Aim.h"
#include "testproj1.h"
#include "Runtime/Engine/Classes/Engine/Canvas.h"
#include "Runtime/Engine/Public/CanvasItem.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "GameManager.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void AAim::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		//AStaticMeshActor *A = *ActorItr;

		UE_LOG(LogTemp, Warning, TEXT("%s"), *(ActorItr->GetName()));

		if (ActorItr->GetName().Compare("GameManager_1") == 0)
		{
			gm = Cast<AGameManager>(*ActorItr);
			break;
		}
	}

	pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void AAim::DrawHUD()
{
	Super::DrawHUD();

	if (gm)
	{
		if (gm->GetisGameRunning())
		{
			//Aim
			if (CrosshairTexture)
			{
				// 캔버스 중심을 찾습니다.
				FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

				// 텍스처 중심이 캔버스 중심에 맞도록 텍스처의 크기 절반 만큼 오프셋을 줍니다.
				FVector2D CrossHairDrawPosition(Center.X - (CrosshairTexture->GetSurfaceWidth() * 0.5f),
					Center.Y - (CrosshairTexture->GetSurfaceHeight() * 0.5f));

				// 중심점에 조준선을 그립니다.
				FCanvasTileItem TileItem(CrossHairDrawPosition, CrosshairTexture->Resource, FLinearColor::White);
				TileItem.BlendMode = SE_BLEND_Translucent;
				Canvas->DrawItem(TileItem);
			}


			//Score
			//if(gm)
			FVector2D DrawScorePoint(Canvas->ClipX * 0.1f, Canvas->ClipY * 0.1f);
			FText	scoreTxt = FText::FromString((FString("Score : ") + FString::FromInt(gm->GetScore())));

			FCanvasTextItem textItem(DrawScorePoint, scoreTxt, font, FLinearColor::Green);
			textItem.BlendMode = SE_BLEND_Translucent;
			textItem.Scale.Set(2, 2);
			Canvas->DrawItem(textItem);

			//Time
			FVector2D DrawTimePoint(Canvas->ClipX * 0.8f, Canvas->ClipY * 0.1f);
			FText	timeTxt = FText::FromString((FString("Time : ") + FString::FromInt(gm->GetGameTime())));

			FCanvasTextItem timeItem(DrawTimePoint, timeTxt, font, FLinearColor::Blue);
			timeItem.BlendMode = SE_BLEND_Translucent;
			timeItem.Scale.Set(2, 2);
			Canvas->DrawItem(timeItem);

			//targetHelper
			if (whereTarget && pawn)
			{
				auto targetLocation = gm->GetNowTargetLocation();
				auto pawnLocation = pawn->GetActorLocation();

				auto p2tVector = (targetLocation - pawnLocation).GetSafeNormal();

				auto pawnLook = pawn->GetActorForwardVector();

				//UE_LOG(LogTemp, Warning, TEXT("p2tVector is %s"), *p2tVector.ToString());

				auto dot = FVector::DotProduct(p2tVector, pawnLook);

				//FOV : 90
				if (dot <= 0)//FMath::Cos(FMath::DegreesToRadians(45))
				{
					//UE_LOG(LogTemp, Warning, TEXT("Dot %f"), Canvas->ClipX);

					//auto X = ((p2tVector.Y + 1.0f)/2.0f) * (Canvas->ClipX / 2);
					auto X = (Canvas->ClipX / 2) + (p2tVector.Y * 300) - (CrosshairTexture->GetSurfaceWidth() * 0.5f);

					//auto Y = ((p2tVector.Z + 1.0f)/2.0f) * (Canvas->ClipY / 2);
					auto Y = (Canvas->ClipY / 2) - (p2tVector.Z * 300) - (CrosshairTexture->GetSurfaceHeight() * 0.5f);


					FVector2D whereTarget2Canvas = FVector2D(X, Y);

					//UE_LOG(LogTemp, Warning, TEXT("Dot %s"), *whereTarget2Canvas.ToString());

					FCanvasTileItem TargetPosItem(whereTarget2Canvas, whereTarget->Resource, FLinearColor::White);
					TargetPosItem.BlendMode = SE_BLEND_Translucent;
					Canvas->DrawItem(TargetPosItem);
				}
			}
		}
		else
		{
			FVector2D DrawScorePoint(Canvas->ClipX * 0.4f, Canvas->ClipY * 0.5f);
			FText	scoreTxt = FText::FromString((FString("Your Score is : ") + FString::FromInt(gm->GetScore())));
			
			FCanvasTextItem textItem(DrawScorePoint, scoreTxt, font, FLinearColor::Red);
			textItem.BlendMode = SE_BLEND_Translucent;
			textItem.Scale.Set(2, 2);
			Canvas->DrawItem(textItem);
		}
	}
}
