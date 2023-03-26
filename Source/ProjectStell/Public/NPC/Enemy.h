// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class PROJECTSTELL_API AEnemy : public ACharacter
{
	GENERATED_BODY()
//�ʱ�ȭ
public:
	AEnemy();
protected:
	virtual void BeginPlay() override;
public:
	virtual void PostInitializeComponents()override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

//��Ʈ�ѷ� ����
private:
	UPROPERTY()
		class AEnemyCtrl* EnemyCtrl;

//�ִϸ��̼� ����
private:
	UPROPERTY()
		class UEnemyAnim* anim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta = (AllowPrivateAccess = true))
		class UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta = (AllowPrivateAccess = true))
		class UAnimMontage* HitMontage;
	UFUNCTION()
		void OnAttackMontageEnded(class UAnimMontage* Montage, bool bInterrupted);

//���ݰ���
	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UStat* Stat;

//���ӻ��� ����
private:
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		EEnemyStateInGame CurrentInGameState;
public:
	EEnemyStateInGame GetInGameState()const;
	void SetInGameState(EEnemyStateInGame newState);

//�ൿ ���� 
private:
	//�̵�
	FVector DirectionToMove = FVector::ZeroVector;
	//����
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking = false;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRange;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRadius;
	void Attack();
	void AttackCheck();
	//���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
		float DeadTimer;
	FTimerHandle DeadTimerHandle = {};
};
