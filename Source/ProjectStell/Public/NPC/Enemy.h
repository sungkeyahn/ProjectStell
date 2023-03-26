// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

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

//애니메이션 관련
private:
	UPROPERTY()
		class UEnemyAnim* anim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta = (AllowPrivateAccess = true))
		class UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta = (AllowPrivateAccess = true))
		class UAnimMontage* HitMontage;
	UFUNCTION()
		void OnAttackMontageEnded(class UAnimMontage* Montage, bool bInterrupted);

//스텟관련
	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UStat* Stat;

//게임상태 관련
private:
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		EEnemyStateInGame CurrentInGameState;
public:
	EEnemyStateInGame GetInGameState()const;
	void SetInGameState(EEnemyStateInGame newState);

//행동 관련 
private:
	//이동
	FVector DirectionToMove = FVector::ZeroVector;
	//공격
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking = false;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRange;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRadius;
	void Attack();
	void AttackCheck();
	//사망
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
		float DeadTimer;
	FTimerHandle DeadTimerHandle = {};
};
