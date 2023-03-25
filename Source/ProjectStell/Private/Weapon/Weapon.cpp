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

	/*호출을 여기서 해야겟는데?
	* 1.몽타주가 실행된다 -
	* 2.몽타주가 실행되기 전에 조건문으로 검사한다 < 여기까지 애님 인스턴스
	* 3.무기에서 매개변수로 받은 몽타주가 무엇인지 검사하고 해당 몽타주가 무기에 있는몽타주이면
	* 검사를 진행한다.
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
	//판정여부 
	if (bResult)
	{
		for (auto hitResult : hitResults)
		{
			if (hitResult.GetActor())
			{
				FDamageEvent DamageEvent;
				float LastDamage = Damage; //데미지연산 과정은 추후에 따로 빠질 수 도 있는 부분임
				auto other = Cast<AActor>(hitResult.GetActor());
				UGameplayStatics::ApplyDamage(other, LastDamage, character->GetController(), nullptr, NULL);
			}
		}
	}
}


