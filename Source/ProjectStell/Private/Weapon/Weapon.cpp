// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"
#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharacterAnim.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = mesh;
}
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}
