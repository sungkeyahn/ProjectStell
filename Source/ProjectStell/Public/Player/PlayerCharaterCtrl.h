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

//�Է� ���� ����
private:
	FInputModeGameOnly GameInputMode;
	FInputModeUIOnly UIInputMode;
public:
	void ChangeInputMode(bool bGameMode = true);

//�������� ��� ����
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Ui)
		TSubclassOf<class UGamePauseMenuWidget>MenuWidgetClass;
private:
	UPROPERTY()
		class UGamePauseMenuWidget* MenuWidget;
	void GamePause();
};
