#pragma once

#include "DiabloM.h"
#include "CharInfo.h"
#include "Blueprint/UserWidget.h"
#include "CharSelect.generated.h"


class UStartMenuCanvas;
class USaveCharacterStatus;
class USaveLoadManager;
/**
 * 로딩한 캐릭터 고르는 화면
 */
UCLASS()
class DIABLOM_API UCharSelect : public UUserWidget
{
	GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
    UButton* m_BtnCreateNewChar;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
    UButton* m_BtnContinue;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
    UVerticalBox* m_SlotParent;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UCharInfo> m_ClassCharInfo;
    UPROPERTY()
    int m_FocusedIndex;
    UPROPERTY()
    TArray<TWeakObjectPtr<UCharInfo>> m_AryCharInfoSlot;
public:
    void Init(USaveLoadManager* SaveLoadManagerOld);
    
    void CreateCharInfo(const USaveCharacterStatus* charStats);

    void FocusCharacter(int slotIndex);

    friend UStartMenuCanvas;

    UFUNCTION()
    void ContinueToGameWorld();
};
