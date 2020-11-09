// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Datas/ShopItemTable.h"
#include "Item/Inventory.h"
#include "Village/VillageActor.h"
#include "ShopKeeper.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API AShopKeeper : public AVillageActor
{
	GENERATED_BODY()
public:
	AShopKeeper();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* m_MeshShopKeeper;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FShopItemSellHandle m_ShopItemSellHandle1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FShopItemSellHandle m_ShopItemSellHandle2;

protected:
	UPROPERTY()
	TArray<UInventory*> m_AryShopItemContainer;

protected:
	void SetItemFromDataTable1();

	void SetItemFromDataTable2();

	//void SetItemFromDataTable3();
	
public:
	virtual void BeginPlay() override;
	
	virtual void Interact(AActor* instigator) override;

	FORCEINLINE TArray<UInventory*>* GetShopItemContainer()
	{
		return &m_AryShopItemContainer;
	}
};
