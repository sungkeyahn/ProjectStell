// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacterAnim.h"
#include "Weapon/Weapon.h"

UPlayerCharacterAnim::UPlayerCharacterAnim()
{
	CurrentPawnSpeed = 0.0f;
	IsDead = false;
}
void UPlayerCharacterAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	auto pawn = TryGetPawnOwner();
	if (!::IsValid(pawn)) return;
	if (!IsDead)
	{
		CurrentPawnSpeed = pawn->GetVelocity().Size();
	}
}
void UPlayerCharacterAnim::AnimNotify_ComboCheck()
{
	OnNextAttackComboCheck.Broadcast();
}
void UPlayerCharacterAnim::AnimNotify_HitCheck()
{
	OnAttackHitCheck.Broadcast();
}
void UPlayerCharacterAnim::PlayPlayerMontage(UAnimMontage* montage, float PalySpeed)
{
	if (OnMontageFeasibleCheck.Execute(montage))
		Montage_Play(montage, PalySpeed);
}
void UPlayerCharacterAnim::NextSectionPlay_PlayerMontage(UAnimMontage* montage, int32 nextSectionNum)
{
	if (Montage_IsPlaying(montage))
		if (OnMontageFeasibleCheck.Execute(montage))
			Montage_JumpToSection(montage->GetSectionName(nextSectionNum - 1), montage);
}


