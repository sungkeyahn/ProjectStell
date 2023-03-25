// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnim.generated.h"
//노티파이델리게이트 정의 
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

//노티파이델리게이트 관련 변수 + Broadcast용 함수들
public:
	FOnNextAttackComboCheckDeletegate  OnNextAttackComboCheck;
	FOnAttackHitCheckDeletegate OnAttackHitCheck;
	FOnMontageFeasibleCheckDeletegate OnMontageFeasibleCheck;
private:
	UFUNCTION(BlueprintCallable)
		void AnimNotify_ComboCheck();
	UFUNCTION(BlueprintCallable)
		void AnimNotify_HitCheck();

//상태에 관련한 변수와 함수
private:
	//애니메이션에 영향을 줄 수 있는 캐릭터의 물리적 수치(스텟아님)에 해당하는 변수들
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;
	//캐릭터의 상태에 따른 애니메이션의 변경에 관련된 변수들
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsDead;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsMirror;
public:
	void SetMirror(bool m) { IsMirror = m; }
//몽타주 관련 변수와 함수 (주의사항) 몽타주 를만들때 섹션이름을 꼭 "Section숫자"형식으로 만들것
public:
	//특정 몽타주를 재생
	void PlayPlayerMontage(UAnimMontage* montage, float PalySpeed = 1.0f);
	//특정 몽타주의 다음 섹션을 재생 
	void NextSectionPlay_PlayerMontage(UAnimMontage* montage, int32 nextSectionNum);
	//특정 몽타주가 재생할 수 있는 조건을 만족한 상태인지를 알아봐 주는 함수
};
