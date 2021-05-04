
#include "MagicStoneDgManager.h"
#include "DiabloGameInstance.h"
#include "Kismet/GameplayStatics.h"

UMagicStoneDgManager::UMagicStoneDgManager()
{
	m_nDgLevel=0;
}

void UMagicStoneDgManager::RequestMoveMagicStoneDg(int dgLevel)
{
	m_nDgLevel = dgLevel;
	
	UGameplayStatics::OpenLevel(UDiabloGameInstance::Get->GetWorld(),TEXT("MagicDragonNest"), true);

	m_LevelLoadHandle = FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this,&UMagicStoneDgManager::OnLevelLoaded);
	
}

void UMagicStoneDgManager::OnLevelLoaded(UWorld* world)
{
	FCoreUObjectDelegates::PostLoadMapWithWorld.Remove(m_LevelLoadHandle);
	PRINTF("MagicStoneManager! World:%s",*world->GetMapName());
	
}

void UMagicStoneDgManager::Tick(float delta_seconds)
{
	
}
