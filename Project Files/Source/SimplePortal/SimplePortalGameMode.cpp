// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimplePortalGameMode.h"
#include "SimplePortalCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASimplePortalGameMode::ASimplePortalGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
