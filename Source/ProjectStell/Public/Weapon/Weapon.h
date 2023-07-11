// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FAttackInfoStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* montage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlaySpeed = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Damage = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)//공격 범위의 형태(원통형,직선형,큐브형,무기형태)
		EAttackShapeType AttackShape = EAttackShapeType::Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)//공격 범위가 캐릭터를 기준으로 나타나는 기준위치
		FVector AttackLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)//공격 범위의 크기
		FVector AttackScale;

	/*
	//이 부분이 공격 범위 구조체로 변경될 예정
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FAttackRangeStruct AttackRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AttackRange = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AttackRadius = 0.f;
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isCancelAble = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EHitEffectType HitType = EHitEffectType::Stiff;
	UPROPERTY(EditAnywhere, Category = Camera)
		TSubclassOf<UCameraShakeBase> CameraShakeType;
};

USTRUCT(Atomic, BlueprintType)
struct FComboInfoStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int32> ComboIndex;
};
UCLASS()
class PROJECTSTELL_API AWeapon : public AActor
{
	GENERATED_BODY()
public:
	AWeapon();
protected:
	virtual void BeginPlay() override;
public:
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		class USkeletalMeshComponent* mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		FName LeftSocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		FName RightSocketName;
	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		 FVector loc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		 FRotator rot;
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		TArray<FAttackInfoStruct> AttackInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		TMap<EWeaponType, FComboInfoStruct> ComboList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		EWeaponType Type = EWeaponType::None;

};
