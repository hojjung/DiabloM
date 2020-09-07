// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "GameFramework/PlayerController.h"

#include "Widgets/WorldMap/MainCanvas.h"

#include "DiabloPlayerController.generated.h"

/**
 * 
 */

class AUnitPawn;
class Inventory;
struct FItemInstance;
class ADroppedItem;
class PlayerSkillSystem;
class EquipmentSystem;
class APlayerDiabloCharacter;


UCLASS()
class DIABLOM_API ADiabloPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADiabloPlayerController();
	 static  ADiabloPlayerController* Get;
protected:
	Inventory* m_Inven;
	PlayerSkillSystem* m_SkillSystem;
	EquipmentSystem* m_EquipSystem;

	
protected:
	UPROPERTY(EditDefaultsOnly,Category="Widget")
	TSubclassOf<UMainCanvas> m_ClassMainMenu; // 태어나게 할 설계도 위젯 ㅇㅇ
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	int m_nInvenX;
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	int m_nInvenY;

protected:
	UPROPERTY()
	UMainCanvas* m_MainMenu;//실제로 태어난 위젯 메인 메뉴

	//able skill tree
	//learned skill tree
	//current SkillSlot
	//Equipment slot
	//키를 눌러서 선택된 슬롯의 함수만 불리게
	//위젯도 만들어서 초기화 시킬것
protected:
	virtual void BeginPlay() override;

	void InitWidget();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void SetupInputComponent() override;

	void ExitGame();

public:
	void MonsterTargetIn(AUnitPawn* targetPawn);

	void MonsterTargetOut(AUnitPawn* targetPawn);

	void PrintStat();

	void PrintInven();

	void PrintEquipment();

	bool AddItem(FItemInstance itemInst);

	bool PickUpItem(ADroppedItem* pickupItem);

	UFUNCTION(BlueprintCallable)
	void OpenMainMenu();
	UFUNCTION(BlueprintCallable)
	void CloseMainMenu();
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
	FORCEINLINE Inventory* GetInven() const
	{
		return  m_Inven;
	}
	FORCEINLINE PlayerSkillSystem* GetSkillSystem() const
	{
		return  m_SkillSystem;
	}
	FORCEINLINE EquipmentSystem* GetEquipment() const
	{
		return  m_EquipSystem;
	}

	APlayerDiabloCharacter* GetUnitPawn();

	
};
