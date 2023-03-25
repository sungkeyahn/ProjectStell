// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "GameFramework/PlayerController.h"
#include "UICtrl.generated.h"

UCLASS()
class PROJECTSTELL_API AUICtrl : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Ui)
		TSubclassOf<class UUserWidget>UiWidgetClass;
	UPROPERTY()
		class UUserWidget* UiWidgetInstance;
	
};
