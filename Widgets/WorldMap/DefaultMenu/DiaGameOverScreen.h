#pragma once
#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "DiaGameOverScreen.generated.h"

class ADiabloPlayerController;
class APlayerDiabloCharacter;
UCLASS()
class DIABLOM_API UDiaGameOverScreen : public UUserWidget
{
	GENERATED_BODY()
	
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "GameOver")
	UButton* m_BtnReviveHere;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "GameOver")
	UButton* m_BtnRestartDungeon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), Category = "GameOver")
	UButton* m_BtnBackToVillage;
protected:
	TWeakObjectPtr<APlayerDiabloCharacter> m_Player;
	TWeakObjectPtr<ADiabloPlayerController> m_PlayerController;
	
public:
	void Init(ADiabloPlayerController* plCon, APlayerDiabloCharacter* player);
	UFUNCTION()
	void ReviveHere();
	UFUNCTION()
    void RestartDungeon();
	UFUNCTION()
    void BackToVillage();
};
