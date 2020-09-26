// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "AbilitySystem/Attribute/PlayerDiabloAttribute.h"

#include "Blueprint/UserWidget.h"
#include "Characters/PlayerDiabloCharacter.h"

#include "DiaStatPanel.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UDiaStatPanel : public UUserWidget
{
	GENERATED_BODY()
	
	//43need
private:
	FText Plus = FText::FromString("+");
	FText Minus = FText::FromString("-");
	FText Per = FText::FromString("%");
	FString FormatArguSet = "{0}{1}{2}";
	FText Empty=FText::FromString(" ");
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_Level;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_RemainExp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_Str;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_Dex;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_Int;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_Vit;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_PhysDmg;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_AllDmg;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_Accuracy;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_AttackPerSec;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_AttackSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_CastSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_Critical;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_DoubleAtk;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_Basher;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_FireDmg;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_FrostDmg;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_PoisonDmg;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_ElecDmg;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_PhysDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_AllDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_Avoid;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_BlockChance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_FireDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_FrostDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_PoisonDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_ElecDef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_MaxHp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_HpRegen;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_LifeSteal;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_MaxMana;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_ManaRegen;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_MaxStm;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_StmRegen;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_MaxRage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_RageRegen;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_MoveSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_AllSkill;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_CDReduce;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_PotionCD;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_PotionHeal;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_MagicItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_GoldGain;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_ExpGain;

protected:
	FText GetPercentFormat(float v);
	
	FText GetFloatToText(float v,int floatCount=1);
    
	UPROPERTY()
	APlayerDiabloCharacter* m_Player;
	UPROPERTY()
	UPlayerDiabloAttribute* m_AttributeSet;
public:
	void Init(APlayerDiabloCharacter* charPlayer);

	void UpdateAllAttributeText();
protected:	
	void UpdateLevel(float v);
    void UpdateRemainExp(float v);
    void UpdateStr(float v);
    void UpdateDex(float v);
    void UpdateInt(float v);
    void UpdateVit(float v);
    void UpdatePhysDmg(float v);
    void UpdateAllDmg(float v);
    void UpdateAccuracy(float v);
    void UpdateAttackPer(float v);
    void UpdateAttackSpeed(float v);
    void UpdateCastSpeed(float v);
    void UpdateCritical(float v);
    void UpdateDoubleAtk(float v);
    void UpdateBasher(float v);
    void UpdateFireDmg(float v);
    void UpdateFrostDmg(float v);
    void UpdatePoisonDmg(float v);
    void UpdateElecDmg(float v);
    void UpdatePhysDef(float v);
    void UpdateAllDef(float v);
    void UpdateAvoid(float v);
    void UpdateBlockChance(float v);
    void UpdateFireDef(float v);
    void UpdateFrostDef(float v);
    void UpdatePoisonDef(float v);
    void UpdateElecDef(float v);
    void UpdateMaxHp(float v);
    void UpdateHpRegen(float v);
    void UpdateLifeSteal(float v);
    void UpdateMaxMana(float v);
    void UpdateManaRegen(float v);
    void UpdateMaxStm(float v);
    void UpdateStmRegen(float v);
    void UpdateMaxRage(float v);
    void UpdateRageRegen(float v);
    void UpdateAllSkill(float v);
    void UpdateCDReduce(float v);
    void UpdatePotionCD(float v);
    void UpdatePotionHeal(float v);
    void UpdateMagicItem(float v);
    void UpdateGoldGain(float v);
    void UpdateExpGain(float v);
	void UpdateMoveSpeed(float v);
};

