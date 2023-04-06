// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProjectStell.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharaterCtrl.generated.h"

UCLASS()
class PROJECTSTELL_API APlayerCharaterCtrl : public APlayerController
{
	GENERATED_BODY()
public:
	APlayerCharaterCtrl();
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;
protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

//입력 계층 관련
private:
	FInputModeGameOnly GameInputMode;
	FInputModeUIOnly UIInputMode;
public:
	void ChangeInputMode(bool bGameMode = true);

//게임제어 기능 관련
private:
	UPROPERTY()
		class UGamePauseMenuWidget* MenuWidget;
	//클리어시 보여줄 데이터가 확정되지 않아 일단 일시정지 클리스로 구현 
	//사망시 UI도 제작 해야 함
	UPROPERTY()
		class UGameClearMenuWidget* ClearWidget;
	void GamePause();//UI띄어주는 역할
public:
	void GameClear();//UI띄어주는 역할
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Ui)
		TSubclassOf<class UGamePauseMenuWidget>MenuWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Ui)
		TSubclassOf<class UGameClearMenuWidget>ClearWidgetClass;
	
};
