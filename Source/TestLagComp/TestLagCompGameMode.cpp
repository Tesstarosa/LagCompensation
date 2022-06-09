// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestLagCompGameMode.h"
#include "TestLagCompHUD.h"
#include "TestLagCompCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "LagCompensationManager.h"

ATestLagCompGameMode::ATestLagCompGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATestLagCompHUD::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
}

void ATestLagCompGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(GIsServer)
	{
		LagCompensationManager::SaveFrame();
	}
}
