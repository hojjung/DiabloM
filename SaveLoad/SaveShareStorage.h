// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"


#include "SaveLoadManager.h"
#include "Datas/ItemDataTable.h"
#include "GameFramework/SaveGame.h"
#include "SaveShareStorage.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API USaveShareStorage : public USaveGame
{
	GENERATED_BODY()//All Character Use This Storage
public:
	UPROPERTY(EditAnywhere)
	bool m_bShareStorage4Opened;//Max 2
	UPROPERTY(EditAnywhere)
	bool m_bShareStorage5Opened;//Max 2
	UPROPERTY(EditAnywhere)
	ESaveVersion m_SaveVersion;
	UPROPERTY(EditAnywhere)
	TArray<FItemInstance> m_AryStorageItems4;
	UPROPERTY(EditAnywhere)
	TArray<FItemInstance> m_AryStorageItems5;
	
public:
	void SetSaveShareStorage(ESaveVersion saveV,const TArray<bool>& aryOpen34,const TArray<UInventory*>& aryItems);
};
