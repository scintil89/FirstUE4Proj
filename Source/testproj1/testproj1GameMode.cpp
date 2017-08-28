// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "testproj1GameMode.h"
#include "testproj1Pawn.h"

Atestproj1GameMode::Atestproj1GameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = Atestproj1Pawn::StaticClass();
}
