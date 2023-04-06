// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "GameFramework/PlayerState.h"
#include "PlayerCharacterState.generated.h"

UCLASS()
class PROJECTSTELL_API APlayerCharacterState : public APlayerState
{
	GENERATED_BODY()
public:
	APlayerCharacterState();
private:
	void LoadPlayerData(class UStellSaveGame* save);
public:
	FString SaveSlotName;
	void InitPlayerData();
	void SavePlayerData();
protected:
	UPROPERTY(Transient)
		float PlayTime=0;
	UPROPERTY(Transient)
		int32 DeadCount=0;
public:
	void AddPlayTime();//게임시간 기록하는부분찿기 게임모드에서 
	void AddDeadCount();
	int32 GetDeadCount();


};



