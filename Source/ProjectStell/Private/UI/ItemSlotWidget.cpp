// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemSlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//Image = Cast<UImage>(GetWidgetFromName(TEXT("ItemImage")));
	//quantity = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemQuantity")));
}
void UItemSlotWidget::NativeOnInitialized()
{
	Super::NativeConstruct();
	Image = Cast<UImage>(GetWidgetFromName(TEXT("ItemImage")));
	quantity = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemQuantity")));
}
void UItemSlotWidget::SetSlot(int32 ID, int32 Quantity, UTexture2D* Icon)
{
	ItemID = ID;
	quantity->SetText(FText::FromString(FString::FromInt(Quantity)));
	Image->SetOpacity(1.0f);
	Image->SetBrushFromTexture(Icon);
}
