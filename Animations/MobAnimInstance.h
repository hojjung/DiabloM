#pragma once
#include "DiabloM.h"
#include "AnimInstanceProxy.h"
#include "Animation/AnimInstance.h"
#include "Characters/Pawns/UnitPawn.h"

#include "MobAnimInstance.generated.h"


class UMobAnimInstance;
USTRUCT(BlueprintType)
struct FMobAnimInstanceProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()
	
public:
	virtual void InitializeObjects(UAnimInstance* InAnimInstance) override;

	virtual void Update(float DeltaSeconds) override;

	UPROPERTY(Transient)
	UMobAnimInstance* m_MobAnim=nullptr;
};

class AMonsterPawn;

UCLASS()
class DIABLOM_API UMobAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Transient, BlueprintReadOnly,meta = (AllowPrivateAccess = "true"))
	FMobAnimInstanceProxy m_Proxy;
	UPROPERTY(Transient,VisibleAnywhere,BlueprintReadWrite)
	bool m_bIsMoving;
	UPROPERTY()
	AUnitPawn* m_Owner;
	
public:
	virtual void NativeBeginPlay() override;
	
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override
	{
		return &m_Proxy;
	}
	
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override {}

	void UpdateMoveFlag();
	
};
