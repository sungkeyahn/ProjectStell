// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnim.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDeletegate);
DECLARE_MULTICAST_DELEGATE(FOnHitEndCheckDeletegate);
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
	FOnHitEndCheckDeletegate OnHitEndCheck;
private:
	UFUNCTION()
		void AnimNotify_AttackHitCheck();
	UFUNCTION()
		void AnimNotify_HitEndCheck();

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
