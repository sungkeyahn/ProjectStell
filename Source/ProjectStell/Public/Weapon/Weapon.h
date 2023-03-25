// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"
UENUM(BlueprintType)
enum class EWeaponActionType : uint8 //무기 라는 개념이 실행할수 잇는 모든 행동들의 타입
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

	//캐릭터 관련
protected:
	class APlayerCharacter* character;

	//애니메이션 관련
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

	//콤보 관련 (추가 구현 필요)
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

	//무기 수치 관련
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
		float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
		float AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
		float AttackRadius;

	//무기를 통한 공격 부분은 다른쪽으로 빼는 것을 고려중
	void Action(EWeaponActionType type = EWeaponActionType::None);
	void NextComboActionCheck();
	void AttackCheck();
};
