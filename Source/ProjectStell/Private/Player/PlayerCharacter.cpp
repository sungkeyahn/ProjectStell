// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharacterAnim.h"
#include "Player/PlayerCharaterCtrl.h"
#include "Weapon/Weapon.h"
#include "Stat/PlayerStat.h"
#include "Player/ComboManager.h"
#include "Player/PlayerCharacterState.h"
#include "UI/CharacterHUDWidget.h"
#include "UI/InventoryWidget.h"
#include "DrawDebugHelpers.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	//������Ʈ ����
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Stat = CreateDefaultSubobject<UPlayerStat>(TEXT("Stat"));
	Combo = CreateDefaultSubobject<UComboManager>(TEXT("Combo"));
	
	//������Ʈ �������� ����
	springArm->SetupAttachment(GetCapsuleComponent());
	camera->SetupAttachment(springArm);

	//������Ʈ �ʱⰪ ����
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);
	GetCapsuleComponent()->SetCapsuleRadius(34.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerCharacter"));
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	GetCharacterMovement()->JumpZVelocity = 800.0f;
	springArm->SetRelativeRotation(FRotator::ZeroRotator);
	springArm->TargetArmLength = 400.0f;

	//�ּ� ���ε�
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Skin(TEXT("SkeletalMesh'/Game/MercenaryWarrior/Meshes/SK_MercenaryWarrior_WithHelmet.SK_MercenaryWarrior_WithHelmet'"));
	if (Skin.Succeeded())
		GetMesh()->SetSkeletalMesh(Skin.Object);
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("AnimBlueprint'/Game/1_Player/Character/PlayerABP.PlayerABP_C'"));
	if (AnimBP.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimBP.Class);
	DefaultViewSetting();
}
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	PlayerCtrl = Cast<APlayerCharaterCtrl>(GetController());
	if (nullptr == PlayerCtrl)return;
	//DisableInput(PlayerController);
	PlayerCtrl->GetHUDWidget()->BindCharacterStat(Stat);
	PlayerCtrl->GetInventoryWidget()->BindCharacterInventory(this);
}
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	springArm->TargetArmLength = FMath::FInterpTo(springArm->TargetArmLength, armLengthTo, DeltaTime, armLengthSpeed);
	if (directionToMove.SizeSquared() > 0.0f)
	{
		if (anim->IsAnyMontagePlaying() == false)
		{
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(directionToMove).Rotator());
			AddMovementInput(directionToMove);
		}
	}
}
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("TurnRight"), this, &APlayerCharacter::Turn); //���콺 �¿� �̵�
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerCharacter::LookUp); //���콺 ���� �̵� 
	//PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ABase_Character::Jump);
	PlayerInputComponent->BindAction(TEXT("Evasion"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Evasion);
	PlayerInputComponent->BindAction(TEXT("LeftAttack"), EInputEvent::IE_Pressed, this, &APlayerCharacter::LeftAttack);
	PlayerInputComponent->BindAction(TEXT("RightAttack"), EInputEvent::IE_Pressed, this, &APlayerCharacter::RightAttack);
	PlayerInputComponent->BindAction(TEXT("KillPlayer"), EInputEvent::IE_Pressed, this, &APlayerCharacter::KillPlayer);
}
void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	anim = Cast<UPlayerCharacterAnim>(GetMesh()->GetAnimInstance());
	if (anim == nullptr) return;
	anim->OnDashStart.AddLambda
	([this]()->void
	{
		IsDashing = true;
		GetCharacterMovement()->BrakingFrictionFactor = 0.f;
		SetCanBeDamaged(false);
	}
	);
	anim->OnDashEnd.AddLambda
	([this]()->void
	{
		IsDashing = false;
		GetCharacterMovement()->BrakingFrictionFactor = 2.f;
		SetCanBeDamaged(true);
	}
	);
	Stat->OnHpIsZero.AddLambda
	([this]()->void
	{
	anim->SetDeadAnim();
	SetActorEnableCollision(false);
	GetWorldTimerManager().SetTimer(CharacterDstroyTimerHandle, this, &APlayerCharacter::CharacterDestroyTimer, 1.0f, true);
	}
	);
	Combo->InitComboManager();
}

void APlayerCharacter::UpDown(float NewAxisValue)
{
	directionToMove.X = NewAxisValue;
	//AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
}
void APlayerCharacter::LeftRight(float NewAxisValue)
{
	directionToMove.Y = NewAxisValue;
	//AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
}
void APlayerCharacter::LookUp(float NewAxisValue)
{
	//AddControllerPitchInput(NewAxisValue * 45.f * GetWorld()->GetDeltaSeconds());
}
void APlayerCharacter::Turn(float NewAxisValue)
{
	//AddControllerYawInput(NewAxisValue * 45.f * GetWorld()->GetDeltaSeconds());
}
void APlayerCharacter::LeftAttack()
{
	Combo->Attack(true);
}
void APlayerCharacter::RightAttack()
{
	Combo->Attack(false);
}
void APlayerCharacter::Evasion()
{
	//anim->IsAnyMontagePlaying()||
	if (IsDashing || Stat->GetSpRatio() < 0.5f) return;
	Stat->UseStamina(50);
	GetWorldTimerManager().SetTimer(DashCoolTimerHandle, this, &APlayerCharacter::DashCoolTimer, 1.0f, true);
	const int32 FB = directionToMove.X;
	const int32 RL = directionToMove.Y;
	FRotator Rotation = GetActorRotation();
	if (RL == 0 && FB > 0)
		Rotation = FRotator(0, 0, 0);
	else if (RL == 0 && FB < 0)
		Rotation = FRotator(0, 180, 0);
	else if (FB == 0 && RL > 0)
		Rotation = FRotator(0, 90, 0);
	else if (FB == 0 && RL < 0)
		Rotation = FRotator(0, -90, 0);
	else if (FB > 0 && RL > 0) //wd
		Rotation = FRotator(0, 45, 0);
	else if (FB > 0 && RL < 0) //wa
		Rotation = FRotator(0, -45, 0);
	else if (FB < 0 && RL < 0) //sa
		Rotation = FRotator(0, -135, 0);
	else if (FB < 0 && RL > 0) //sd
		Rotation = FRotator(0, 135, 0);
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	LaunchCharacter(Direction * 2000.f, true, true);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(Direction).Rotator());
	anim->PlayPlayerMontage(DashMontage);
	Combo->AttackReset();
}

