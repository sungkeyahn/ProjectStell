// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"
UENUM(BlueprintType)
enum class EPlayerActionType : uint8 //ĳ���Ͱ� �Ҽ� �ִ� �ൿ��
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
	UFUNCTION()
		void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
public:
	class UPlayerCharacterAnim* GetCharacterAnim();

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
	void SetViewMode();

	//����, ����, �޺� ���� WeaponManager����� ���� ���� ����
private:
	class AWeapon* leftWeapon;
	class AWeapon* rightWeapon;
public:
	UFUNCTION(BlueprintCallable)
		void PutOnWeapon(FName path = "", int hand = 0);
};
