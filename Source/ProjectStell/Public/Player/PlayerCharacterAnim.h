// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnim.generated.h"
//노티파이델리게이트 정의 
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDeletegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackEndCheckDeletegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackTimeCheckDeletegate);
UCLASS()
class PROJECTSTELL_API UPlayerCharacterAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPlayerCharacterAnim();
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;

//노티파이델리게이트 관련 변수 + Broadcast용 함수들
public:
	FOnAttackHitCheckDeletegate OnAttackHitCheck;
	FOnAttackEndCheckDeletegate OnAttackEndCheck;
	FOnAttackTimeCheckDeletegate OnAttackTimeCheck;
private:
	UFUNCTION(BlueprintCallable)
		void AnimNotify_HitCheck();
	UFUNCTION(BlueprintCallable)
		void AnimNotify_ConnectCheck();
	UFUNCTION(BlueprintCallable)
		void AnimNotify_EndCheck();

//상태에 관련한 변수와 함수
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;
	//캐릭터의 상태에 따른 애니메이션의 변경에 관련된 변수들
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsDead;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsMirror;
public:
	void SetMirror(bool m) { IsMirror = m; }
//몽타주 관련 
private:
	class UAnimMontage* NextMontage;
	float NextMontagePlaySpeed;
public:
	void SetNextAttack(class UAnimMontage* nextMontage, float playSpeed);
	void PlayPlayerMontage(UAnimMontage* montage, float PalySpeed = 1.0f);
	void StopPlayerMontage(UAnimMontage* montage);
};
