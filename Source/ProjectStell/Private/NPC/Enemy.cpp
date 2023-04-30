#include "NPC/Enemy.h"
#include "NPC/EnemyAnim.h"
#include "NPC/EnemyCtrl.h"
#include "Stat/Stat.h"
#include "ProjectStellGameModeBase.h"
#include"DrawDebugHelpers.h"
#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharaterCtrl.h"
#include "Player/ComboManager.h"

AEnemy::AEnemy()
{
	Stat = CreateDefaultSubobject<UStat>(TEXT("Stat"));
	/*
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("AnimBlueprint'/Game/1_Enemy/EnemyABP.EnemyABP_C'"));
	if (AnimBP.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimBP.Class);
	*/
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);
	GetCapsuleComponent()->SetCapsuleRadius(34.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy")); //아직 안만듬
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;

	AIControllerClass = AEnemyCtrl::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	//초기 상태-> "숨김 상태" 
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
	anim->OnMontageEnded.AddDynamic(this, &AEnemy::OnAttackMontageEnded);
	anim->OnHitEndCheck.AddUObject(this, &AEnemy::HitAffter);
	anim->OnAttackHitCheck.AddUObject(this, &AEnemy::AttackCheck);
	Stat->OnHpIsZero.AddLambda([this]()->void {SetInGameState(EEnemyStateInGame::Dead); });
	//Stat->OnHpChanged.AddUObject(this, &AEnemy::Hit);
	SetInGameState(EEnemyStateInGame::Loading);
}
float AEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	/*if (Combo->GetCurAttackInfo().HitType == EHitEffectType::Stiff)
	PlayerCtrl->PlayerCameraManager.Get()->StartCameraShake(shake, 1.0f, ECameraShakePlaySpace::CameraLocal, FRotator(0, 0, 0));*/

	float FinalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	//공격정보
	FAttackInfoStruct attackinfo = Cast<APlayerCharacter>(DamageCauser)->Combo->GetCurAttackInfo();
	//흔들림
	if(attackinfo.CameraShakeType != nullptr)
		Cast<APlayerCharaterCtrl>(EventInstigator)->PlayerCameraManager.Get()->StartCameraShake(attackinfo.CameraShakeType, 1.0f, ECameraShakePlaySpace::CameraLocal, FRotator(0, 0, 0));
	SetMonsterState(EMonsterState::SuperArmor);
	HitEffect(attackinfo);

	//삭제될 코드들
	/*
	if (Stat->GetHpRatio() < 0.5f)
		SetMonsterState(EMonsterState::SuperArmor);
	else if (Stat->GetHpRatio() == 0.7f)
		SetMonsterState(EMonsterState::Groggy);
		*/

	return FinalDamage;
}
void AEnemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!IsAttacking)return;
	IsAttacking = false;
	OnAttackEnd.Broadcast();
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
void AEnemy::SetMonsterState(EMonsterState newState)
{
	if (CurrentMonsterState == newState)return;
	CurrentMonsterState = newState;
	switch (CurrentMonsterState) //각 상태로 진입할때 사용할 초기화 공간
	{
	case EMonsterState::Idle:

		break;
	case EMonsterState::SuperArmor:

		break;
	case EMonsterState::Groggy:
		EnemyCtrl->StopBT();
		anim->StopAllMontages(0.1f);
		GetWorld()->GetTimerManager().SetTimer(GroggyTimerHandle, FTimerDelegate::CreateLambda([this]()->void
		{
			groggyTime -= 0.5f;
			if (groggyTime==0.f)
			{
				GetWorldTimerManager().ClearTimer(GroggyTimerHandle);
				HitAffter();
				SetMonsterState(EMonsterState::Idle);
				groggyTime = 3.5f;
			}
		 }), 0.5f, true);
		break;
	default:
		break;
	}
}
void AEnemy::Attack(int32 infoIndex)
{
	if (!IsAttacking)
	{
		anim->PlayEnemyMontage(AttackPatten[infoIndex].montage);
		CurrentAttackinfo = AttackPatten[infoIndex];
		IsAttacking = true;
	}
}
void AEnemy::AttackCheck()
{
	FHitResult hitResult;
	FCollisionQueryParams params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel
	(
		hitResult,GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * CurrentAttackinfo.AttackRange,
		FQuat::Identity,ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(CurrentAttackinfo.AttackRadius),params
	);
#ifdef ENABLE_DRAW_DEBUG
	FVector traceVec = GetActorForwardVector() * CurrentAttackinfo.AttackRange;
	FVector center = GetActorLocation() + traceVec * 0.5f;
	float halfheight = CurrentAttackinfo.AttackRange * 0.5f + CurrentAttackinfo.AttackRadius;
	FQuat capsuleRot = FRotationMatrix::MakeFromZ(traceVec).ToQuat();
	FColor drawColor = bResult ? FColor::Green : FColor::Red;
	float debugLifeTime = 5.0f;
	DrawDebugCapsule(GetWorld(), center, halfheight, CurrentAttackinfo.AttackRadius, capsuleRot, drawColor, false, debugLifeTime);
#endif  
	if (bResult)
	{
		if (hitResult.GetActor())
		{
			FDamageEvent damageEvent;
			hitResult.GetActor()->TakeDamage(CurrentAttackinfo.Damage, damageEvent, GetController(), this);
		}
	}
}
void AEnemy::HitAffter() //현재는 노티파이로 체크하는 구조지만 일정 시간 동안 ai 무력화 기능으로 개편될듯
{
	EnemyCtrl->RunBT();
}
void AEnemy::HitEffect(FAttackInfoStruct info)
{
	switch (CurrentMonsterState)
	{
	case EMonsterState::Idle:
		EnemyCtrl->StopBT();
		Stat->SetDamage(info.Damage);
		anim->PlayEnemyMontage(HitMontage);
		if (info.HitType== EHitEffectType::Stiff) //경직 실행
		{
			const FVector Direction = FRotationMatrix(FRotator(0, GetActorRotation().Yaw, 0)).GetUnitAxis(EAxis::X);
			LaunchCharacter(Direction * -500.f, true, true);
			GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString::Printf(TEXT("Stiff")));
		}
		else if (info.HitType == EHitEffectType::KnockBack) //넉백 실행
		{
			const FVector Direction = FRotationMatrix(FRotator(0, GetActorRotation().Yaw, 0)).GetUnitAxis(EAxis::X);
			LaunchCharacter(Direction * -1000.f, true, true);
			GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString::Printf(TEXT("KnockBack")));
		}
		else if (info.HitType == EHitEffectType::Airborne) //에어본 실행
		{
			Jump();
			GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString::Printf(TEXT("Airborne")));
		}
		break;
	case EMonsterState::SuperArmor:
		Stat->SetDamage(info.Damage);
		break;
	case EMonsterState::Groggy:
		Stat->SetDamage(info.Damage);
		//그로기 몽타주를 루프로 실행
		break;
	default:
		break;
	}

}

