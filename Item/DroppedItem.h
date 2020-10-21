#pragma once

#include "DiabloM.h"
#include "Engine/DataTable.h"
#include "Datas/ItemDataTable.h"
#include "Objs/Actor/CollisionInteract.h"
#include "DroppedItem.generated.h"

UCLASS()
class DIABLOM_API ADroppedItem : public ACollisionInteract
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Item")
	FItemDataHandle m_TableID;
	UPROPERTY(VisibleAnywhere)
	FItemInstance m_ItemInstance;

protected:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void OnItemVisualChange(const FLinearColor& colorW);
public:
	virtual void Interact(AActor* instigator) override;

	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
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
