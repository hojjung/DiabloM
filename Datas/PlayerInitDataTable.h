#pragma once

#include "CharacterDataTable.h"
#include "DiabloM.h"
#include "ItemDataTable.h"
#include "Item/EquipmentSystem.h"

#include "PlayerInitDataTable.generated.h"


USTRUCT(BlueprintType)
struct FPlayerInitItemTableRow  : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    USkeletalMesh* m_CoolMesh;
    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    UAnimSequence* m_CoolIdle;
    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    FItemDataHandle m_RightHandItem;
    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    FItemDataHandle m_LeftHandItem;
    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    FPlayerTypeHandle m_PlayerClass;
};

UCLASS()
class DIABLOM_API UPlayerInitDataTable : public UObject
{
	GENERATED_BODY()
public:
    UPlayerInitDataTable();
    static	UDataTable* GetPlayerItemTable;

public:
    static const FPlayerInitItemTableRow& GetPlayerItem(FName id);

    static const FPlayerInitItemTableRow* GetPlayerItemPtr(FName id);
};

USTRUCT(BlueprintType) //���̵�,Ƽ��
struct FCurrentCharData
{
    GENERATED_BODY()

public:
    FCurrentCharData(): m_CurrentSkin(nullptr), m_CoolIdle(nullptr)
    {
        m_AryEquipItemData.Reset();
        m_AryEquipItemData.Init(nullptr, (int)ESlotsEquipAry::Length);
    }

    UPROPERTY()
    FText m_TextNameClass;
    UPROPERTY()
    FName m_ClassID;
    UPROPERTY()
    USkeletalMesh*   m_CurrentSkin;
    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    UAnimSequence* m_CoolIdle;
    
    TArray<const FItemData*> m_AryEquipItemData;
    
    
    //
    void Clear()
    {
        m_CurrentSkin=nullptr;

        m_CoolIdle =nullptr;
        
        for(const FItemData* itemData : m_AryEquipItemData)
        {
            itemData = nullptr;
        }

        m_TextNameClass = FText();
        m_ClassID=NAME_None;
    }

    const FItemData* GetItemData(ESlotsEquipAry slot) const
    {
        return m_AryEquipItemData[static_cast<int>(slot)];
    }
};
