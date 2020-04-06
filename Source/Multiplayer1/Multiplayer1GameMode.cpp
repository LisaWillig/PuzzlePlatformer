// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Multiplayer1GameMode.h"
#include "Multiplayer1Character.h"
#include "UObject/ConstructorHelpers.h"

AMultiplayer1GameMode::AMultiplayer1GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
