// Fill out your copyright notice in the Description page of Project Settings.


#include "StellGameStateBase.h"

AStellGameStateBase::AStellGameStateBase()
{
	GameScore = 0;
	isGameClear = false;
}
int32 AStellGameStateBase::GetGameScore() const
{
	return GameScore;
}
void AStellGameStateBase::SetGameCleared()
{
	isGameClear = true;
}
void AStellGameStateBase::AddGameScore()
{
	GameScore++;
}
bool AStellGameStateBase::IsGameCleared() const
{
	return isGameClear;
}
