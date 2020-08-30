// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Datas/ItemDataTable.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Widgets/DiaInvenGridPanel.h"
#include "Widgets/DiaInvenGridSlot.h"
#include "DiaDragDrop.generated.h"

/**
 *드래그 시작시
 드래그 시작한 슬롯을 반투명하게
 실패시 원래 자리로 돌아감
 성공시 아이템 옮김
 성공 및 교체시 내자리로 이전 아이템 옮김

 휴지통에 드랍할경우, 땅에 버림
 캡슐화 x
 */


UCLASS()
class DIABLOM_API UDiaDragDrop : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	USizeBox* m_MainSizeBox;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_IconImage;
	UPROPERTY()
	UDiaInvenGridPanel* m_PreGrid;
	UPROPERTY()
	UDiaInvenGridSlot* m_PreSlot;

public:
	int m_nPreIndex;

	FItemInstance m_DraggedItem;

public:
	void SetDDO(const FItemInstance& itemInst);

	void InitDrag(UDiaInvenGridSlot* preSlot);//slot turn blur

public:
	UFUNCTION()
	void OnDragCancel(UDragDropOperation* meSelf);
	UFUNCTION()
	void OnDrop(UDragDropOperation* meSelf);
};
