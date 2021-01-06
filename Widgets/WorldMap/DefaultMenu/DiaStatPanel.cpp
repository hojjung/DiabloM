#include "DiaStatPanel.h"

#include "AbilitySystem/Attribute/PlayerDiabloAttribute.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"
#include "Managers/DiabloGameInstance.h"

FText UDiaStatPanel::GetPercentFormat(float v)
{
    FFormatOrderedArguments Args;

    if (FMath::IsNearlyZero(v))
    {
        Args.Add(Empty);
        v = 0.f;
    }
    else if (v > 1.f) //곱했을때 커지는 수
    {
        Args.Add(Plus);
        v -= 1.f;
        v *= 100.f;
        v = FMath::Abs(v);
    }
    else //0.35
    {
        Args.Add(Empty);
        v -= 1.f;
        v *= 100.f;
        v = FMath::Abs(v);
    }

    //1.35 -> 0.35 -> +35%
    //0.25 -> -0.75 -> -75%
    Args.Add(GetFloatToText(v, 1));
    Args.Add(Per); //2 

    FTextFormat FormatT = FText::FromString(FormatArguSet);

    return FText::Format(FormatT, Args);
}

FText UDiaStatPanel::GetFloatToText(float v, int floatCount)
{
    return UKismetTextLibrary::Conv_FloatToText(SetFloatPrecision(v,floatCount), ERoundingMode::HalfFromZero, false);
}

float UDiaStatPanel::SetFloatPrecision(float v, int count)
{
    return UDiaBlueprintFunctionLibrary::SetFloatPrecision(v,count);
}

void UDiaStatPanel::Init(APlayerDiabloCharacter* charPlayer)
{
    UPlayerDiabloAttribute* DiaAttri = Cast<UPlayerDiabloAttribute>(charPlayer->GetAttributeSet());
    m_Player = charPlayer;
    m_AttributeSet = Cast<UPlayerDiabloAttribute>(m_Player->GetAttributeSet());
    m_AttributeSet->m_OnStatChanged.AddUObject(this, &UDiaStatPanel::UpdateAllAttributeText);
    ADiabloPlayerController::Get->GetEquipment()->m_OnOptionChanged.AddUObject(
        this, &UDiaStatPanel::UpdateAllAttributeTextWrap);
    m_Player->GetLevelDele().AddUObject(this, &UDiaStatPanel::UpdateLevel);
    m_Player->GetRemainExpDele().AddUObject(this, &UDiaStatPanel::UpdateRemainExp); //m_OnChangedExpRamain

    //
    UpdateAllAttributeTextWrap();
}

