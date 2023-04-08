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

//키입력 바인딩 기능 관련 
private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	void LeftAttack();
	void RightAttack();
	void Evasion();
//애니메이션 관련
private:
	UPROPERTY()
		class UPlayerCharacterAnim* anim;
public:
	class UPlayerCharacterAnim* GetCharacterAnim();
//컨트롤러 관련
	class APlayerCharaterCtrl* PlayerCtrl;
//시점,카메라 관련
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
//피격 관련
public:
	virtual float TakeDamage(float DamageAmout,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser)override;
//무기 관련 
private:
	class AWeapon* leftWeapon;
	class AWeapon* rightWeapon;
public:
	class AWeapon* GetLeftWeapon();
	class AWeapon* GetRightWeapon();
	UFUNCTION(BlueprintCallable)
		void PutOnWeapon(FName path = "", int hand = 0);
//콤보 관련	
	UPROPERTY(VisibleAnywhere, Category = Combo)
		class UComboManager* Combo;
//스텟관련
	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UPlayerStat* Stat;
//대쉬 관련
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
//사망 관련
private:
	void KillPlayer();
	FTimerHandle CharacterDstroyTimerHandle;
	float CharacterDstroyCoolTime = 0;
	void CharacterDestroyTimer();
};
