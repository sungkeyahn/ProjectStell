// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnim.generated.h"
//��Ƽ���̵�������Ʈ ���� 
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

//��Ƽ���̵�������Ʈ ���� ���� + Broadcast�� �Լ���
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

//���¿� ������ ������ �Լ�
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;
	//ĳ������ ���¿� ���� �ִϸ��̼��� ���濡 ���õ� ������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsDead;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsMirror;
public:
	void SetMirror(bool m) { IsMirror = m; }
//��Ÿ�� ���� 
private:
	class UAnimMontage* NextMontage;
	float NextMontagePlaySpeed;
public:
	void SetNextAttack(class UAnimMontage* nextMontage, float playSpeed);
	void PlayPlayerMontage(UAnimMontage* montage, float PalySpeed = 1.0f);
	void StopPlayerMontage(UAnimMontage* montage);
};