void UDiaStatPanel::UpdateAllAttributeTextWrap()
{
    UpdateLevel(m_Player->GetCharacterLevel());
    
    float RemainExp = m_Player->m_fMaxExp - m_Player->m_fCurrentExp;
    UpdateRemainExp(RemainExp);
    
    UpdateStr(m_AttributeSet->GetStr());
    UpdateDex(m_AttributeSet->GetDex());
    UpdateInt(m_AttributeSet->GetInt());
    UpdateVit(m_AttributeSet->GetVit());
    //
    UpdatePhysDmg(m_AttributeSet->GetPhysicalDamage());
    UpdateAllDmg(m_AttributeSet->GetDamagePer());
    UpdateAccuracy(m_AttributeSet->GetAccuracy());
    //UpdateAttackPer(v);
    UpdateAttackSpeed(m_AttributeSet->GetAttackSpeed());
    UpdateCastSpeed(m_AttributeSet->GetCastingSpeed());
    UpdateCritical(m_AttributeSet->GetCriticalChance());
    UpdateCriticalDmg(m_AttributeSet->GetCriticalDamage()); //default 150

    UpdateFireDmg(m_AttributeSet->GetAtkFire());
    UpdateFrostDmg(m_AttributeSet->GetAtkCold());
    UpdatePoisonDmg(m_AttributeSet->GetAtkPoison());
    UpdateElecDmg(m_AttributeSet->GetAtkElec());

    UpdatePhysDef(m_AttributeSet->GetPhysicalDefense());
    UpdateAllDef(m_AttributeSet->GetDefensePer());
    UpdateAvoid(m_AttributeSet->GetAvoid());
    UpdateBlockChance(m_AttributeSet->GetBlockChance());

    UpdateFireDef(m_AttributeSet->GetResFire());
    UpdateFrostDef(m_AttributeSet->GetResCold());
    UpdatePoisonDef(m_AttributeSet->GetResPoison());
    UpdateElecDef(m_AttributeSet->GetResElec());

    UpdateMaxHp(m_AttributeSet->GetMaxHealth());
    UpdateHpRegen(m_AttributeSet->GetHealthRegen());
    UpdateLifeSteal(m_AttributeSet->GetLifeSteal());

    UpdateMaxMana(m_AttributeSet->GetMaxMana());
    UpdateManaRegen(m_AttributeSet->GetManaRegen());
    UpdateMaxStm(m_AttributeSet->GetMaxStamina());
    UpdateStmRegen(m_AttributeSet->GetStaminaRegen());
    UpdateMaxRage(m_AttributeSet->GetMaxRage());
    UpdateRageRegen(m_AttributeSet->GetRageRegen());

    UpdateAllSkill(m_AttributeSet->GetAllSkillBonus());
    UpdateCDReduce(m_AttributeSet->GetCoolDownReduce());
    UpdatePotionCD(m_AttributeSet->GetPotionCoolDownReduce());
    UpdatePotionHeal(m_AttributeSet->GetPotionBonusPer());
    UpdateMagicItem(m_AttributeSet->GetMagicItemDropBonus());
    UpdateGoldGain(m_AttributeSet->GetGoldBonusPer());
    UpdateExpGain(m_AttributeSet->GetExpBonusPer());
    //
    FGameplayAttribute MoveSpd = m_AttributeSet->GetMoveSpeedAttribute();
    MoveSpd.GetGameplayAttributeData(m_AttributeSet)->GetBaseValue();
    UpdateMoveSpeed(
        MoveSpd.GetGameplayAttributeData(m_AttributeSet)->GetCurrentValue() / MoveSpd.
                                                                              GetGameplayAttributeData(m_AttributeSet)->
                                                                              GetBaseValue());
}

void UDiaStatPanel::UpdateAllAttributeText(AUnitPawn* mobInfo)
{
    UpdateAllAttributeTextWrap();
}

void UDiaStatPanel::UpdateLevel(float v)
{
    m_Level->SetText(GetFloatToText(v));
}

void UDiaStatPanel::UpdateRemainExp(float v)
{
    m_RemainExp->SetText(GetFloatToText(v));
}

void UDiaStatPanel::UpdateStr(float v)
{
    m_Str->SetText(GetFloatToText(v));
}

void UDiaStatPanel::UpdateDex(float v)
{
    m_Dex->SetText(GetFloatToText(v));
}

void UDiaStatPanel::UpdateInt(float v)
{
    m_Int->SetText(GetFloatToText(v));
}

void UDiaStatPanel::UpdateVit(float v)
{
    m_Vit->SetText(GetFloatToText(v));
}

void UDiaStatPanel::UpdatePhysDmg(float v)
{
    m_PhysDmg->SetText(GetFloatToText(v));
}

void UDiaStatPanel::UpdateAllDmg(float v)
{
    FText TextCreated = GetPercentFormat(v);
    m_AllDmg->SetText(TextCreated);
}

void UDiaStatPanel::UpdateAccuracy(float v)
{
    FText TextCreated = GetPercentFormat(v);
    m_Accuracy->SetText(TextCreated);
}


void UDiaStatPanel::UpdateAttackSpeed(float v)
{
    FText TextCreated = GetPercentFormat(v);
    m_AttackSpeed->SetText(TextCreated);
}

void UDiaStatPanel::UpdateCastSpeed(float v)
{
    FText TextCreated = GetPercentFormat(v);
    m_CastSpeed->SetText(TextCreated);
}

void UDiaStatPanel::UpdateCritical(float v)
{
    FText TextCreated = GetPercentFormat(v);
    m_Critical->SetText(TextCreated);
}

void UDiaStatPanel::UpdateCriticalDmg(float v)
{
    FText TextCreated = GetPercentFormat(v);
    m_CriticalDmg->SetText(TextCreated);
}

void UDiaStatPanel::UpdateFireDmg(float v)
{
    FText TextCreated = GetPercentFormat(v);
    m_FireDmg->SetText(TextCreated);
}

void UDiaStatPanel::UpdateFrostDmg(float v)
{
    FText TextCreated = GetPercentFormat(v);
    m_FrostDmg->SetText(TextCreated);
}

