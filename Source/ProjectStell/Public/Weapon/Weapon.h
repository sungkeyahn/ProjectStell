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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AttackRange = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AttackRadius = 0.f;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		TArray<FAttackInfoStruct> AttackInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		TMap<EWeaponType, FComboInfoStruct> ComboList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		EWeaponType Type = EWeaponType::None;
};
