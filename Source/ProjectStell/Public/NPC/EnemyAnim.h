// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnim.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDeletegate);
DECLARE_DELEGATE_RetVal_OneParam(bool, FOnMontageFeasibleCheckDeletegate, UAnimMontage* montage);
UCLASS()
class PROJECTSTELL_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()
		//�ʱ�ȭ 
public:
	UEnemyAnim();
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;

	//�ִϸ��̼� ���� ��� -> ���� ����
	void PlayEnemyMontage(class UAnimMontage* montage);

	//���� �ǰ� ����
public:
	FOnAttackHitCheckDeletegate OnAttackHitCheck;
private:
	UFUNCTION()
		void AnimNotify_AttackHitCheck();

	//���� ����
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsInAir;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsDead;

public:
	void SetDeadAnim() { IsDead = true; }
};
