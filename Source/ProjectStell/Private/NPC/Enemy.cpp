// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/Enemy.h"
#include "NPC/EnemyAnim.h"
#include "NPC/EnemyCtrl.h"
#include "Stat/Stat.h"
#include "ProjectStellGameModeBase.h"
#include"DrawDebugHelpers.h"

AEnemy::AEnemy()
{
	Stat = CreateDefaultSubobject<UStat>(TEXT("Stat"));

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("AnimBlueprint'/Game/1_Animation/ABP/EnemyABP.EnemyABP_C'"));
	if (AnimBP.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimBP.Class);

	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);
	GetCapsuleComponent()->SetCapsuleRadius(34.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy")); //¾ÆÁ÷ ¾È¸¸µë
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;

	AIControllerClass = AEnemyCtrl::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	//ÃÊ±â »óÅÂ-> "¼û±è »óÅÂ" 
	SetInGameState(EEnemyStateInGame::Init);
}
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	EnemyCtrl = Cast<AEnemyCtrl>(GetController());
	if (EnemyCtrl == nullptr)return;

	SetInGameState(EEnemyStateInGame::Ready);
}
void AEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());
	if (nullptr == anim) return;
	SetInGameState(EEnemyStateInGame::Loading);
}
float AEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	Stat->SetDamage(FinalDamage);
	if (CurrentInGameState == EEnemyStateInGame::Dead)
	{
		if (EventInstigator->IsPlayerController())
		{}
	}

	return FinalDamage;
}
void AEnemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (IsAttacking)return;
	IsAttacking = false;
}
EEnemyStateInGame AEnemy::GetInGameState() const
{
	return CurrentInGameState;
}
void AEnemy::SetInGameState(EEnemyStateInGame newState)
{
	if (CurrentInGameState == newState)return;
	CurrentInGameState = newState;
	switch (CurrentInGameState)
	{
		case EEnemyStateInGame::Init:
		{
			SetActorHiddenInGame(true);
			SetCanBeDamaged(false);
			break;
		}
		case EEnemyStateInGame::Loading:
		{
			SetActorHiddenInGame(true);
			SetCanBeDamaged(false);
			anim->OnMontageEnded.AddDynamic(this, &AEnemy::OnAttackMontageEnded);
			anim->OnAttackHitCheck.AddUObject(this, &AEnemy::AttackCheck);
			Stat->OnHpChanged.AddLambda([this]()->void {anim->PlayEnemyMontage(HitMontage); });
			Stat->OnHpIsZero.AddLambda([this]()->void {SetInGameState(EEnemyStateInGame::Dead); });
			break;
		}
		case EEnemyStateInGame::Ready:
		{
			SetActorHiddenInGame(false);
			SetCanBeDamaged(true);
	
			EnemyCtrl->RunBT();
			break;
		}
		case EEnemyStateInGame::Dead:
		{
			SetActorEnableCollision(false);
			SetCanBeDamaged(false);
			GetMesh()->SetHiddenInGame(false);
			anim->SetDeadAnim();
			EnemyCtrl->StopBT();
			GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]()->void
			{ 	
				++DeadTime;
				if (DeadTime > 3.f)
				{
					GetWorldTimerManager().ClearTimer(DeadTimerHandle);
					SetActorHiddenInGame(true);
					Destroy();
					auto gm = Cast<AProjectStellGameModeBase>(GetWorld()->GetAuthGameMode());
					if (nullptr != gm) gm->AddScore();
				}
			}), 1.0f, true);

			break;
		}
	}

}
void AEnemy::Attack()
{
	if (!IsAttacking)
	{
		anim->PlayEnemyMontage(AttackMontage);
		IsAttacking = true;
	}
}
void AEnemy::AttackCheck()
{
	FHitResult hitResult;
	FCollisionQueryParams params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel
	(
		hitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(AttackRadius),
		params
	);
#ifdef ENABLE_DRAW_DEBUG
	FVector traceVec = GetActorForwardVector() * AttackRange;
	FVector center = GetActorLocation() + traceVec * 0.5f;
	float halfheight = AttackRange * 0.5f + AttackRadius;
	FQuat capsuleRot = FRotationMatrix::MakeFromZ(traceVec).ToQuat();
	FColor drawColor = bResult ? FColor::Green : FColor::Red;
	float debugLifeTime = 5.0f;
	DrawDebugCapsule(GetWorld(), center, halfheight, AttackRadius, capsuleRot, drawColor, false, debugLifeTime);
#endif  
	if (bResult)
	{
		if (hitResult.GetActor())
		{
			FDamageEvent damageEvent;
			//hitResult.Actor->TakeDamage(CharacterStat->GetAttack(), damageEvent, GetController(), this);
			//CPPEX_LOG(Warning,TEXT("Hit Actor Name: %s"),*hitResult.Actor->GetName());
		}
	}
}

