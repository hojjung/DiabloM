// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Managers/DgContentManagers/PetDgManager.h"
#include "Widgets/CommonElement/ImageAndText.h"

#include "PetDgBtn.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UPetDgBtn : public UUserWidget
{
	GENERATED_BODY()

	protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImageAndText* m_TxtDgName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnEnter;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImageAndText* m_TxtTicketCost;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImageAndText* m_TxtRewards;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImageAndText* m_TxtLimitTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImageAndText* m_TxtMonsterHealth;

	int m_nIndex;
	public:
	UFUNCTION()
	void EnterDungeon();

	void Init(int index, const FPetDgTableRow* petDg);
};
