// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectStell.h"
#include "GameFramework/Actor.h"
#include "TransparencyWall.generated.h"
/*
1. 카메라에서 캐릭터로 레이저 쏨 or 캐릭터 와 카메라 사이에 콜리전을 배치
2. 만약 카메라에서 캐릭터로 쏘는 레이저에 "벽(투명화가능액터)"이라는 물채가 들오면 
3. 해당 벽의 투명화 기능을 활성화 
*/
UCLASS()
class PROJECTSTELL_API ATransparencyWall : public AActor
{
	GENERATED_BODY()
public:	
	ATransparencyWall();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
        class UStaticMeshComponent* Mesh;
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
        float TransparentTime;
    bool isTransparent;
    float RunningTimer; //진행도 
public:
    UFUNCTION(BlueprintCallable)
    void SetTransparent(bool Transparent);
};
