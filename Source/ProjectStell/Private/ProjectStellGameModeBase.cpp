// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProjectStellGameModeBase.h"
#include "Player/PlayerCharaterCtrl.h"


AProjectStellGameModeBase::AProjectStellGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Blueprint'/Game/1_BP/Player/Player1.Player1_C'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	PlayerControllerClass = APlayerCharaterCtrl::StaticClass();
	//PlayerStateClass = AABPlayerState::StaticClass();
	//GameStateClass = AABGameState::StaticClass();
	/*
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerCtrlBPClass(TEXT("Blueprint'/Game/1_BP/BP_PlayerCtrl.BP_PlayerCtrl_C'"));
	if (PlayerCtrlBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerCtrlBPClass.Class;
	}*/
}
