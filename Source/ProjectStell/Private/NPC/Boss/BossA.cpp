// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/Boss/BossA.h"
#include "NPC/Boss/BossACtrl.h"
#include "NPC/EnemyAnim.h"
#include "Stat/Stat.h"

ABossA::ABossA()
{}
void ABossA::BeginPlay()
{
	Super::BeginPlay();
	SenseRange = 800.f;
	GetWorldTimerManager().SetTimer(SkillCoolTimerHandle, this, &ABossA::SkillCoolTimer, 1.0f, true);
	//GetWorldTimerManager().ClearTimer(SkillCoolTimerHandle);
}
void ABossA::SkillCoolTimer()
{
	CurCoolTime++;
	if (CurCoolTime > SkillCoolTime)
	{
		CurCoolTime = 0.f;
		Cast<ABossACtrl>(GetController())->SummonAble(true);
	}
}
void ABossA::SpawnEnemy()
{
	auto spawnEnemy = GetWorld()->SpawnActor<AEnemy>(SpawnMonsters, GetActorLocation(), FRotator::ZeroRotator);
}
void ABossA::SummonMonster()
{
	anim->PlayEnemyMontage(SkillMontage);
	//Cast<ABossACtrl>(GetController())->StopBT();
	const FRotator YawRotation(0, GetActorRotation().Yaw, 0);
	const FVector Direction = -1*FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	LaunchCharacter(Direction * 2000.f, true, true);
	
	SpawnEnemy();
}
