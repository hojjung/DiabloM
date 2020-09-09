// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Button.h"
#include "CharInfo.h"
#include "VerticalBox.h"
#include "Blueprint/UserWidget.h"
#include "CharSelect.generated.h"





class USaveCharacterStatus;
class SaveLoadManager;
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
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    TArray<UCharInfo*> m_AryCharSlots;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
    UVerticalBox* m_SlotParent;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UCharInfo> m_ClassCharInfo;
    UPROPERTY()
    UCharInfo* m_FocusedInfo;
public:
    void Init(SaveLoadManager* saveLoadManager);
    
    void CreateCharInfo(const USaveCharacterStatus* charStats);

    void FocusCharacter(int slotIndex);
};
