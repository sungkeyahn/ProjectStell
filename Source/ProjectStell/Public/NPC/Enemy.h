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
	Idle, SuperArmor, Groggy
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
	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UStat* Stat;
//애니메이션 관련
private:
	UPROPERTY()
		class UEnemyAnim* anim;
	UFUNCTION()
		void OnAttackMontageEnded(class UAnimMontage* Montage, bool bInterrupted);

//게임상태, 몬스터 상태관련
public:
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = GameState)
		EEnemyStateInGame CurrentInGameState;
	UPROPERTY(VisibleAnywhere, Category = State)
		EMonsterState CurrentMonsterState = EMonsterState::Idle;
	void SetInGameState(EEnemyStateInGame newState);
	void SetMonsterState(EMonsterState newState);

//공격 ,이동 관련
private:
	FVector DirectionToMove = FVector::ZeroVector;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Sense, Meta = (AllowPrivateAccess = true))
		float SenseRange = 400.f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
		TArray<FMonsterAttackInfoStruct> AttackPatten;
public:
	FMonsterAttackInfoStruct CurrentAttackinfo;
	void Attack(int32 infoIndex);
	void AttackCheck();
	FOnAttackEndDelegate OnAttackEnd;

//피격 관련
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta = (AllowPrivateAccess = true))
		class UAnimMontage* HitMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hit, Meta = (AllowPrivateAccess = true))
		float stiffTime=0.5f;
	FTimerHandle GroggyTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hit, Meta = (AllowPrivateAccess = true))
		float groggyTime = 3.5f;;
	FAttackInfoStruct takeAttackInfo;

public:
	UFUNCTION()
		void HitEffect();
//사망 관련
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
		float DeadTime = 0;
	FTimerHandle DeadTimerHandle;
};
