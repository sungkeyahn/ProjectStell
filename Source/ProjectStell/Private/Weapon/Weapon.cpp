// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"
#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharacterAnim.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = mesh;
	Setting_ComboAttackEnd();
}
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}
void AWeapon::InitWeapon()
{
	character = Cast<APlayerCharacter>(GetOwner());
	anim = character->GetCharacterAnim();
	anim->OnAttackHitCheck.AddUObject(this, &AWeapon::AttackCheck);
	anim->OnNextAttackComboCheck.AddUObject(this, &AWeapon::NextComboActionCheck);
	anim->OnMontageEnded.AddDynamic(this, &AWeapon::OnMontageEnded);
	anim->OnMontageFeasibleCheck.BindLambda([this](UAnimMontage* mon) {return  true; });

	/*ȣ���� ���⼭ �ؾ߰ٴµ�?
	* 1.��Ÿ�ְ� ����ȴ� -
	* 2.��Ÿ�ְ� ����Ǳ� ���� ���ǹ����� �˻��Ѵ� < ������� �ִ� �ν��Ͻ�
	* 3.���⿡�� �Ű������� ���� ��Ÿ�ְ� �������� �˻��ϰ� �ش� ��Ÿ�ְ� ���⿡ �ִ¸�Ÿ���̸�
	* �˻縦 �����Ѵ�.
	*/

}
void AWeapon::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AttackMontage)
	{
		if (!IsAttacking)return;
		//if(CurrentCombo > 0)
		//if (!IsComboInputOn) return;
		Setting_ComboAttackEnd();
	}
	else if (Montage == EvasionMontage)
	{

	}
}
UAnimMontage* AWeapon::GetAttackMontage()
{
	return AttackMontage;
}
UAnimMontage* AWeapon::GetEvasionMontage()
{
	return EvasionMontage;
}
void AWeapon::Setting_ComboAttackStart()
{
	IsAttacking = true;
	CanNextCombo = true;
	IsComboInputOn = false;
	if (FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1) == false) return;
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}
void AWeapon::Setting_ComboAttackEnd()
{
	IsAttacking = false;
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}
void AWeapon::Action(EWeaponActionType type)
{
	if (anim == nullptr) return;
	switch (type)
	{
	case EWeaponActionType::None:
	{
		break;
	}
	case EWeaponActionType::Attack:
	{
		if (IsAttacking)
		{
			if (FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo) == false) return;
			if (CanNextCombo) IsComboInputOn = true;
		}
		else
		{
			if (CurrentCombo != 0) return;
			Setting_ComboAttackStart();
			anim->PlayPlayerMontage(AttackMontage);
			anim->NextSectionPlay_PlayerMontage(AttackMontage, CurrentCombo);
		}
		break;
	}
	case EWeaponActionType::Evasion:
	{
		anim->PlayPlayerMontage(EvasionMontage);
		break;
	}
	case EWeaponActionType::Guard:
	{
		break;
	}
	}
}
void AWeapon::NextComboActionCheck()
{
	CanNextCombo = false;
	if (IsComboInputOn)
	{
		Setting_ComboAttackStart();
		anim->NextSectionPlay_PlayerMontage(AttackMontage, CurrentCombo);
	}
}
void AWeapon::AttackCheck()
{
	TArray<FHitResult> hitResults;
	FCollisionQueryParams params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepMultiByChannel(hitResults,
		character->GetActorLocation(),
		character->GetActorLocation() + character->GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel12,
		FCollisionShape::MakeSphere(AttackRadius),
		params);
#ifdef ENABLE_DRAW_DEBUG
	FVector traceVec = character->GetActorForwardVector() * AttackRange;
	FVector center = character->GetActorLocation() + traceVec * 0.5f;
	float halfheight = AttackRange * 0.5f + AttackRadius;
	FQuat capsuleRot = FRotationMatrix::MakeFromZ(traceVec).ToQuat();
	FColor drawColor = bResult ? FColor::Green : FColor::Red;
	float debugLifeTime = 5.0f;
	DrawDebugCapsule(GetWorld(), center, halfheight, AttackRadius, capsuleRot, drawColor, false, debugLifeTime);
#endif  
	//�������� 
	if (bResult)
	{
		for (auto hitResult : hitResults)
		{
			if (hitResult.GetActor())
			{
				FDamageEvent DamageEvent;
				float LastDamage = Damage; //���������� ������ ���Ŀ� ���� ���� �� �� �ִ� �κ���
				auto other = Cast<AActor>(hitResult.GetActor());
				UGameplayStatics::ApplyDamage(other, LastDamage, character->GetController(), nullptr, NULL);
			}
		}
	}
}


