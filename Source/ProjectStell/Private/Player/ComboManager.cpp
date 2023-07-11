#include "Player/ComboManager.h"
#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharacterAnim.h"
#include "Weapon/Weapon.h"
UComboManager::UComboManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}
void UComboManager::InitComboManager()
{
	character = Cast<APlayerCharacter>(GetOwner());
	character->GetCharacterAnim()->OnAttackHitCheck.AddUObject(this, &UComboManager::AttackCheck); 
	character->GetCharacterAnim()->OnAttackEndCheck.AddUObject(this, &UComboManager::AttackEnd);
	character->GetCharacterAnim()->OnAttackTimeCheck.AddUObject(this, &UComboManager::ConnectAttack);
}
bool UComboManager::MakeAttackRange(TArray<FHitResult>* hitResults)
{
	
	FCollisionQueryParams params(NAME_None, false, character);
	bool bResult;
	const FVector ResultVector = CurrentAttackInfo.AttackLocation.X * character->GetActorForwardVector() + CurrentAttackInfo.AttackLocation.Y * character->GetActorRightVector() + CurrentAttackInfo.AttackLocation.Z * character->GetActorUpVector();

	FVector StartVector = character->GetActorLocation() + ResultVector;
	FVector EndVector =  StartVector+ character->GetActorForwardVector() * CurrentAttackInfo.AttackScale;
#ifdef ENABLE_DRAW_DEBUG
	FVector center = character->GetActorLocation() + StartVector * 0.5f;
	FQuat capsuleRot = FRotationMatrix::MakeFromZ(StartVector).ToQuat();
	float debugLifeTime = 5.0f;
	float halfheight = FCollisionShape::MakeCapsule(CurrentAttackInfo.AttackScale).GetCapsuleHalfHeight();
	float radius = FCollisionShape::MakeSphere(CurrentAttackInfo.AttackScale.Size()).GetSphereRadius();
	FVector extent = FCollisionShape::MakeBox(CurrentAttackInfo.AttackScale * 0.5).GetExtent();
	FColor drawColor;
#endif
	switch (CurrentAttackInfo.AttackShape)
	{
	case EAttackShapeType::Weapon:
		break;
	case EAttackShapeType::Line:
		break;
	case EAttackShapeType::Sphere:
		bResult = GetWorld()->SweepMultiByChannel(*hitResults, StartVector, EndVector,FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(CurrentAttackInfo.AttackScale.Size()),
		params);
		drawColor = bResult ? FColor::Green : FColor::Red;
		DrawDebugSphere(GetWorld(), StartVector, radius, 8, drawColor, false, debugLifeTime);
		break;
	case EAttackShapeType::Capsule:
		bResult = GetWorld()->SweepMultiByChannel(*hitResults, StartVector, EndVector, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeCapsule(CurrentAttackInfo.AttackScale),
		params);
		drawColor = bResult ? FColor::Green : FColor::Red;
		DrawDebugCapsule(GetWorld(), StartVector, halfheight, FCollisionShape::MakeCapsule(CurrentAttackInfo.AttackScale).GetCapsuleRadius(), capsuleRot, drawColor, false, debugLifeTime);
		break;
	case EAttackShapeType::Box:
		bResult = GetWorld()->SweepMultiByChannel(*hitResults, StartVector, EndVector, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeBox(CurrentAttackInfo.AttackScale *0.5f),
		params);
		drawColor = bResult ? FColor::Green : FColor::Red;
		DrawDebugBox(GetWorld(), StartVector, extent, capsuleRot, drawColor, false, debugLifeTime);
		break;
	default:
		break;
	}


	return bResult;
}
FAttackInfoStruct UComboManager::GetCurAttackInfo()
{
	return CurrentAttackInfo;
}
void UComboManager::Attack(bool isLeftClick)
{
	UPlayerCharacterAnim* anim = character->GetCharacterAnim();
	if (anim == nullptr) return;
	AWeapon* LeftWeapon = character->GetLeftWeapon();
	if (LeftWeapon == nullptr)return;
	AWeapon* RightWeapon = character->GetRightWeapon();
	if (RightWeapon == nullptr)return;

	bool result = InputCheck();
	if (result)
	{
		bool result2;
		if (isLeftClick) result2 = FindAttackInfo(LeftWeapon, RightWeapon);
		else result2 = FindAttackInfo(RightWeapon, LeftWeapon);
		if (result2)
		{
			CanNextAttack = false;
			anim->SetMirror(isLeftClick);
			if (PreAttackInfo.isCancelAble)
			{
				anim->PlayPlayerMontage(CurrentAttackInfo.montage, CurrentAttackInfo.PlaySpeed);
				PreAttackInfo = CurrentAttackInfo;
			}
			else
				NextAttackInfo = CurrentAttackInfo;
		}
		else
			AttackReset();
	}
}
bool UComboManager::InputCheck()
{
	if (IsAttacking) //이미 공격 중이었는데 입력이 또 들어온 경우
	{
		if (CanNextAttack) return true;
		return false;
	}
	else //공격중이 아닌데 입력이 들어온 경우
	{
		IsAttacking = true;
		CurrentCombo = 0;
		return true;
	}
}
void UComboManager::ConnectAttack()
{
	CanNextAttack = true;
}
void UComboManager::AttackEnd()
{
	if (NextAttackInfo.montage != nullptr) //다음 공격이 존재 하는 경우
	{
		UPlayerCharacterAnim* anim = character->GetCharacterAnim();
		if (anim == nullptr) return;
		CurrentAttackInfo = NextAttackInfo;
		anim->PlayPlayerMontage(CurrentAttackInfo.montage, CurrentAttackInfo.PlaySpeed);
		PreAttackInfo = CurrentAttackInfo;
		NextAttackInfo = FAttackInfoStruct();
	}
	else //공격 종료
	{
		IsAttacking = false;
		PreAttackInfo = FAttackInfoStruct();
	}
}
bool UComboManager::FindAttackInfo(AWeapon* clickWeapon, AWeapon* otherWeapon)
{
	TArray<int32> combolist = clickWeapon->ComboList.FindRef(otherWeapon->Type).ComboIndex;
	TArray<FAttackInfoStruct> attacklist = clickWeapon->AttackInfo;
	int32 Attackindex = -1;

	if (!combolist.IsValidIndex(CurrentCombo)) return false;

	Attackindex = combolist[CurrentCombo];
	if (attacklist.IsValidIndex(Attackindex) && 0<=Attackindex)
	{
		CurrentAttackInfo = attacklist[Attackindex];
		CurrentCombo += 1;
		return true;
	}
	else
	{
		CurrentAttackInfo = FAttackInfoStruct();
		CurrentCombo = 0;
		return false;
	}
}
void UComboManager::AttackCheck()
{
	/*아래 코드를 함수화 
	bool bResult = GetWorld()->SweepMultiByChannel(hitResults,character->GetActorLocation(),
		character->GetActorLocation() + character->GetActorForwardVector() * CurrentAttackInfo.AttackRange,
		FQuat::Identity,ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(CurrentAttackInfo.AttackRadius),
		params);
	//MakeSphere , MakeBox , MakeCapsule  CurrentAttackInfo.AttackShape == AttackShapeType::Weapon;
#ifdef ENABLE_DRAW_DEBUG
	FVector traceVec = character->GetActorForwardVector() * CurrentAttackInfo.AttackRange;
	FVector center = character->GetActorLocation() + traceVec * 0.5f;
	float halfheight = CurrentAttackInfo.AttackRange * 0.5f + CurrentAttackInfo.AttackRadius;
	FQuat capsuleRot = FRotationMatrix::MakeFromZ(traceVec).ToQuat();
	FColor drawColor = bResult ? FColor::Green : FColor::Red;
	float debugLifeTime = 5.0f;
	DrawDebugCapsule(GetWorld(), center, halfheight, CurrentAttackInfo.AttackRadius, capsuleRot, drawColor, false, debugLifeTime);
#endif
	//여기까지 함수화
	*/
	TArray<FHitResult> hitResults;

	bool bResult = MakeAttackRange(&hitResults);
	if (bResult)
	{
		for (auto hitResult : hitResults)
		{
			if (hitResult.GetActor())
			{
				//캐릭터 회전 보정
				FVector DirectionToOtherCharacter = hitResult.GetActor()->GetActorLocation() - character->GetActorLocation();
				DirectionToOtherCharacter.Z = 0.0f;
				DirectionToOtherCharacter.Normalize();
				FRotator TargetRotation = FRotationMatrix::MakeFromX(DirectionToOtherCharacter).Rotator();
				character->SetActorRotation(TargetRotation);

				//데미지 결정
				FDamageEvent DamageEvent;
				float LastDamage = CurrentAttackInfo.Damage;
				auto other = Cast<AActor>(hitResult.GetActor());
				UGameplayStatics::ApplyDamage(other, LastDamage, character->GetController(), character, NULL);
			}
		}
	}
}
void UComboManager::AttackReset()
{
	 PreAttackInfo = FAttackInfoStruct();
	 CurrentAttackInfo = PreAttackInfo;
	 NextAttackInfo = PreAttackInfo;
	 IsAttacking = false;
	 CanNextAttack = false;
}



