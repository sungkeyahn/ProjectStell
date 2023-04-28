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
	//컴포넌트 생성
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Stat = CreateDefaultSubobject<UPlayerStat>(TEXT("Stat"));
	Combo = CreateDefaultSubobject<UComboManager>(TEXT("Combo"));
	
	//컴포넌트 계층구조 설정
	springArm->SetupAttachment(GetCapsuleComponent());
	camera->SetupAttachment(springArm);

	//컴포넌트 초기값 설정
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);
	GetCapsuleComponent()->SetCapsuleRadius(34.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerCharacter"));
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	GetCharacterMovement()->JumpZVelocity = 800.0f;
	springArm->SetRelativeRotation(FRotator::ZeroRotator);
	springArm->TargetArmLength = 400.0f;

	//애셋 바인딩
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
	PlayerInputComponent->BindAxis(TEXT("TurnRight"), this, &APlayerCharacter::Turn); //마우스 좌우 이동
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerCharacter::LookUp); //마우스 상하 이동 
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
void APlayerCharacter::PutOnWeapon(FName path, int hand) //매개 변수를 아이템으로 바꿀 예정
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
		//게임 저장과 사망 UI띄우고 게임 멈추기 기능 필요
		GetWorldTimerManager().ClearTimer(CharacterDstroyTimerHandle);
		auto ps = Cast<APlayerCharacterState>(GetPlayerState());
		if (nullptr != ps) ps->AddDeadCount();
		//아래 코드들은 한번만 실행해야함 타이머 코드 추가할 때 조심
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
{//이 함수에서는 인벤이 아이템을 습득 가능한 상태인지를 검사
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

	/*여기서 인벤UI에 현재 먹은 아이템의 정보를 넣는 작업이 필요
	* 컨트롤러의 인벤 UI를 건들여서 UPdateInvenUI()를 실행시키는 형식으로 구성*/
	
	
	
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
{//이 함수에서는 정상적으로 월드에 존재하는 아이템 인지를 검사
	if (AddItem(info)) return true;
	else return false;
}
void APlayerCharacter::KillPlayer()
{
	Stat->SetDamage(100.f);
}
