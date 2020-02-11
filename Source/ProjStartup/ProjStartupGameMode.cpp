// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ProjStartupGameMode.h"
#include "ProjStartupBall.h"

AProjStartupGameMode::AProjStartupGameMode()
{
	// set default pawn class to our ball
	DefaultPawnClass = AProjStartupBall::StaticClass();
}
