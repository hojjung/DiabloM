#pragma once

#include "DiabloM.h"
#include "GameFramework/PlayerController.h"
#include "Widgets/WorldMap/DefaultMenu/DiaGameOverScreen.h"
#include "Widgets/WorldMap/DefaultMenu/MainCanvas.h"
#include "Widgets/WorldMap/DefaultMenu/Shop/DiaShopPanel.h"
#include "Widgets/WorldMap/WorldWidget/DamageTextWidgetComponent.h"

#include "DiabloPlayerController.generated.h"

/**
 * 
 */

class AShopKeeper;
class AUnitPawn;
class UInventory;
struct FItemInstance;
class ADroppedItem;
class UPlayerSkillSystem;
class UEquipmentSystem;
class APlayerDiabloCharacter;


UCLASS()
class DIABLOM_API ADiabloPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADiabloPlayerController();
	
	static  ADiabloPlayerController* Get;

protected:
	UPROPERTY()
	UInventory* m_Inven;
	UPROPERTY()
	UEquipmentSystem* m_EquipSystem;
	UPROPERTY()
	TArray<UInventory*> m_AryStorage;
	UPROPERTY()
	TArray<bool> m_AryStorageOpend;

	FTextFormat m_FormatMiss;
protected:
	UPROPERTY(EditDefaultsOnly,Category="Widget")
	TSubclassOf<UMainCanvas> m_ClassMainMenu; // 태어나게 할 설계도 위젯 ㅇㅇ
	UPROPERTY(EditDefaultsOnly,Category="Widget")
	TSubclassOf<UDiaGameOverScreen> m_ClassGameOver; // 
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UDamageTextWidgetComponent> m_ClassDmgText;
protected:
	UPROPERTY()
	UMainCanvas* m_MainMenu;//실제로 태어난 위젯 메인 메뉴
	UPROPERTY()
	UDiaGameOverScreen* m_GameOverScreen;
	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TArray<UDamageTextWidgetComponent*> m_AryDmgWC;

	int m_DmgIndex;
protected:
	void InitWidget();

	void CreateDmgWC(int count);

	UDamageTextWidgetComponent* GetDmgWC(); 

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void SetupInputComponent() override;

	void ExitGame();

	void OnPlayerDied(AUnitPawn* player);

	void OnPlayerRevived(AUnitPawn* player);

public:
	void PrintStat();

	void PrintInven();

	bool AddItem(FItemInstance itemInst);

	bool PickUpItem(ADroppedItem* pickupItem);


public:
	void OnWidgetOpenClose(bool isOpen);
	
	UFUNCTION(BlueprintCallable)
	void OnDeviceBackKey();
	//
	void ShowDamageNumber(const float local_damage_done,AUnitPawn* unit_pawn,EDamagePopup dmgPopup); //target

	void ShowDamageText(const FString stringWant,AUnitPawn* unit_pawn,EDamagePopup dmgPopup); //target

	UMainCanvas* GetMainCanvas();

	UFUNCTION()
    void BackToSelectMenu();
    
public:
	FORCEINLINE UInventory* GetInven() const
	{
		return  m_Inven;
	}
	FORCEINLINE UEquipmentSystem* GetEquipment() const
	{
		return  m_EquipSystem;
	}

	FORCEINLINE TArray<UInventory*>& GetStorageAry()
	{
		return  m_AryStorage;
	}

	FORCEINLINE TArray<bool>& GetStorageOpenAry()
	{
		return m_AryStorageOpend;
	}

	APlayerDiabloCharacter* GetPlayerPawn();

	void InitPlCtrlAndWidget();

};
