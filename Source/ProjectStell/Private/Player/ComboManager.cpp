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

	//�Է¿� ������ �� �ִ� �������� üũ
	bool result = InputCheck();

	//�������� Ž��
	if (result)
	{
		bool result2;
		if (isLeftClick)
			result2 = FindAttackInfo(LeftWeapon, RightWeapon);
		else
			result2 = FindAttackInfo(RightWeapon, LeftWeapon);

		if (result2) //���⼭���� ���� ���� , �ϳ��� ������ �ʼ� ����� ������ CanNextAttack �� true�� �����ؾ���
		{
			CanNextAttack = false;
			anim->SetMirror(isLeftClick);
			if (PreAttackInfo.isCancelAble) //ĵ���� 1Ÿ �������� ���Ұ��� �ϱ� ������ 1Ÿ�� ������ �ʿ�
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
	if (IsAttacking) //�̹� ���� ���̾��µ� �Է��� �� ���� ���
	{
		if (CanNextAttack) //���� �ִϸ��̼��� �ʼ� ����ð��� ������ ���� �Է½� �Է¹���
		{
			CurrentCombo += 1;
			return true;
		}
		else return false;
	}
	else //�������� �ƴѵ� �Է��� ���� ���
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
void UComboManager::AttackEnd() //���� ���� ���� �κ��� ��������Ʈ�� ó��
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
	//�������� 
	if (bResult)
	{
		for (auto hitResult : hitResults)
		{
			if (hitResult.GetActor())
			{
				FDamageEvent DamageEvent;
				float LastDamage = CurrentAttackInfo.Damage; //���������� ������ ���Ŀ� ���� ���� �� �� �ִ� �κ���
				auto other = Cast<AActor>(hitResult.GetActor());
				UGameplayStatics::ApplyDamage(other, LastDamage, character->GetController(), nullptr, NULL);
			}
		}
	}
}


