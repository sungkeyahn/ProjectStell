// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "GameFramework/Actor.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemType :uint8
{
	None = 0, Weapon, Consumer, ETC
};
USTRUCT(Atomic, BlueprintType)
struct FItemInfoStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemInfo)
		int32 ID = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemInfo)
		TSubclassOf<class AActor> ItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemInfo)
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemInfo)
		EItemType Type = EItemType::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemInfo)
		int32 Quantity = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemInfo)
		UTexture2D* Icon = nullptr;

};

UCLASS()
class PROJECTSTELL_API AItem : public AActor
{
	GENERATED_BODY()
public:	
	AItem();
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh, Meta = (AllowPrivateAccess = true))
		class USkeletalMeshComponent* mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item, Meta = (AllowPrivateAccess = true))
		FItemInfoStruct info;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = box, Meta = (AllowPrivateAccess = true))
		UBoxComponent* box;
	UPROPERTY(VisibleAnywhere)
		URotatingMovementComponent* movement;
public: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
		float reSpawnTime;
		FItemInfoStruct GetItemInfo();
		void Acquiring_Item();
		FTimerHandle ItemReSpawnTimmerHandle;
private:
	void ReSpawnItemTimmer();
	float CurSpawnTime;
		/*플레이어가 아이템에 접촉해서 장착 버튼을 누르면 호출될 
		* 아이템 사라지게 하기 함수 와 자동으로 다시 나타나게 할 함수 델리게이트로 바인딩하기 언바인딩하기 
		* 
		*/

};
