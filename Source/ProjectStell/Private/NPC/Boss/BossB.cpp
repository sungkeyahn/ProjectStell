// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/Boss/BossB.h"
#include "NPC/Boss/BossBCtrl.h"
#include "NPC/EnemyAnim.h"

ABossB::ABossB()
{}
void ABossB::BeginPlay()
{
	Super::BeginPlay();
	SenseRange = 800.f;
	GetWorldTimerManager().SetTimer(SkillCoolTimerHandle, this, &ABossB::SkillCoolTimer, 1.0f, true);
}
void ABossB::SkillCoolTimer()
{
	CurCoolTime++;
	if (CurCoolTime > SkillCoolTime)
	{
		CurCoolTime = 0.f;
		Cast<ABossBCtrl>(GetController())->SkillActivate(true);
	}
}
void ABossB::Skill_JumpAttack()
{
	anim->OnMontageEnded.AddDynamic(this, &ABossB::OnSkillEndFun);
	anim->PlayEnemyMontage(SkillAttackInfo.montage);
	Attack(SkillAttackInfo);
}
void ABossB::OnSkillEndFun(UAnimMontage* Montage, bool bInterrupted)
{
	OnSkillEnd.Broadcast();
}
void ABossB::Skill()
{
	Skill_JumpAttack();
}
