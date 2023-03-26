#include "Player/PlayerCharacterState.h"
#include "StellSaveGame.h"
#include "Stell.h"

APlayerCharacterState::APlayerCharacterState()
{
	SaveSlotName=TEXT("Player1");
}
void APlayerCharacterState::InitPlayerData()
{
	auto SaveGame = Cast<UStellSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (nullptr == SaveGame)
		SaveGame = GetMutableDefault<UStellSaveGame>();
	
	SetCharacterLevel(SaveGame->Level);
	SavePlayerData();
}
void APlayerCharacterState::SavePlayerData()
{
	UStellSaveGame* NewPlayerData = NewObject<UStellSaveGame>();
	NewPlayerData->Level = CharacterLevel;
	NewPlayerData->Exp = Exp;
	NewPlayerData->PlayTime = PlayTime;
	NewPlayerData->DeadCount = DeadCount;
	if (!UGameplayStatics::SaveGameToSlot(NewPlayerData, SaveSlotName, 0))
	{}
}
void APlayerCharacterState::SetCharacterLevel(int32 NewCharacterLevel)
{
	auto gameinstance = Cast<UStell>(GetGameInstance());
	if(nullptr == gameinstance)return;

	CurrentStatData1 = gameinstance->GetDataTableRow(NewCharacterLevel);
	CurrentStatData2 = gameinstance->GetPlayerStatDataTableRow(NewCharacterLevel);
	if (nullptr == CurrentStatData1)return;
	if (nullptr == CurrentStatData2)return;

	CharacterLevel = NewCharacterLevel;
}
int32 APlayerCharacterState::GetCharacterLevel() const
{
	return CharacterLevel;
}
