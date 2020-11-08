// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"


#include "SaveLoadManager.h"
#include "Datas/ItemDataTable.h"
#include "GameFramework/SaveGame.h"
#include "SaveStorage.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API USaveStorage : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TArray<bool> m_AryIsStorageOpened;//10000
	UPROPERTY(EditAnywhere)
	ESaveVersion m_SaveVersion;
	//어떤 아이템이, 어떤 인벤칸에 있는지, 그리고 옵션의 종류와 수치,등급
	UPROPERTY(EditAnywhere)
	TArray<FItemInstance> m_AryStorageItems1;
	UPROPERTY(EditAnywhere)
	TArray<FItemInstance> m_AryStorageItems2;
	UPROPERTY(EditAnywhere)
	TArray<FItemInstance> m_AryStorageItems3;
	UPROPERTY(EditAnywhere)
	TArray<FItemInstance> m_AryStorageItems4;
	UPROPERTY(EditAnywhere)
	TArray<FItemInstance> m_AryStorageItems5;
public:
	void SetSaveStorage(ESaveVersion saveV,const TArray<bool>& aryOpen,const TArray<TArray<FItemInstance>>& aryItems);
	
};
