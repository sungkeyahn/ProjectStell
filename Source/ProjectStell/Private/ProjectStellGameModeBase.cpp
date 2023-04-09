// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProjectStellGameModeBase.h"
#include "Player/PlayerCharaterCtrl.h"
#include "StellGameStateBase.h"
#include "Player/PlayerCharacterState.h"
#include "UI/GamePauseMenuWidget.h"

AProjectStellGameModeBase::AProjectStellGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Blueprint'/Game/1_Player/Character/Player.Player_C'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	PlayerControllerClass = APlayerCharaterCtrl::StaticClass();
	PlayerStateClass = APlayerCharacterState::StaticClass();
	GameStateClass = AStellGameStateBase::StaticClass();
	StageClearScore = 1;
}
void AProjectStellGameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	StellGameState = Cast<AStellGameStateBase>(GameState);
}
void AProjectStellGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	auto PlayerState = Cast<APlayerCharacterState>(NewPlayer->PlayerState);
	if(nullptr == PlayerState)return;
	PlayerState->InitPlayerData();
}

void AProjectStellGameModeBase::AddScore()
{
	StellGameState->AddGameScore();
	if (GetCurrentScore() >= StageClearScore)
	{
		StellGameState->SetGameCleared();
		for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
			(*It)->TurnOff();
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			const auto pctrl = Cast<APlayerCharaterCtrl>(It->Get());
			if (nullptr != pctrl) pctrl->GameClear();
		}
	}
}

int32 AProjectStellGameModeBase::GetCurrentScore() const
{
	return StellGameState->GetGameScore();
}


