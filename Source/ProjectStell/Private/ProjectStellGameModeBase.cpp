// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProjectStellGameModeBase.h"
#include "Player/PlayerCharaterCtrl.h"
#include "Player/PlayerCharacterState.h"

AProjectStellGameModeBase::AProjectStellGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Blueprint'/Game/1_BP/Player/Player1.Player1_C'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	PlayerControllerClass = APlayerCharaterCtrl::StaticClass();
	PlayerStateClass = APlayerCharacterState::StaticClass();
	//GameStateClass = AABGameState::StaticClass();
}

void AProjectStellGameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//ABGameState = Cast<AABGameState>(GameState);
}

void AProjectStellGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	auto PlayerState = Cast<APlayerCharacterState>(NewPlayer->PlayerState);
	if(nullptr == PlayerState)return;
	PlayerState->InitPlayerData();
}
