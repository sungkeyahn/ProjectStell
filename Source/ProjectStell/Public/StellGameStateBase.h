// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "GameFramework/GameStateBase.h"
#include "StellGameStateBase.generated.h"

UCLASS()
class PROJECTSTELL_API AStellGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	AStellGameStateBase();
public:
	int32 GetGameScore() const;
	void SetGameCleared();
	void AddGameScore();
	bool IsGameCleared() const;
private:
	UPROPERTY(Transient)
		int32 GameScore;
	UPROPERTY(Transient)
		bool isGameClear;
};
