// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "GameFramework/SaveGame.h"
#include "StellSaveGame.generated.h"

UCLASS()
class PROJECTSTELL_API UStellSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UStellSaveGame();

	UPROPERTY()
		int32 Level;
	UPROPERTY()
		int32 Exp;
	UPROPERTY()
		int32 PlayTime;
	UPROPERTY()
		int32 DeadCount;

};
