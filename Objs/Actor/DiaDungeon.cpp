#include "DiaDungeon.h"
#include "DungeonModelHelper.h"
#include "GridFlowConfig.h"
#include "SceneProviderCommand.h"

FString ADiaDungeon::m_EmitNameEnemy ="Enemy";
FString ADiaDungeon::m_EmitNameStart ="Start";

ADiaDungeon::ADiaDungeon(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}


bool ADiaDungeon::IsMyActor(AActor* actor)
{
    const FName DungeonTag = UDungeonModelHelper::GetDungeonIdTag(this);

    if (actor->ActorHasTag(DungeonTag) || actor->ActorHasTag(UDungeonModelHelper::GenericDungeonIdTag))
    {
        return true;
    }

    return false;
}

void ADiaDungeon::ShuffleSpawnPoints(TArray<FTransform>& aryEmit, int iter) const
{
	int MaxIndex = aryEmit.Num() - 1;
    
	for (int j = 0; j < iter; j++)
	{
		for (int i = 0; i < aryEmit.Num(); i++)
		{
			FTransform Temp = aryEmit[i];
			
			int RandIndex = FMath::RandRange(0, MaxIndex);
			
			aryEmit[i] = aryEmit[RandIndex];
			
			aryEmit[RandIndex] = Temp;
		}
	}
}


 TArray<FTransform>& ADiaDungeon::GetArySpawnPoints() 
{
	TArray<FTransform>& EmitAry = m_MapEmitTransform[ADiaDungeon::m_EmitNameEnemy];
	
    ShuffleSpawnPoints(EmitAry,3);
	
	return EmitAry;
}

FVector ADiaDungeon::GetStartPoint()
{
   return m_MapEmitTransform[ADiaDungeon::m_EmitNameStart][0].GetLocation();
}

