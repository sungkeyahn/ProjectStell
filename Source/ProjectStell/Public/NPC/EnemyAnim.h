// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnim.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDeletegate);
UCLASS()
class PROJECTSTELL_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()
//초기화 및 기본 제공 함수들
public:
	UEnemyAnim();
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;
	void PlayEnemyMontage(class UAnimMontage* montage);

//공격 관련
public:
	FOnAttackHitCheckDeletegate OnAttackHitCheck;
private:
	UFUNCTION()
		void AnimNotify_AttackHitCheck();

//상태 관련
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed=0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsDead=false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsSleep=true;
};
