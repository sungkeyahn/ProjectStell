// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "GameFramework/PlayerState.h"
#include "PlayerCharacterState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangedDelegate);
UCLASS()
class PROJECTSTELL_API APlayerCharacterState : public APlayerState
{
	GENERATED_BODY()

public:
	APlayerCharacterState();
	FString SaveSlotName;
	void InitPlayerData();
	void SavePlayerData();

	FOnPlayerStateChangedDelegate OnPlayerStateChanged;

private:
	struct FBaseStatData* CurrentStatData1;
	struct FPlayerStatDataByLevel* CurrentStatData2;
	void SetCharacterLevel(int32 NewCharacterLevel);
public:
	int32 GetCharacterLevel() const;
public:
	UPROPERTY(Transient)
		int32 CharacterLevel;
	UPROPERTY(Transient)
		int32 Exp;	
	UPROPERTY(Transient)
		int32 PlayTime;
	UPROPERTY(Transient)
		int32 DeadCount;
};
