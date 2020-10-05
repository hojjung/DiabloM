#pragma once

#include "DiabloM.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Datas/ItemDataTable.h"
#include "Objs/Interfaces/Interactable.h"
#include "DroppedItem.generated.h"

UCLASS()
class DIABLOM_API ADroppedItem : public AActor,public IInteractable
{
	GENERATED_BODY()
	
public:
	ADroppedItem(const FObjectInitializer& objInit);

protected:
	UPROPERTY(EditAnywhere, Category = "Item")
	FItemDataHandle m_TableID;
	UPROPERTY(VisibleAnywhere, Category = "Item")
	UStaticMeshComponent* m_MeshComp;
	UPROPERTY(VisibleAnywhere, Category = "Item")
	USphereComponent* m_CollSphere;
	UPROPERTY(VisibleAnywhere, Category = "Item")
	UWidgetComponent* m_BillBoard;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnItemVisualChange(const FLinearColor& colorW);
protected:
	UPROPERTY(VisibleAnywhere)
	FItemInstance m_ItemInstance;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void PickupItem(AActor* interactCaster);

public:
	virtual void Interact(AActor* instigator) override;
	
	void SetItemVisual(const FItemInstance& ItemData);

	void SetItem(FName itemID);

	void SetItemInstance(FItemInstance& itemInst);

	UFUNCTION(BlueprintCallable)
	const FItemInstance& GetCurrentItem()const;

	UFUNCTION(BlueprintCallable)
	const FItemData& GetCurrentItemData() const
	{
		return *(GetCurrentItem().m_ItemData);
	}
	UFUNCTION(BlueprintCallable)
	const FLinearColor& GetItemColor()
	{
		return GetCurrentItem().m_ItemTier->m_TierColor;
	}
};
