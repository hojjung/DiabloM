#pragma once

#include "DiabloM.h"
#include "GameFramework/PlayerController.h"
#include "Widgets/CommonElement/DamageTextWidgetComponent.h"

#include "DiabloPlayerController.generated.h"

/**
 * 
 */


UCLASS()
class DIABLOM_API ADiabloPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADiabloPlayerController();
	
protected:
	FTextFormat m_FormatMiss;
protected:
	UPROPERTY()
	TArray<UDamageTextWidgetComponent*> m_AryDmgWC;

	int m_DmgIndex;
protected:
	void InitWidget();

	void CreateDmgWC(int count);

	UDamageTextWidgetComponent* GetDmgWC(); 

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void SetupInputComponent() override;

	void ExitGame();

public:
	UFUNCTION()
	void OnDeviceBackKey();
	//
	void ShowDamageNumber(const float local_damage_done,AUnitPawn* unit_pawn,EDamagePopup dmgPopup); //target

	void ShowDamageText(const FString stringWant,AUnitPawn* unit_pawn,EDamagePopup dmgPopup); //target

	UFUNCTION()
    void BackToSelectMenu();
    

};
