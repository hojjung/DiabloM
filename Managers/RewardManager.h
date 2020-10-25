// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "CActionBaseInterface.h"
#include "UObject/NoExportTypes.h"
#include "RewardManager.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API URewardManager : public UObject
{
	GENERATED_BODY()
	//아이템 액터 풀링 필요함 렉걸림
public:
	void RequestMonsterDropItem(int level); // monster drop table Data Need
protected:
	void BezierCurveMove(AActor* target,float height,FVector destination,FOnEnd* endCallback=nullptr);

	FVector GetQuadControlPoint(FVector start,FVector end,float height);
};
