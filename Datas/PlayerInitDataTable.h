#pragma once

#include "CharacterDataTable.h"
#include "DiabloM.h"
#include "ItemDataTable.h"
#include "PlayerInitDataTable.generated.h"


USTRUCT(BlueprintType)
struct FPlayerInitItemTableRow  : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    USkeletalMesh* m_CoolMesh;
    UPROPERTY(EditAnywhere,BlueprintReadOnly)
    UAnimBlueprint* m_StanceAnim;
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
    FCurrentCharData(): m_CurrentSkin(nullptr), m_CurrentRightWeapon(nullptr), m_CurrentLeftWeapon(nullptr)
    {
    }

    UPROPERTY()
    FText m_TextNameClass;
    UPROPERTY()
    FName m_ClassID;
    UPROPERTY()
    USkeletalMesh*   m_CurrentSkin;
    
    const FItemData*     m_CurrentRightWeapon;
    const FItemData*     m_CurrentLeftWeapon;
    //
    void Clear()
    {
        m_CurrentSkin=nullptr;         
        m_CurrentRightWeapon=nullptr;  
        m_CurrentLeftWeapon=nullptr;

        m_TextNameClass = FText();
        m_ClassID=NAME_None;
    }
};
