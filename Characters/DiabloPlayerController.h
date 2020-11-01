#pragma once

#include "DiabloM.h"
#include "GameFramework/PlayerController.h"
#include "Widgets/WorldMap/DiaMonsterInfo.h"
#include "Widgets/WorldMap/MainCanvas.h"
#include "Widgets/WorldMap/DefaultMenu/DiaGameOverScreen.h"
#include "Widgets/WorldMap/WorldWidget/DamageTextWidgetComponent.h"

#include "DiabloPlayerController.generated.h"

/**
 * 
 */

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

	FTextFormat m_FormatMiss;
protected:
	UPROPERTY(EditDefaultsOnly,Category="Widget")
	TSubclassOf<UMainCanvas> m_ClassMainMenu; // 태어나게 할 설계도 위젯 ㅇㅇ
	UPROPERTY(EditDefaultsOnly,Category="Widget")
	TSubclassOf<UDiaGameOverScreen> m_ClassGameOver; // 
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	int m_nInvenX;
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	int m_nInvenY;
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
	//able skill tree
	//learned skill tree
	//current SkillSlot
	//Equipment slot
	//키를 눌러서 선택된 슬롯의 함수만 불리게
	//위젯도 만들어서 초기화 시킬것
protected:
	virtual void BeginPlay() override;

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

	void PrintEquipment();

	bool AddItem(FItemInstance itemInst);

	bool PickUpItem(ADroppedItem* pickupItem);

	UFUNCTION(BlueprintCallable)
	void OpenMainMenu();
	UFUNCTION(BlueprintCallable)
	void CloseMainMenu();
	UFUNCTION(BlueprintCallable)
	void OnDeviceBackKey();

	void PlayerMeshChange(int slot,FItemInstance& item);
	//
	//배울수 있는 스킬
	//배운 스킬
	//슬롯에 장착한 스킬
	//
	//인벤토리
	//
	//장비장착슬롯


	//아이템의 인스턴스는 어떻게되는가?

	//아이템의 구성? 아이콘,기본 이름,메쉬,기본공격력?
	//생성할때 옵션과 공격력 방어력 추가
	//이 변수값을 어빌리티로 올려줘야함

public:
	FORCEINLINE UInventory* GetInven() const
	{
		return  m_Inven;
	}
	FORCEINLINE UEquipmentSystem* GetEquipment() const
	{
		return  m_EquipSystem;
	}

	APlayerDiabloCharacter* GetPlayerPawn();

	void ShowDamageNumber(const float local_damage_done,AUnitPawn* unit_pawn,EDamagePopup dmgPopup); //target	
	void HideFocusStatusWidget();
	void ShowFocusStatusWidget(AUnitPawn* unit);
	void UpdateMinimap(UMaterialInterface* mapMat);
};
