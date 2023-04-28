// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProjectStell.h"
#include "Blueprint/UserWidget.h"

#include "Object/Item.h"

#include "InventoryWidget.generated.h"

UCLASS()
class PROJECTSTELL_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
//措面 UI狼 备己 夸家甸
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
		TArray<class UItemSlotWidget*> ItemSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
		class UItemSlotWidget* LeftWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
		class UItemSlotWidget* RightWeapon;
public:
	void BindCharacterInventory(class APlayerCharacter* p);
	void UpdateItemSlot(FItemInfoStruct info);
};
