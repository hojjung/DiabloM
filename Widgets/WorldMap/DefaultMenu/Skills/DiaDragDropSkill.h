// My First Hack n Slash

#pragma once

#include "DiabloM.h"


#include "Image.h"
#include "SizeBox.h"
#include "Blueprint/DragDropOperation.h"
#include "Datas/SkillDataTable.h"

#include "DiaDragDropSkill.generated.h"

/**
 * 
 */
class USkillLearnButton;
UCLASS()
class DIABLOM_API UDiaDragDropSkill : public UDragDropOperation
{
	GENERATED_BODY()

public:
	static UDiaDragDropSkill* GetDDOInst;

	FSkillDataSpec* m_DraggedSkillData;
	UPROPERTY()
	USkillLearnButton* m_SkillLearnWidget;
public:
	void SetDDO(FSkillDataSpec* skillSpec,USkillLearnButton* skillLearnBtn);
	UFUNCTION()
	void OnDragCancel(UDragDropOperation* meSelf);
	UFUNCTION()
	void OnDrop(UDragDropOperation* meSelf);

	friend USkillLearnButton;
};
