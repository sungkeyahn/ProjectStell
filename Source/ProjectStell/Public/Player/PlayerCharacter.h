// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"
UENUM(BlueprintType)
enum class EPlayerActionType : uint8 //캐릭터가 할수 있는 행동들
{
	None, Hit
};
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
	UFUNCTION()
		void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
public:
	class UPlayerCharacterAnim* GetCharacterAnim();

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
	void SetViewMode();

	//무기, 공격, 콤보 관련 WeaponManager만들어 따로 관리 예정
private:
	class AWeapon* leftWeapon;
	class AWeapon* rightWeapon;
public:
	UFUNCTION(BlueprintCallable)
		void PutOnWeapon(FName path = "", int hand = 0);
};
