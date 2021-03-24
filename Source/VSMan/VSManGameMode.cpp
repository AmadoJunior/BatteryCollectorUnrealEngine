// Copyright Epic Games, Inc. All Rights Reserved.

#include "VSManGameMode.h"
#include "VSManCharacter.h"
#include "UObject/ConstructorHelpers.h"

AVSManGameMode::AVSManGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
