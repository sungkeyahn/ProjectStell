// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/SkeletonA/SkeletonA.h"
#include "NPC/SkeletonA/SkeletonACtrl.h"
#include "NPC/EnemyAnim.h"
#include "Stat/Stat.h"

#include "DrawDebugHelpers.h"
#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharaterCtrl.h"
#include "Player/ComboManager.h"
ASkeletonA::ASkeletonA()
{}
float ASkeletonA::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage =0.f;
	if (!isHit)
	{
		isHit = true;
		//공격 정보
		takeAttackInfo = Cast<APlayerCharacter>(DamageCauser)->Combo->GetCurAttackInfo();
		//흔들림
		if (takeAttackInfo.CameraShakeType != nullptr)
			Cast<APlayerCharaterCtrl>(EventInstigator)->PlayerCameraManager.Get()->StartCameraShake(takeAttackInfo.CameraShakeType, 1.0f, ECameraShakePlaySpace::CameraLocal, FRotator(0, 0, 0));
		//데미지 적용
		Stat->SetDamage(takeAttackInfo.Damage);
		//가드 and 반격
		if (!isGuard)
			Cast<ASkeletonACtrl>(GetController())->Guard();
		else if (isGuard)
		{
			GuardTime = 3.0f;
			isGuard = false;
			OnGuardEnd.Broadcast();
			SetMonsterState(EMonsterState::Idle);
			GetWorldTimerManager().ClearTimer(GuardTimerHandle);
			Attack(GuardAttackInfo);
		}
	}
	return FinalDamage;
}
void ASkeletonA::WakeUp() 
{
	anim->IsSleep = false;
}
void ASkeletonA::Guard()
{
	SetMonsterState(EMonsterState::Invincibility);
	isGuard = true;
	anim->PlayEnemyMontage(GuardMontage);
	GetWorld()->GetTimerManager().SetTimer(GuardTimerHandle, FTimerDelegate::CreateLambda([this]()->void
	{
		GuardTime -= 1.0f;
		if (GuardTime <= 0.f)
		{
			GuardTime = 3.0f;
			isGuard = false;
			OnGuardEnd.Broadcast();
			SetMonsterState(EMonsterState::Idle);
			GetWorldTimerManager().ClearTimer(GuardTimerHandle);
		}
	}), 1.0f, true);
	
}


