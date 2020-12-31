#pragma once

#include "DiabloM.h"
#include "Engine/DataTable.h"
#include "Datas/ItemDataTable.h"
#include "Objs/Actor/CollisionInteract.h"
#include "DroppedItem.generated.h"

class UBillboardComponent;
UCLASS()
class DIABLOM_API ADroppedItem : public ACollisionInteract
{
	GENERATED_BODY()

public:
	ADroppedItem(const FObjectInitializer& objInit);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Item")
	UBillboardComponent* m_Imposter;
	UPROPERTY(EditAnywhere, Category = "Item")
	FItemDataHandle m_TableID;
	UPROPERTY(EditAnywhere, Category = "Item")
	bool m_bIsDroppedInField;
	UPROPERTY(VisibleAnywhere)
	FItemInstance m_ItemInstance;

	bool m_bIsOverlapAble;

	bool m_bIsDropEnd;

protected:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void OnItemVisualChange(const FLinearColor& colorW);
public:
	virtual void Interact(AActor* instigator) override;

	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	void SetItemVisual(const FItemInstance& ItemData);

	void SetItemInstance(FItemInstance& itemInst);

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

	void DropEnd();

	virtual void ShowAll(bool hasBeenShowed) override ;
	
	virtual void HideAll(bool hasBeenShowed) override ;
};
