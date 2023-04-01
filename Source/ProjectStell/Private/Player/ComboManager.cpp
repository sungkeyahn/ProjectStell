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
	//character->GetCharacterAnim()->OnMontageEnded.AddDynamic(this, &UComboManager::AttackEnd);
	character->GetCharacterAnim()->OnAttackTimeCheck.AddUObject(this, &UComboManager::ConnectAttack);

}
void UComboManager::Attack(bool isLeftClick)
{
	UPlayerCharacterAnim* anim = character->GetCharacterAnim();
	if (anim == nullptr) return;
	AWeapon* LeftWeapon = character->GetLeftWeapon();
	if (LeftWeapon == nullptr)return;
	AWeapon* RightWeapon = character->GetRightWeapon();
	if (RightWeapon == nullptr)return;

	//입력에 대응할 수 있는 상태인지 체크
	bool result = InputCheck();

	//공격정보 탐색
	if (result)
	{
		bool result2;
		if (isLeftClick)
			result2 = FindAttackInfo(LeftWeapon, RightWeapon);
		else
			result2 = FindAttackInfo(RightWeapon, LeftWeapon);

		if (result2) //여기서부터 공격 실행 , 하나의 동작이 필수 재생이 끝나면 CanNextAttack 을 true로 변경해야함
		{
			CanNextAttack = false;
			anim->SetMirror(isLeftClick);
			if (PreAttackInfo.isCancelAble) //캔슬은 1타 이전에는 사용불가능 하기 때문에 1타용 로직이 필요
			{
				//anim->StopPlayerMontage(PreAttackInfo.montage);
				anim->PlayPlayerMontage(CurrentAttackInfo.montage, CurrentAttackInfo.PlaySpeed);
			}
			else
				anim->SetNextAttack(CurrentAttackInfo.montage, CurrentAttackInfo.PlaySpeed);
			PreAttackInfo = CurrentAttackInfo;
		}
	}
}
bool UComboManager::InputCheck()
{
	if (IsAttacking) //이미 공격 중이었는데 입력이 또 들어온 경우
	{
		if (CanNextAttack) //이전 애니메이션의 필수 재생시간이 끝나기 전에 입력시 입력무시
		{
			CurrentCombo += 1;
			return true;
		}
		else return false;
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
void UComboManager::AttackEnd() //공격 종료 같은 부분은 델리게이트로 처리
{
	IsAttacking = false;
}
bool UComboManager::FindAttackInfo(AWeapon* clickWeapon, AWeapon* otherWeapon)
{
	TArray<int32> combolist = clickWeapon->ComboList.FindRef(otherWeapon->Type).ComboIndex;
	TArray<FAttackInfoStruct> attacklist = clickWeapon->AttackInfo;
	int32 Attackindex = -1;

	if (combolist.IsValidIndex(CurrentCombo))
		Attackindex = combolist[CurrentCombo];

	if (attacklist.IsValidIndex(Attackindex))
	{
		CurrentAttackInfo = attacklist[Attackindex];
		return true;
	}
	else
		return false;
}
void UComboManager::AttackCheck()
{
	TArray<FHitResult> hitResults;
	bool bResult = GetWorld()->SweepMultiByChannel(hitResults,
		character->GetActorLocation(),
		character->GetActorLocation() + character->GetActorForwardVector() * CurrentAttackInfo.AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel12,
		FCollisionShape::MakeSphere(CurrentAttackInfo.AttackRadius),
		FCollisionQueryParams());

#ifdef ENABLE_DRAW_DEBUG
	FVector traceVec = character->GetActorForwardVector() * CurrentAttackInfo.AttackRange;
	FVector center = character->GetActorLocation() + traceVec * 0.5f;
	float halfheight = CurrentAttackInfo.AttackRange * 0.5f + CurrentAttackInfo.AttackRadius;
	FQuat capsuleRot = FRotationMatrix::MakeFromZ(traceVec).ToQuat();
	FColor drawColor = bResult ? FColor::Green : FColor::Red;
	float debugLifeTime = 5.0f;
	DrawDebugCapsule(GetWorld(), center, halfheight, CurrentAttackInfo.AttackRadius, capsuleRot, drawColor, false, debugLifeTime);
#endif  
	//판정여부 
	if (bResult)
	{
		for (auto hitResult : hitResults)
		{
			if (hitResult.GetActor())
			{
				FDamageEvent DamageEvent;
				float LastDamage = CurrentAttackInfo.Damage; //데미지연산 과정은 추후에 따로 빠질 수 도 있는 부분임
				auto other = Cast<AActor>(hitResult.GetActor());
				UGameplayStatics::ApplyDamage(other, LastDamage, character->GetController(), nullptr, NULL);
			}
		}
	}
}