void UDiaStatPanel::UpdatePoisonDmg(float v)
{
    FText TextCreated = GetPercentFormat(v);
    m_PoisonDmg->SetText(TextCreated);
}

void UDiaStatPanel::UpdateElecDmg(float v)
{
    FText TextCreated = GetPercentFormat(v);
    m_ElecDmg->SetText(TextCreated);
}

void UDiaStatPanel::UpdatePhysDef(float v)
{
    //v=roundf(v);
    m_PhysDef->SetText(GetFloatToText(v));
}

void UDiaStatPanel::UpdateAllDef(float v)
{
    FText TextCreated = GetPercentFormat(v);
    m_AllDef->SetText(TextCreated);
}

void UDiaStatPanel::UpdateAvoid(float v)
{
    FText TextCreated = GetPercentFormat(v);
    m_Avoid->SetText(TextCreated);
}

void UDiaStatPanel::UpdateBlockChance(float v)
{
    FText TextCreated = GetPercentFormat(v);
    m_BlockChance->SetText(TextCreated);
}

void UDiaStatPanel::UpdateFireDef(float v)
{
    FText TextCreated = GetPercentFormat(v);
    m_FireDef->SetText(TextCreated);
}

void UDiaStatPanel::UpdateFrostDef(float v)
{
    FText TextCreated = GetPercentFormat(v);
    m_FrostDef->SetText(TextCreated);
}


void UDiaStatPanel::UpdatePoisonDef(float v)
{
    FText TextCreated = GetPercentFormat(v);
    m_PoisonDef->SetText(TextCreated);
}

void UDiaStatPanel::UpdateElecDef(float v)
{
    FText TextCreated = GetPercentFormat(v);
    m_ElecDef->SetText(TextCreated);
}

void UDiaStatPanel::UpdateMaxHp(float v)
{
    m_MaxHp->SetText(GetFloatToText((int)v, 1));
}

void UDiaStatPanel::UpdateHpRegen(float v)
{
    m_HpRegen->SetText(GetFloatToText(v));
}

void UDiaStatPanel::UpdateLifeSteal(float v)
{
    FText TextCreated = GetPercentFormat(v);
    m_LifeSteal->SetText(TextCreated);
}

void UDiaStatPanel::UpdateMaxMana(float v)
{
    m_MaxMana->SetText(GetFloatToText(v));
}

void UDiaStatPanel::UpdateManaRegen(float v)
{
    m_ManaRegen->SetText(GetFloatToText(v));
}

void UDiaStatPanel::UpdateMaxStm(float v)
{
    m_MaxStm->SetText(GetFloatToText(v));
}

void UDiaStatPanel::UpdateStmRegen(float v)
{
    m_StmRegen->SetText(GetFloatToText(v));
}

void UDiaStatPanel::UpdateMaxRage(float v)
{
    m_MaxRage->SetText(GetFloatToText(v));
}

void UDiaStatPanel::UpdateRageRegen(float v)
{
    m_RageRegen->SetText(GetFloatToText(v));
}

void UDiaStatPanel::UpdateAllSkill(float v)
{
    m_AllSkill->SetText(GetFloatToText(v));
}

void UDiaStatPanel::UpdateCDReduce(float v)
{
    FText TextCreated = GetPercentFormat(v);
    m_CDReduce->SetText(TextCreated);
}

void UDiaStatPanel::UpdatePotionCD(float v)
{
    m_PotionCD->SetText(GetFloatToText(v));
}

void UDiaStatPanel::UpdatePotionHeal(float v)
{
    FText TextCreated = GetPercentFormat(v);
    m_PotionHeal->SetText(TextCreated);
}

void UDiaStatPanel::UpdateMagicItem(float v)
{
    FText TextCreated = GetPercentFormat(v);
    m_MagicItem->SetText(TextCreated);
}

void UDiaStatPanel::UpdateGoldGain(float v)
{
    FText TextCreated = GetPercentFormat(v);
    m_GoldGain->SetText(TextCreated);
}

void UDiaStatPanel::UpdateExpGain(float v)
{
    FText TextCreated = GetPercentFormat(v);
    m_ExpGain->SetText(TextCreated);
}

void UDiaStatPanel::UpdateMoveSpeed(float v)
{
    FText TextCreated = GetPercentFormat(v);
    m_MoveSpeed->SetText(TextCreated);
}
