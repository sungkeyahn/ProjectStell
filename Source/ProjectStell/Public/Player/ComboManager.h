// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "Components/ActorComponent.h"
#include "Weapon/Weapon.h"
#include "ComboManager.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTSTELL_API UComboManager : public UActorComponent
{
	GENERATED_BODY()
public:	
	UComboManager();
	void InitComboManager();
//캐릭터 관련
private:
	class APlayerCharacter* character;
//공격 관련
private:
	FAttackInfoStruct PreAttackInfo;	
	FAttackInfoStruct CurrentAttackInfo;
	FAttackInfoStruct NextAttackInfo;
public:
	FAttackInfoStruct GetCurAttackInfo();
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Combo, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Combo, Meta = (AllowPrivateAccess = true))
		bool CanNextAttack;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Combo, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo;
//콤보 관련
public:
	void Attack(bool isLeftClick);

	bool InputCheck();
	void ConnectAttack();
	void AttackEnd();

	bool FindAttackInfo(class AWeapon* clickWeapon, class AWeapon* otherWeapon);
	void AttackCheck();
	void AttackReset(); //공격 리셋
};
