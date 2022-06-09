// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TestLagCompGameMode.generated.h"

UCLASS(minimalapi)
class ATestLagCompGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATestLagCompGameMode();
	virtual void Tick(float DeltaSeconds) override;
};



