#pragma once

#include "ProjectStell.h"
#include "GameFramework/Character.h"
#include "Object/Item.h"
#include "PlayerCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryChangedDelegate, FItemInfoStruct);

UCLASS()
class PROJECTSTELL_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	APlayerCharacter();
protected:
	virtual void BeginPlay() override;
public:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents()override;

//키입력 바인딩 기능 관련 
public:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	void LeftAttack();
	void RightAttack();
	void Evasion();
//애니메이션 관련
public:
	UPROPERTY()
		class UPlayerCharacterAnim* anim;
	class UPlayerCharacterAnim* GetCharacterAnim();
//컨트롤러 관련
	class APlayerCharaterCtrl* PlayerCtrl;
//시점,카메라 관련
public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* springArm;
	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* camera;
protected:
	FVector directionToMove = FVector::ZeroVector;
	FRotator armRotationTo = FRotator::ZeroRotator;
	float armLengthTo = 0.0f;
	float armLengthSpeed = 3.0f;
	float armRotationSpeed = 10.0f;
	void DefaultViewSetting();
//피격 관련
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hit)
		class USoundBase* HitSound;
	virtual float TakeDamage(float DamageAmout,struct FDamageEvent const& DamageEvent,class AController* EventInstigator, AActor* DamageCauser)override;

//무기 관련 
private:
	class AWeapon* leftWeapon;
	class AWeapon* rightWeapon;
	class AItem* ContactedItem = nullptr;

	UFUNCTION(BlueprintCallable)
		void PutOnWeapon(class AWeapon* newWeapon, int hand = 0);
public:
	void Equipment_Left();
	void Equipment_Right();
	UFUNCTION(BlueprintCallable)
		void SetContactedItem(class AItem* Item=nullptr);
	class AWeapon* GetLeftWeapon();
	class AWeapon* GetRightWeapon();

//콤보 관련	
	UPROPERTY(VisibleAnywhere, Category = Combo)
		class UComboManager* Combo;
//스텟관련
	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UPlayerStat* Stat;
//hp회복기능 
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HPRegen)
		float Regeneration = 1.f; //재생량 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HPRegen)
		float HPRegenerationTime = 5.f; //재생 쿨타임 
private:
	float CurRegenerationTime = 0.f; 
	FTimerHandle HPRegenerationTimerHandle;
	void HPRegeneration();

//대쉬 관련
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dash, Meta = (AllowPrivateAccess = true))
		class UAnimMontage* DashMontage = nullptr;
	UPROPERTY(VisibleAnywhere,Category = Dash)
		bool IsDashing;
	UPROPERTY(VisibleAnywhere, Category = Dash)
		int32 DashCount = 2;
	UPROPERTY(VisibleAnywhere,Category = Dash)
		int32 DashCoolTime = 10;
	FTimerHandle DashCoolTimerHandle;
	void DashCoolTimer();


//사망 관련
private:
	void KillPlayer();
	FTimerHandle CharacterDstroyTimerHandle;
	float CharacterDstroyCoolTime = 0;
	void CharacterDestroyTimer();
//아이템 관련
private:
	TMap<int32,FItemInfoStruct>Inventory;
	void LoadInvenData(); //저장된 게임의 인벤 정보를 불러와 인벤을 초기화하는 함수
	bool AddItem(FItemInfoStruct info); //인벤에 아이템을 추가하는 함수 -> 중복 아이템 검사, 수량만 증가되도록 
public:
	FOnInventoryChangedDelegate OnInventoryChanged;
	FItemInfoStruct* GetItem(int32 ID); //인벤에 해당 아이템이 있는지 체크후 데이터에 접근
	TMap<int32, FItemInfoStruct> GetInventory();
	UFUNCTION(BlueprintCallable)
		AWeapon* ItemAcquisition(FItemInfoStruct info); //인게임에서 아이템 액터에 닿았을 때 호출될 함수 ,현재 인벤 상태를 체크해서 먹을수 있으면 추가
	//인벤에 대한 변경값이 생길경우에 대한 델리게이트 필요할듯?
};
