// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"
UENUM(BlueprintType)
enum class EWeaponActionType : uint8 //���� ��� ������ �����Ҽ� �մ� ��� �ൿ���� Ÿ��
{
	None, Attack, Evasion, Guard
};
UCLASS()
class PROJECTSTELL_API AWeapon : public AActor
{
	GENERATED_BODY()
public:
	AWeapon();
protected:
	virtual void BeginPlay() override;
public:
	void InitWeapon();
public:
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		class USkeletalMeshComponent* mesh;

	//ĳ���� ����
protected:
	class APlayerCharacter* character;

	//�ִϸ��̼� ����
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
		class UPlayerCharacterAnim* anim;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage, Meta = (AllowPrivateAccess = true))
		class UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage, Meta = (AllowPrivateAccess = true))
		class UAnimMontage* EvasionMontage;
private:
	UFUNCTION()
		void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
public:
	UAnimMontage* GetAttackMontage();
	UAnimMontage* GetEvasionMontage();

	//�޺� ���� (�߰� ���� �ʿ�)
public:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Combo, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Combo, Meta = (AllowPrivateAccess = true))
		bool CanNextCombo;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Combo, Meta = (AllowPrivateAccess = true))
		bool IsComboInputOn;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Combo, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combo, Meta = (AllowPrivateAccess = true))
		int32 MaxCombo;
	void Setting_ComboAttackStart();
	void Setting_ComboAttackEnd();

	//���� ��ġ ����
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
		float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
		float AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
		float AttackRadius;

	//���⸦ ���� ���� �κ��� �ٸ������� ���� ���� �����
	void Action(EWeaponActionType type = EWeaponActionType::None);
	void NextComboActionCheck();
	void AttackCheck();
};
