// Copyright Epic Games, Inc. All Rights Reserved.

#include "PhysicsTest2GameMode.h"
#include "PhysicsTest2Character.h"
#include "UObject/ConstructorHelpers.h"

APhysicsTest2GameMode::APhysicsTest2GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
