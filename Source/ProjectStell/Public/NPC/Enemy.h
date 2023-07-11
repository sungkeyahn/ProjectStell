#pragma once

#include "ProjectStell.h"
#include "GameFramework/Character.h"
#include "Weapon/Weapon.h"
#include "Enemy.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FMonsterAttackInfoStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* montage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlaySpeed = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Damage = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AttackRange = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AttackRadius = 0.f;
};

UENUM(BlueprintType)
enum class EMonsterState :uint8
{
	Idle, SuperArmor, Groggy , Invincibility
};

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class PROJECTSTELL_API AEnemy : public ACharacter
{
	GENERATED_BODY()
//초기화
public:
	AEnemy();
protected:
	virtual void BeginPlay() override;
public:
	virtual void PostInitializeComponents()override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
//컨트롤러 관련
private:
	UPROPERTY()
		class AEnemyCtrl* EnemyCtrl;
//스텟 관련
protected:
	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UStat* Stat;

//애니메이션 관련
protected:
	UPROPERTY()
		class UEnemyAnim* anim;
	UFUNCTION()
		void OnAttackMontageEnded(class UAnimMontage* Montage, bool bInterrupted);

//게임상태, 몬스터 상태관련
protected: 
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = GameState)
		EEnemyStateInGame CurrentInGameState;
	UPROPERTY(VisibleAnywhere, Category = State)
		EMonsterState CurrentMonsterState = EMonsterState::Idle;
public:
	void SetInGameState(EEnemyStateInGame newState);
	void SetMonsterState(EMonsterState newState);

//공격 ,이동 관련
protected:
	FVector DirectionToMove = FVector::ZeroVector;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Sense, Meta = (AllowPrivateAccess = true))
		float SenseRange = 500.f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking = false;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
		TArray<FMonsterAttackInfoStruct> AttackPatten;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Sense, Meta = (AllowPrivateAccess = true))
		float AttackSenseRange = 100.f;
	FMonsterAttackInfoStruct CurrentAttackinfo;
	virtual void Attack(int32 infoIndex);
	virtual void Attack(FMonsterAttackInfoStruct attackInfo);

	void AttackCheck();
	FOnAttackEndDelegate OnAttackEnd;

//피격 관련
protected:
	bool isHit = false;
	FTimerHandle SternTimerHandle;
	FAttackInfoStruct takeAttackInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta = (AllowPrivateAccess = true))
		class UAnimMontage* HitMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hit, Meta = (AllowPrivateAccess = true))
		float SternTime=0.0f;
	void Stern(float sterntime);

public:
	UFUNCTION()
		void HitEffect();
//사망 관련
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
		float DeadTime = 0;
	FTimerHandle DeadTimerHandle;
};
