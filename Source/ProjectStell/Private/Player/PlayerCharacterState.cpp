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
	SavePlayerData();
	LoadPlayerData(SaveGame);
}
void APlayerCharacterState::SavePlayerData()
{
	UStellSaveGame* NewPlayerData = NewObject<UStellSaveGame>();
	NewPlayerData->PlayTime += PlayTime;
	NewPlayerData->DeadCount += DeadCount;
	if (!UGameplayStatics::SaveGameToSlot(NewPlayerData, SaveSlotName, 0))
		GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString::Printf(TEXT("SaveFail")));
}
void APlayerCharacterState::AddPlayTime()
{
}
void APlayerCharacterState::AddDeadCount()
{
	DeadCount += 1;
	SavePlayerData();

}
int32 APlayerCharacterState::GetDeadCount()
{
	return DeadCount;
}
void APlayerCharacterState::LoadPlayerData(UStellSaveGame* save)
{
	//게임인스턴스 불러와서 캐릭터의 데이터를 동기화 시켜주는 함수
	auto gameinstance = Cast<UStell>(GetGameInstance());
	if(nullptr == gameinstance) return;
	//로드할 데이터가 생기면 작성
	PlayTime = save->PlayTime;
	DeadCount = save->DeadCount;
}
