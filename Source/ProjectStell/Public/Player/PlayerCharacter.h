// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class PROJECTSTELL_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	APlayerCharacter();
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents()override;

//Ű�Է� ���ε� ��� ���� 
private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	void LeftAttack();
	void RightAttack();
	void Evasion();
//�ִϸ��̼� ����
private:
	UPROPERTY()
		class UPlayerCharacterAnim* anim;
public:
	class UPlayerCharacterAnim* GetCharacterAnim();
//��Ʈ�ѷ� ����
	class APlayerCharaterCtrl* PlayerCtrl;
//����,ī�޶� ����
public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* springArm;
	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* camera;
protected:
	FVector directionToMove = FVector::ZeroVector;
	FRotator armRotationTo = FRotator::ZeroRotator;
	float armLengthTo = 0.0f;
	float armLengthSpeed = 3.0f;
	float armRotationSpeed = 10.0f;
	void DefaultViewSetting();
//�ǰ� ����
public:
	virtual float TakeDamage(float DamageAmout,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser)override;
//���� ���� 
private:
	class AWeapon* leftWeapon;
	class AWeapon* rightWeapon;
public:
	class AWeapon* GetLeftWeapon();
	class AWeapon* GetRightWeapon();
	UFUNCTION(BlueprintCallable)
		void PutOnWeapon(FName path = "", int hand = 0);
//�޺� ����	
	UPROPERTY(VisibleAnywhere, Category = Combo)
		class UComboManager* Combo;
//���ݰ���
	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UPlayerStat* Stat;
//�뽬 ����
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dash, Meta = (AllowPrivateAccess = true))
		class UAnimMontage* DashMontage = nullptr;
	UPROPERTY(VisibleAnywhere,Category = Dash)
		bool IsDashing;
	UPROPERTY(VisibleAnywhere, Category = Dash)
		int32 DashCount = 2;
	UPROPERTY(VisibleAnywhere,Category = Dash)
		int32 DashCoolTime = 10;
	FTimerHandle DashCoolTimerHandle;
	void DashCoolTimer();
//��� ����
private:
	void KillPlayer();
	FTimerHandle CharacterDstroyTimerHandle;
	float CharacterDstroyCoolTime = 0;
	void CharacterDestroyTimer();
};
