// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnim.generated.h"
//��Ƽ���̵�������Ʈ ���� 
DECLARE_MULTICAST_DELEGATE(FOnNextAttackComboCheckDeletegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDeletegate);
DECLARE_DELEGATE_RetVal_OneParam(bool, FOnMontageFeasibleCheckDeletegate, UAnimMontage* montage);
UCLASS()
class PROJECTSTELL_API UPlayerCharacterAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPlayerCharacterAnim();
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;

//��Ƽ���̵�������Ʈ ���� ���� + Broadcast�� �Լ���
public:
	FOnNextAttackComboCheckDeletegate  OnNextAttackComboCheck;
	FOnAttackHitCheckDeletegate OnAttackHitCheck;
	FOnMontageFeasibleCheckDeletegate OnMontageFeasibleCheck;
private:
	UFUNCTION(BlueprintCallable)
		void AnimNotify_ComboCheck();
	UFUNCTION(BlueprintCallable)
		void AnimNotify_HitCheck();

//���¿� ������ ������ �Լ�
private:
	//�ִϸ��̼ǿ� ������ �� �� �ִ� ĳ������ ������ ��ġ(���ݾƴ�)�� �ش��ϴ� ������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;
	//ĳ������ ���¿� ���� �ִϸ��̼��� ���濡 ���õ� ������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsDead;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsMirror;
public:
	void SetMirror(bool m) { IsMirror = m; }
//��Ÿ�� ���� ������ �Լ� (���ǻ���) ��Ÿ�� �����鶧 �����̸��� �� "Section����"�������� �����
public:
	//Ư�� ��Ÿ�ָ� ���
	void PlayPlayerMontage(UAnimMontage* montage, float PalySpeed = 1.0f);
	//Ư�� ��Ÿ���� ���� ������ ��� 
	void NextSectionPlay_PlayerMontage(UAnimMontage* montage, int32 nextSectionNum);
	//Ư�� ��Ÿ�ְ� ����� �� �ִ� ������ ������ ���������� �˾ƺ� �ִ� �Լ�
};
