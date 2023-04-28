// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotWidget.generated.h"

UCLASS()
class PROJECTSTELL_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized()override;
private:
	UPROPERTY()
		int32 ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
		class UImage* Image;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
		class UTextBlock* quantity;
public:
	void SetSlot(int32 ID, int32 Quantity, UTexture2D* Icon);
};
