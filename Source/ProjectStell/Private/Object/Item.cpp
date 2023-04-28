// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Item.h"

AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));
	movement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Movement"));
	RootComponent = mesh;
}
void AItem::BeginPlay()
{
	Super::BeginPlay();
	movement->RotationRate = FRotator(0.0f, 60.f, 0.0f);
}
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FItemInfoStruct AItem::GetItemInfo()
{
	return info;
}

