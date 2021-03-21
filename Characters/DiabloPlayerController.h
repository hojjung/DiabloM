#pragma once

#include "DiabloM.h"
#include "MovePointIndicator.h"
#include "GameFramework/PlayerController.h"
#include "Widgets/CommonElement/DamageTextWidgetComponent.h"

#include "DiabloPlayerController.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTick,float);

class AUnitPawn;
UCLASS()
class DIABLOM_API ADiabloPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADiabloPlayerController();

	FOnTick m_OnTick;
	
protected:
	UPROPERTY()
	AMovePointIndicator* m_MovePointIndicator;
	UPROPERTY()
	TSubclassOf<UDamageTextWidgetComponent> m_ClassDW;
	UPROPERTY()
	TArray<UDamageTextWidgetComponent*> m_AryDmgWC;
	
	FTextFormat m_FormatMiss;
	
	int m_DmgIndex;

	UPROPERTY()
	TArray<AActor*> m_AryIgnoreActors;

	TArray<TEnumAsByte<EObjectTypeQuery>> m_AryQuery;
protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
	
	void InitWidget();

	void CreateDmgWC(int count);

	UDamageTextWidgetComponent* GetDmgWC(); 

	virtual void SetupInputComponent() override;

	void ExitGame();

public:
	UFUNCTION()
	void OnDeviceBackKey();
	//
	void ShowDamageNumber(const BigInt& local_damage_done,AUnitPawn* unit_pawn,EDamagePopup dmgPopup); //target

	void ShowDamageText(const FString& stringWant,AUnitPawn* unit_pawn,EDamagePopup dmgPopup); //target

	UFUNCTION()
    void BackToSelectMenu();

public:
	UFUNCTION()
	void ClickActor();

	void ShowMoveIndicator(FVector loc);

	void HideMoveIndicator();

	virtual void Tick(float DeltaSeconds) override;
};
