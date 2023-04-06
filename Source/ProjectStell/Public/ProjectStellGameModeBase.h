// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ProjectStell.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectStellGameModeBase.generated.h"

UCLASS()
class PROJECTSTELL_API AProjectStellGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
		AProjectStellGameModeBase();
		virtual void PostInitializeComponents() override;
		virtual void PostLogin(APlayerController* NewPlayer) override;
private:
	UPROPERTY()
		class AStellGameStateBase* StellGameState;
	UPROPERTY()
		int32 StageClearScore;
public:
	void AddScore();
	int32 GetCurrentScore() const;
};
