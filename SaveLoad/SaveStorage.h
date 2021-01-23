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
	ESaveVersion m_SaveVersion;
	UPROPERTY(EditAnywhere)
	bool m_bShareStorage1Opened;//100,
	UPROPERTY(EditAnywhere)
	bool m_bShareStorage2Opened;//100,
	UPROPERTY(EditAnywhere)
	bool m_bShareStorage3Opened;//100,
	UPROPERTY(EditAnywhere)
	TArray<FItemInstance> m_AryStorageItems1;
	UPROPERTY(EditAnywhere)
	TArray<FItemInstance> m_AryStorageItems2;
	UPROPERTY(EditAnywhere)
	TArray<FItemInstance> m_AryStorageItems3;

public:
	void SetSaveStorage(ESaveVersion saveV,const TArray<bool>& aryOpen,const TArray<UInventory*>& aryItems);
	
};
