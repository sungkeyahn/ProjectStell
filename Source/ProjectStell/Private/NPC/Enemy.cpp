#include "NPC/Enemy.h"
#include "NPC/EnemyAnim.h"
#include "NPC/EnemyCtrl.h"
#include "Stat/Stat.h"
#include "ProjectStellGameModeBase.h"

#include "DrawDebugHelpers.h"
#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharaterCtrl.h"
#include "Player/ComboManager.h"

AEnemy::AEnemy()
{
	Stat = CreateDefaultSubobject<UStat>(TEXT("Stat"));
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
	anim->OnAttackHitCheck.AddUObject(this, &AEnemy::AttackCheck);
	Stat->OnHpIsZero.AddLambda([this]()->void {SetInGameState(EEnemyStateInGame::Dead); });
	Stat->OnHpChanged.AddUObject(this, &AEnemy::HitEffect);
	SetInGameState(EEnemyStateInGame::Loading);
}
float AEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
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
	}

	return FinalDamage;
}
void AEnemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (IsAttacking) 
	{
		IsAttacking = false;
		OnAttackEnd.Broadcast();
	}
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
			anim->IsDead=true;
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
void AEnemy::Attack(FMonsterAttackInfoStruct attackInfo)
{
	if (!IsAttacking)
	{
		anim->PlayEnemyMontage(attackInfo.montage);
		CurrentAttackinfo = attackInfo;
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
			GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString::Printf(TEXT("asdasd")));

			FDamageEvent damageEvent;
			hitResult.GetActor()->TakeDamage(CurrentAttackinfo.Damage, damageEvent, GetController(), this);
		}
	}
}
void AEnemy::Stern(float sterntime) //이 함수에 시간을 넣으면 자동으로 그만큼 멈추고 실행시켜줌 
{
	SternTime = sterntime;
	EnemyCtrl->StopBT();
	if (isHit) //피격중에 또맞으면 타이머 초기화
		GetWorldTimerManager().ClearTimer(SternTimerHandle);

	GetWorld()->GetTimerManager().SetTimer(SternTimerHandle,FTimerDelegate::CreateLambda([this]()->void
	{
		SternTime -= 1.0f;
	if (SternTime <= 0.f)
	{
		EnemyCtrl->RunBT();
		isHit = false;
		GetWorldTimerManager().ClearTimer(SternTimerHandle);
	}}), 1.0f, true);
}
void AEnemy::HitEffect()
{
	switch (CurrentMonsterState)
	{
	case EMonsterState::Idle:
		Stern(1.0f);
		anim->PlayEnemyMontage(HitMontage);
		if (takeAttackInfo.HitType== EHitEffectType::Stiff) //경직 실행
		{
			const FVector Direction = FRotationMatrix(FRotator(0, GetActorRotation().Yaw, 0)).GetUnitAxis(EAxis::X);
			LaunchCharacter(Direction * -500.f, true, true);
			GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString::Printf(TEXT("Stiff")));
		}
		else if (takeAttackInfo.HitType == EHitEffectType::KnockBack) //넉백 실행
		{
			const FVector Direction = FRotationMatrix(FRotator(0, GetActorRotation().Yaw, 0)).GetUnitAxis(EAxis::X);
			LaunchCharacter(Direction * -1000.f, true, true);
			GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString::Printf(TEXT("KnockBack")));
		}
		else if (takeAttackInfo.HitType == EHitEffectType::Airborne) //에어본 실행
		{
			Jump();
			GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString::Printf(TEXT("Airborne")));
		}
		break;
	case EMonsterState::SuperArmor:
		break;
	case EMonsterState::Groggy:
		//Stern(3.5f); SetMonsterState(EMonsterState::Idle); 몬스터의 상태변화는 여기서 설정하는것이아님 
		break;
	default:
		Stern(0.0f);
		break;
	}
}

