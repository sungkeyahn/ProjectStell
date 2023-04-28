// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "Blueprint/UserWidget.h"
#include "CharacterHUDWidget.generated.h"

UCLASS()
class PROJECTSTELL_API UCharacterHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void BindCharacterStat(class UPlayerStat* Stat);
protected:
	virtual void NativeConstruct() override;
	void UpdateCharacterStat();
private:
	TWeakObjectPtr<class UPlayerStat> PlayerStat;
	UPROPERTY()
		class UProgressBar* HPBar;
	UPROPERTY()
		class UProgressBar* SPBar;
};
