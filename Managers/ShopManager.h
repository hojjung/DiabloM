// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "UObject/NoExportTypes.h"
#include "ShopManager.generated.h"

/**
 * 
 */


class UGachaCardGridPanel;
class UPlayFabJsonObject;
class UGachaPanel;
UCLASS()
class DIABLOM_API UShopManager : public UObject
{
	GENERATED_BODY()
public:
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnUpdateGold,const BigInt&,const BigInt&,const BigInt&);
	DECLARE_MULTICAST_DELEGATE(FOnItemPurchased);
	

	FOnItemPurchased m_OnItemPurchased;
	
	FOnUpdateGold m_OnUpdateShopGold;

	UPROPERTY()
	UGachaPanel* m_GachaPanel;
	

public:
	void SetShopDataFromServer(const UPlayFabJsonObject* iapJsonStr);
	
protected:
	TArray<bool> m_PackagePurchased;
	
	BigInt m_Gold01;
	
	BigInt m_Gold02;
	
	BigInt m_Gold03;

	bool m_bUsingAutoSkill;

public:
	UFUNCTION()
	void PurchasePacakge01();
	UFUNCTION()
	void PurchasePacakge02();
	UFUNCTION()
	void PurchasePacakge03();
	UFUNCTION()
	void PurchasePacakge04();
	UFUNCTION()
	void PurchasePacakge05();
	UFUNCTION()
	void PurchaseGemStone01();
	UFUNCTION()
	void PurchaseGemStone02();
	UFUNCTION()
	void PurchaseGemStone03();
	UFUNCTION()
	void PurchaseGemStone04();
	UFUNCTION()
	void PurchaseGemStone05();
	UFUNCTION()
	void PurchaseGemStone06();
	UFUNCTION()
	void PurchaseGold01();
	UFUNCTION()
	void PurchaseGold02();
	UFUNCTION()
	void PurchaseGold03();
	UFUNCTION()
	void PurchaseGachaWeapon01();
UFUNCTION()
	void PurchaseGachaWeapon11();
	UFUNCTION()
	void PurchaseGachaWeapon55();
	UFUNCTION()
	void PurchaseGachaSkin01();
	UFUNCTION()
	void PurchaseGachaSkin11();
	UFUNCTION()
	void PurchaseGachaSkin55();
	UFUNCTION()
	void PurchaseGachaPet01();
	UFUNCTION()
	void PurchaseGachaPet11();
	UFUNCTION()
	void PurchaseGachaPet55();
	UFUNCTION()
	void PurchaseGachaAccessory01();
	UFUNCTION()
	void PurchaseGachaAccessory11();
	UFUNCTION()
	void PurchaseGachaAccessory55();
	UFUNCTION()
	void UpdateGold();
	UFUNCTION()
	void RollGachaOneTime();
	UFUNCTION()
    void RollGachaElevenTime();
	UFUNCTION()
    void RollGachaFiftyTime();

	void OnPurchasedGainItem(FString itemID,bool updateData);

	

	bool GetPackagePurchased(int index);

	void SetIAPDataToJson(UPlayFabJsonObject*);

	FORCEINLINE const TArray<bool>& GetPackageBoughtAry() const
	{
		return m_PackagePurchased;
	}

	void ShowTouchBan();
	
	bool IsAbleToUseAutoSkill();

	void SetUseAutoSkill(bool bUse);

	FORCEINLINE bool GetUseAutoSkill()
	{
		return m_bUsingAutoSkill;
	}
};


