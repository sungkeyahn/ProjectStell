// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Item.h"

AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));
	movement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Movement"));
	box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = box;
	mesh->SetupAttachment(RootComponent);
}
void AItem::BeginPlay()
{
	Super::BeginPlay();
	movement->RotationRate = FRotator(0.0f, 60.f, 0.0f);
}
FItemInfoStruct AItem::GetItemInfo()
{
	return info;
}
void AItem::Acquiring_Item() //아이템을 접촉 한 상태에서 아이템을 습득 했을때 호출될 함수 
{
	CurSpawnTime = reSpawnTime;
	GetWorldTimerManager().SetTimer(ItemReSpawnTimmerHandle, this, &AItem::ReSpawnItemTimmer, 1.0f, true);
	mesh->SetVisibility(false);
	//SetActorHiddenInGame(true);
	box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
void AItem::ReSpawnItemTimmer()
{
	CurSpawnTime--;
	if (CurSpawnTime <=0.f)
	{
		CurSpawnTime = reSpawnTime;
		mesh->SetVisibility(true);
		//SetActorHiddenInGame(false);
		box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		GetWorldTimerManager().ClearTimer(ItemReSpawnTimmerHandle);
	}
}

