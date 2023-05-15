// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverMenuWidget.generated.h"

UCLASS()
class PROJECTSTELL_API UGameOverMenuWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	//이 위젯의 구성요소들 
	UPROPERTY()
		class UButton* ReturnToTitleButton; //타이틀로 가기버튼
	UPROPERTY()
		class UButton* ReStartLevelButton; //타이틀로 가기버튼
	UPROPERTY()
		class UButton* ExitGameButton;//게임 나가기 버튼
	UFUNCTION()
		void OnReturnToTitleClicked();
	UFUNCTION()
		void OnReStartLevelClicked();
	UFUNCTION()
		void OnExitGameClicked();
};
