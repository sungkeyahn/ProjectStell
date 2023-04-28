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
	FInputModeGameAndUI GameAndUIInputMode;
public:
	void ChangeInputMode(int32 bGameMode = 0);

//인 게임UI 관련
private:
	UPROPERTY()
		class UGamePauseMenuWidget* MenuWidget;
	UPROPERTY()
		class UGameClearMenuWidget* ClearWidget;
	UPROPERTY()
		class UCharacterHUDWidget* HUDWidget;
	UPROPERTY()
		class UInventoryWidget* InventoryWidget;
	UPROPERTY()
		class UItemSlotWidget* ItemSlotWidget;
	bool isInvenopen=false;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Ui)
		TSubclassOf<class UGamePauseMenuWidget>MenuWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Ui)
		TSubclassOf<class UGameClearMenuWidget>ClearWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Ui)
		TSubclassOf<class UInventoryWidget>InventoryWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Ui)
		TSubclassOf<class UItemSlotWidget> ItemSlotWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Ui)
		TSubclassOf<class UCharacterHUDWidget>HUDWidgetClass;
public:
	void GamePause();//UI띄어주는 역할
	void GameClear();
	void ShowInventoryUI();
	class UCharacterHUDWidget* GetHUDWidget() const;
	class UInventoryWidget* GetInventoryWidget() const;
	class UItemSlotWidget* GetItemSlotWidget() const;
};
