// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharacterAnim.h"
#include "Weapon/Weapon.h"
#include "DrawDebugHelpers.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	//������Ʈ ����
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

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
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Skin(TEXT("SkeletalMesh'/Game/Frank_RPG_Warrior/Skeletal_Meshes/SK_MannequinF.SK_MannequinF'"));
	if (Skin.Succeeded())
		GetMesh()->SetSkeletalMesh(Skin.Object);
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("AnimBlueprint'/Game/1_Animation/ABP/PlayerABP.PlayerABP_C'"));
	if (AnimBP.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimBP.Class);

	//�ʱ� �����ʿ� �Լ�, ���� 
	SetViewMode();
}
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	springArm->TargetArmLength = FMath::FInterpTo(springArm->TargetArmLength, armLengthTo, DeltaTime, armLengthSpeed);
	if (directionToMove.SizeSquared() > 0.0f)
	{
		GetController()->SetControlRotation(FRotationMatrix::MakeFromX(directionToMove).Rotator());
		AddMovementInput(directionToMove);
	}
}
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//�÷��̾� Ű�Է� ���ε� 
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("TurnRight"), this, &APlayerCharacter::Turn); //���콺 �¿� �̵�
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerCharacter::LookUp); //���콺 ���� �̵� 
	//PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ABase_Character::Jump);
	PlayerInputComponent->BindAction(TEXT("Evasion"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Evasion);
	PlayerInputComponent->BindAction(TEXT("LeftAttack"), EInputEvent::IE_Pressed, this, &APlayerCharacter::LeftAttack);
	PlayerInputComponent->BindAction(TEXT("RightAttack"), EInputEvent::IE_Pressed, this, &APlayerCharacter::RightAttack);
}
void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	anim = Cast<UPlayerCharacterAnim>(GetMesh()->GetAnimInstance());
	if (anim == nullptr) return;
	anim->OnMontageEnded.AddDynamic(this, &APlayerCharacter::OnMontageEnded);
	//anim->OnMontageFeasibleCheck.BindUObject();
}
void APlayerCharacter::UpDown(float NewAxisValue)
{
	directionToMove.X = NewAxisValue;
}
void APlayerCharacter::LeftRight(float NewAxisValue)
{
	directionToMove.Y = NewAxisValue;
}
void APlayerCharacter::LookUp(float NewAxisValue)
{
}
void APlayerCharacter::Turn(float NewAxisValue)
{
}
void APlayerCharacter::LeftAttack()
{
	anim->SetMirror(true);
	leftWeapon->Action(EWeaponActionType::Attack);
}
void APlayerCharacter::RightAttack()
{
	anim->SetMirror(false);
	rightWeapon->Action(EWeaponActionType::Attack);
}
void APlayerCharacter::Evasion()
{
	//Weapon->Attack(EAttackType::RightMouse);
}
void APlayerCharacter::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// hit �� ��Ÿ ĳ���� ȥ�ڸ��� ������ ��Ÿ�ַ� ǥ���Ǵ°�� �� ��Ÿ�� ���� ���� �ۼ��κ�
}
UPlayerCharacterAnim* APlayerCharacter::GetCharacterAnim()
{
	return anim;
}
void APlayerCharacter::SetViewMode()
{
	armLengthTo = 800.0f;// == springArm->TargetArmLength = 800.0f;
	armRotationTo = FRotator(-45.0f, 0.0f, 0.0f);// == springArm->SetRelativeRotation(FRotator(-45.0f,0.0f,0.0f));
	springArm->bUsePawnControlRotation = false;
	springArm->bInheritPitch = false;
	springArm->bInheritRoll = false;
	springArm->bInheritYaw = false;
	springArm->bDoCollisionTest = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
}
void APlayerCharacter::PutOnWeapon(FName path, int hand)
{
	if (hand == 0)
	{
		if (path.IsNone())
		{
			leftWeapon = GetWorld()->SpawnActor<AWeapon>(AWeapon::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}
		else
		{
			UClass* BP = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *path.ToString()));
			leftWeapon = GetWorld()->SpawnActor<AWeapon>(BP, FVector::ZeroVector, FRotator::ZeroRotator);
		}
		leftWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_rSocket"));
		leftWeapon->SetOwner(this);
		leftWeapon->InitWeapon();
	}
	else if (hand == 1)
	{
		if (path.IsNone())
		{
			rightWeapon = GetWorld()->SpawnActor<AWeapon>(AWeapon::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}
		else
		{
			UClass* BP = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *path.ToString()));
			rightWeapon = GetWorld()->SpawnActor<AWeapon>(BP, FVector::ZeroVector, FRotator::ZeroRotator);
		}
		rightWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_lSocket"));
		rightWeapon->SetOwner(this);
		rightWeapon->InitWeapon();
	}
}