UPlayerCharacterAnim* APlayerCharacter::GetCharacterAnim()
{
	return anim;
}
AWeapon* APlayerCharacter::GetLeftWeapon()
{
	return leftWeapon;
}
AWeapon* APlayerCharacter::GetRightWeapon()
{
	return rightWeapon;
}

void APlayerCharacter::DefaultViewSetting()
{
	armLengthTo = 400.0f;// == springArm->TargetArmLength = 800.0f;
	armRotationTo = FRotator(-45.0f, 0.0f, 0.0f);// == springArm->SetRelativeRotation(FRotator(-45.0f,0.0f,0.0f));
	springArm->bUsePawnControlRotation = false;
	springArm->bInheritPitch = false;
	springArm->bInheritRoll = false;
	springArm->bInheritYaw = false;
	springArm->bDoCollisionTest = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);//540.f , 720.f
}
float APlayerCharacter::TakeDamage(float DamageAmout, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float FinalDamage = Super::TakeDamage(DamageAmout, DamageEvent, EventInstigator, DamageCauser);
	Stat->SetDamage(FinalDamage);
	return FinalDamage;
}
void APlayerCharacter::PutOnWeapon(FName path, int hand) //�Ű� ������ ���������� �ٲ� ����
{
	//rightWeapon = GetWorld()->SpawnActor<AWeapon>(AWeapon::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
	//NULLWeapon
	UClass* BP = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *path.ToString()));
	if (hand == 0)
	{
		if (path.IsNone())
			leftWeapon = GetWorld()->SpawnActor<AWeapon>();
		else
			leftWeapon = GetWorld()->SpawnActor<AWeapon>(BP, FVector::ZeroVector, FRotator::ZeroRotator);
		leftWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_lSocket"));
		leftWeapon->SetOwner(this);
	}
	else 
	{
		if (path.IsNone())
			rightWeapon = GetWorld()->SpawnActor<AWeapon>();
		else
			rightWeapon = GetWorld()->SpawnActor<AWeapon>(BP, FVector::ZeroVector, FRotator::ZeroRotator);
		rightWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_rSocket"));
		rightWeapon->SetOwner(this);
	}
}
void APlayerCharacter::DashCoolTimer()
{
	if (Stat->GetSpRatio() >= 1.0f)
		GetWorldTimerManager().ClearTimer(DashCoolTimerHandle);

	Stat->UseStamina(-5);
	IsDashing = false;
	GetCharacterMovement()->BrakingFrictionFactor = 2.f;
	SetCanBeDamaged(true);
}
void APlayerCharacter::CharacterDestroyTimer()
{
	++CharacterDstroyCoolTime;
	if (CharacterDstroyCoolTime > 3.f)
	{
		//���� ����� ��� UI���� ���� ���߱� ��� �ʿ�
		GetWorldTimerManager().ClearTimer(CharacterDstroyTimerHandle);
		auto ps = Cast<APlayerCharacterState>(GetPlayerState());
		if (nullptr != ps) ps->AddDeadCount();
		//�Ʒ� �ڵ���� �ѹ��� �����ؾ��� Ÿ�̸� �ڵ� �߰��� �� ����
		leftWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		leftWeapon->Destroy();
		leftWeapon = nullptr;
		rightWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		rightWeapon->Destroy();
		rightWeapon = nullptr;
		SetActorHiddenInGame(true);
	}
}
void APlayerCharacter::LoadInvenData()
{
}
bool APlayerCharacter::AddItem(FItemInfoStruct info)
{//�� �Լ������� �κ��� �������� ���� ������ ���������� �˻�
	bool result =true;
	if (Inventory.Find(info.ID))
	{
		Inventory.Find(info.ID)->Quantity += info.Quantity;
		OnInventoryChanged.Broadcast(info);
	}
	else if (info.ID > 0)
	{
		Inventory.Add(info.ID, info);
		OnInventoryChanged.Broadcast(info);
	}
	else
		result = false;

	/*���⼭ �κ�UI�� ���� ���� �������� ������ �ִ� �۾��� �ʿ�
	* ��Ʈ�ѷ��� �κ� UI�� �ǵ鿩�� UPdateInvenUI()�� �����Ű�� �������� ����*/
	
	
	
	GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, FString::Printf(TEXT("Add Item %d"), Inventory.Find(info.ID)->Quantity));
	return result;
}
FItemInfoStruct* APlayerCharacter::GetItem(int32 ID)
{
	return Inventory.Find(ID);
}
TMap<int32, FItemInfoStruct> APlayerCharacter::GetInventory()
{
	return Inventory;
}
bool APlayerCharacter::ItemAcquisition(FItemInfoStruct info)
{//�� �Լ������� ���������� ���忡 �����ϴ� ������ ������ �˻�
	if (AddItem(info)) return true;
	else return false;
}
void APlayerCharacter::KillPlayer()
{
	Stat->SetDamage(100.f);
}
