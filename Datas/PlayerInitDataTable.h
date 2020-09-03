// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataTable.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "Engine/SkeletalMesh.h"
#include "PlayerInitDataTable.generated.h"

/**
 * 
 */



USTRUCT(BlueprintType)
struct FPlayerHairRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    FPlayerHairRow():
    m_MeshHalfHair(nullptr),
    m_MeshFullHair(nullptr)
    {
        
    }
    
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
    FText m_ShowingName;
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
    USkeletalMesh* m_MeshHalfHair;
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
    USkeletalMesh* m_MeshFullHair;
};

USTRUCT(BlueprintType)
struct FPlayerFaceRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    FPlayerFaceRow():
    m_MeshFace(nullptr)
    {
    }
    
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
    FText m_ShowingName;
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
    USkeletalMesh* m_MeshFace;
    
};

USTRUCT(BlueprintType)
struct FPlayerArmorRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    FPlayerArmorRow()//사실상 시작 아이템 세트
    {
        m_BodyArmorHandle.DataTable = UItemDataTable::GetItemTable;
        m_HelmetHandle.DataTable = UItemDataTable::GetItemTable;
        m_GloveHandle.DataTable = UItemDataTable::GetItemTable;
        m_ShoeHandle.DataTable = UItemDataTable::GetItemTable;
    }

    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
    FDataTableRowHandle m_BodyArmorHandle;
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
    FDataTableRowHandle m_HelmetHandle;
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
    FDataTableRowHandle m_ShoeHandle;
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
    FDataTableRowHandle m_GloveHandle;
};

USTRUCT(BlueprintType)
struct FPlayerWeaponRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    FPlayerWeaponRow()
    {
        m_RightWeaponHandle.DataTable = UItemDataTable::GetItemTable;
        m_LeftWeaponHandle.DataTable = UItemDataTable::GetItemTable;
    }
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
    FDataTableRowHandle m_RightWeaponHandle;
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
    FDataTableRowHandle m_LeftWeaponHandle;
};

USTRUCT(BlueprintType)
struct FPlayerItemRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    
    FPlayerItemRow()
    {
        m_ItemHandle.DataTable = UItemDataTable::GetItemTable;
    }
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
    FDataTableRowHandle m_ItemHandle;
};

USTRUCT(BlueprintType)
struct FPlayerPerkRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    
    FPlayerPerkRow()
    {
    }
    
};

UCLASS()
class DIABLOM_API UPlayerInitDataTable : public UObject
{
	GENERATED_BODY()
public:
    UPlayerInitDataTable();
public:
    static	UDataTable* GetPlayerHairTable;

    static	UDataTable* GetPlayerFaceTable;

    static	UDataTable* GetPlayerArmorTable;

    static	UDataTable* GetPlayerWeaponTable;

    static	UDataTable* GetPlayerItemTable;

    static	UDataTable* GetPlayerPerkTable;

public:
    static const FPlayerHairRow& GetPlayerHair(FName id);

    static const FPlayerHairRow* GetPlayerHairPtr(FName id);

    static const FPlayerFaceRow& GetPlayerFace(FName id);

    static const FPlayerFaceRow* GetPlayerFacePtr(FName id);

    static const FPlayerArmorRow& GetPlayerArmor(FName id);

    static const FPlayerArmorRow* GetPlayerArmorPtr(FName id);

    static const FPlayerWeaponRow& GetPlayerWeapon(FName id);

    static const FPlayerWeaponRow* GetPlayerWeaponPtr(FName id);

    static const FPlayerItemRow& GetPlayerItem(FName id);

    static const FPlayerItemRow* GetPlayerItemPtr(FName id);

    static const FPlayerPerkRow& GetPlayerPerk(FName id);

    static const FPlayerPerkRow* GetPlayerPerkPtr(FName id); 
};
