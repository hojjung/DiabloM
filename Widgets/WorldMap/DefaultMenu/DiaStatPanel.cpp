#include "DiaStatPanel.h"

#include "AbilitySystem/Attribute/PlayerDiabloAttribute.h"

FText UDiaStatPanel::GetPercentFormat(float v)
{
    FFormatOrderedArguments Args;

    if (FMath::IsNearlyZero(v))
    {
        Args.Add(Empty);
        v=0.f;
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
    Args.Add(UKismetTextLibrary::Conv_FloatToText(v, ERoundingMode::FromZero, false, true, 1, 324, 1, 1));
    Args.Add(Per); //2 

    FTextFormat FormatT = FText::FromString(FormatArguSet);

    return FText::Format(FormatT, Args);
}

void UDiaStatPanel::Init(APlayerDiabloCharacter* charPlayer)
{
    UPlayerDiabloAttribute* DiaAttri = Cast<UPlayerDiabloAttribute>(charPlayer->GetAttributeSet());

    float v = 0.f;
    UpdateLevel(v);
    UpdateRemainExp(v);
    UpdateStr(v);
    UpdateDex(v);
    UpdateInt(v);
    UpdateVit(v);
    UpdatePhysDmg(v);
    UpdateAllDmg(v);
    UpdateAccuracy(v);
    UpdateAttackPer(v);
    UpdateAttackSpeed(v);
    UpdateCastSpeed(v);
    UpdateCritical(v);
    UpdateDoubleAtk(v);
    UpdateBasher(v);
    UpdateFireDmg(v);
    UpdateFrostDmg(v);
    UpdatePoisonDmg(v);
    UpdateElecDmg(v);
    UpdatePhysDef(v);
    UpdateAllDef(v);
    UpdateAvoid(v);
    UpdateBlockChance(v);
    UpdateFireDef(v);
    UpdateFrostDef(v);
    UpdatePoisonDef(v);
    UpdateElecDef(v);
    UpdateMaxHp(v);
    UpdateHpRegen(v);
    UpdateLifeSteal(v);
    UpdateMaxMana(v);
    UpdateManaRegen(v);
    UpdateMaxStm(v);
    UpdateStmRegen(v);
    UpdateMaxRage(v);
    UpdateRageRegen(v);
    UpdateAllSkill(v);
    UpdateCDReduce(v);
    UpdatePotionCD(v);
    UpdatePotionHeal(v);
    UpdateMagicItem(v);
    UpdateGoldGain(v);
    UpdateExpGain(v);
    UpdateMoveSpeed(v);

    charPlayer->m_OnAttackPerSecChanged.AddUObject(this, &UDiaStatPanel::UpdateAttackPer);
    charPlayer->m_OnLevelChanged.AddUObject(this, &UDiaStatPanel::UpdateLevel);
    charPlayer->m_OnRemainExpChanged.AddUObject(this, &UDiaStatPanel::UpdateRemainExp); //m_OnChangedExpRamain

    DiaAttri->m_OnChangedResCold.AddUObject(this, &UDiaStatPanel::UpdateFrostDef);
    DiaAttri->m_OnChangedResElec.AddUObject(this, &UDiaStatPanel::UpdateElecDef);
    DiaAttri->m_OnChangedDex.AddUObject(this, &UDiaStatPanel::UpdateDex);
    DiaAttri->m_OnChangedInt.AddUObject(this, &UDiaStatPanel::UpdateInt);
    DiaAttri->m_OnChangedStr.AddUObject(this, &UDiaStatPanel::UpdateStr);
    DiaAttri->m_OnChangedVit.AddUObject(this, &UDiaStatPanel::UpdateVit);
    DiaAttri->m_OnChangedBashChance.AddUObject(this, &UDiaStatPanel::UpdateBasher);
    DiaAttri->m_OnChangedBlockChance.AddUObject(this, &UDiaStatPanel::UpdateBlockChance);
    DiaAttri->m_OnChangedCastingSpeed.AddUObject(this, &UDiaStatPanel::UpdateCastSpeed);
    DiaAttri->m_OnChangedCriticalChance.AddUObject(this, &UDiaStatPanel::UpdateCritical);
    DiaAttri->m_OnChangedHealthRegen.AddUObject(this, &UDiaStatPanel::UpdateHpRegen);
    DiaAttri->m_OnChangedLifeSteal.AddUObject(this, &UDiaStatPanel::UpdateLifeSteal);
    DiaAttri->m_OnChangedManaRegen.AddUObject(this, &UDiaStatPanel::UpdateManaRegen);
    DiaAttri->m_OnChangedMaxMana.AddUObject(this, &UDiaStatPanel::UpdateMaxMana);
    DiaAttri->m_OnChangedMaxRage.AddUObject(this, &UDiaStatPanel::UpdateMaxRage);
    DiaAttri->m_OnChangedMaxStamina.AddUObject(this, &UDiaStatPanel::UpdateMaxStm);
    DiaAttri->m_OnChangedRageRegen.AddUObject(this, &UDiaStatPanel::UpdateRageRegen);
    DiaAttri->m_OnChangedStaminaRegen.AddUObject(this, &UDiaStatPanel::UpdateStmRegen);
    DiaAttri->m_OnChangedAllSkillBonus.AddUObject(this, &UDiaStatPanel::UpdateAllSkill);
    DiaAttri->m_OnChangedCoolDownReduce.AddUObject(this, &UDiaStatPanel::UpdateCDReduce);
    DiaAttri->m_OnChangedDoubleAttackChance.AddUObject(this, &UDiaStatPanel::UpdateDoubleAtk);
    DiaAttri->m_OnChangedExpBonusPer.AddUObject(this, &UDiaStatPanel::UpdateExpGain); //m_OnChangedExpRamain

    DiaAttri->m_OnChangedGoldBonusPer.AddUObject(this, &UDiaStatPanel::UpdateGoldGain);
    DiaAttri->m_OnChangedPotionBonusPer.AddUObject(this, &UDiaStatPanel::UpdatePotionHeal);
    DiaAttri->m_OnChangedMagicItemDropBonus.AddUObject(this, &UDiaStatPanel::UpdateMagicItem);
    DiaAttri->m_OnChangedPotionCoolDownReduce.AddUObject(this, &UDiaStatPanel::UpdatePotionCD);
    DiaAttri->m_OnChangedAccuracy.AddUObject(this, &UDiaStatPanel::UpdateAccuracy);
    DiaAttri->m_OnChangedAtkCold.AddUObject(this, &UDiaStatPanel::UpdateFrostDmg);
    DiaAttri->m_OnChangedAtkElec.AddUObject(this, &UDiaStatPanel::UpdateElecDmg);
    DiaAttri->m_OnChangedAtkFire.AddUObject(this, &UDiaStatPanel::UpdateFireDmg);
    DiaAttri->m_OnChangedAtkPoison.AddUObject(this, &UDiaStatPanel::UpdatePoisonDmg);
    DiaAttri->m_OnChangedAttackSpeed.AddUObject(this, &UDiaStatPanel::UpdateAttackSpeed);
    DiaAttri->m_OnChangedAvoidChance.AddUObject(this, &UDiaStatPanel::UpdateAvoid);
    DiaAttri->m_OnChangedDamagePer.AddUObject(this, &UDiaStatPanel::UpdateAttackPer);
    DiaAttri->m_OnChangedDefensePer.AddUObject(this, &UDiaStatPanel::UpdateAllDef);
    DiaAttri->m_OnChangedMaxHealth.AddUObject(this, &UDiaStatPanel::UpdateMaxHp);
    DiaAttri->m_OnChangedMoveSpeed.AddUObject(this, &UDiaStatPanel::UpdateMoveSpeed);
    DiaAttri->m_OnChangedPhysicalDamage.AddUObject(this, &UDiaStatPanel::UpdatePhysDmg);
    DiaAttri->m_OnChangedPhysicalDefense.AddUObject(this, &UDiaStatPanel::UpdatePhysDef);
    DiaAttri->m_OnChangedResFire.AddUObject(this, &UDiaStatPanel::UpdateFireDef);
    DiaAttri->m_OnChangedResPoison.AddUObject(this, &UDiaStatPanel::UpdatePoisonDef);

    //UpdateAttackPer
}

void UDiaStatPanel::UpdateLevel(float v)
{
    m_Level->SetText(UKismetTextLibrary::Conv_FloatToText(v, ERoundingMode::FromZero));
}

void UDiaStatPanel::UpdateRemainExp(float v)
{
    FText ASD = UKismetTextLibrary::Conv_FloatToText(v, ERoundingMode::FromZero);
    m_RemainExp->SetText(ASD);
}

void UDiaStatPanel::UpdateStr(float v)
{
    m_Str->SetText(UKismetTextLibrary::Conv_FloatToText(v, ERoundingMode::FromZero));
}

void UDiaStatPanel::UpdateDex(float v)
{
    m_Dex->SetText(UKismetTextLibrary::Conv_FloatToText(v, ERoundingMode::FromZero));
}

void UDiaStatPanel::UpdateInt(float v)
{
    m_Int->SetText(UKismetTextLibrary::Conv_FloatToText(v, ERoundingMode::FromZero));
}

void UDiaStatPanel::UpdateVit(float v)
{
    m_Vit->SetText(UKismetTextLibrary::Conv_FloatToText(v, ERoundingMode::FromZero));
}

void UDiaStatPanel::UpdatePhysDmg(float v)
{
    m_PhysDmg->SetText(UKismetTextLibrary::Conv_FloatToText(v, ERoundingMode::FromZero));
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

void UDiaStatPanel::UpdateAttackPer(float v)
{
    m_AttackPerSec->SetText(UKismetTextLibrary::Conv_FloatToText(v, ERoundingMode::FromZero));
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

void UDiaStatPanel::UpdateDoubleAtk(float v)
{
    FText TextCreated = GetPercentFormat(v);
    m_DoubleAtk->SetText(TextCreated);
}

void UDiaStatPanel::UpdateBasher(float v)
{
    FText TextCreated = GetPercentFormat(v);
    m_Basher->SetText(TextCreated);
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
    m_PhysDef->SetText(UKismetTextLibrary::Conv_FloatToText(v, ERoundingMode::FromZero));
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
    m_MaxHp->SetText(UKismetTextLibrary::Conv_FloatToText(v, ERoundingMode::FromZero));
}

void UDiaStatPanel::UpdateHpRegen(float v)
{
    m_HpRegen->SetText(UKismetTextLibrary::Conv_FloatToText(v, ERoundingMode::FromZero));
}

void UDiaStatPanel::UpdateLifeSteal(float v)
{
    FText TextCreated = GetPercentFormat(v);
    m_LifeSteal->SetText(TextCreated);
}

void UDiaStatPanel::UpdateMaxMana(float v)
{
    m_MaxMana->SetText(UKismetTextLibrary::Conv_FloatToText(v, ERoundingMode::FromZero));
}

void UDiaStatPanel::UpdateManaRegen(float v)
{
    m_ManaRegen->SetText(UKismetTextLibrary::Conv_FloatToText(v, ERoundingMode::FromZero));
}

void UDiaStatPanel::UpdateMaxStm(float v)
{
    m_MaxStm->SetText(UKismetTextLibrary::Conv_FloatToText(v, ERoundingMode::FromZero));
}

void UDiaStatPanel::UpdateStmRegen(float v)
{
    m_StmRegen->SetText(UKismetTextLibrary::Conv_FloatToText(v, ERoundingMode::FromZero));
}

void UDiaStatPanel::UpdateMaxRage(float v)
{
    m_MaxRage->SetText(UKismetTextLibrary::Conv_FloatToText(v, ERoundingMode::FromZero));
}

void UDiaStatPanel::UpdateRageRegen(float v)
{
    m_RageRegen->SetText(UKismetTextLibrary::Conv_FloatToText(v, ERoundingMode::FromZero));
}

void UDiaStatPanel::UpdateAllSkill(float v)
{
    m_AllSkill->SetText(UKismetTextLibrary::Conv_FloatToText(v, ERoundingMode::FromZero));
}

void UDiaStatPanel::UpdateCDReduce(float v)
{
    FText TextCreated = GetPercentFormat(v);
    m_CDReduce->SetText(TextCreated);
}

void UDiaStatPanel::UpdatePotionCD(float v)
{
    m_PotionCD->SetText(UKismetTextLibrary::Conv_FloatToText(v, ERoundingMode::FromZero));
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
