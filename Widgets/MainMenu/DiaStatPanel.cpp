#include "DiaStatPanel.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/DgContentManagers/DungeonManager.h"

void UDiaStatPanel::UpdateAll()
{
	m_DisplayName->SetText(FText::FromString((FString::Printf(TEXT("닉네임:%s"),*UDiabloGameInstance::Get->m_PlayfabManager->m_LoadedNickname))));
	m_NormalStageMax->SetText(FText::FromString(FString::Printf(TEXT("최대스테이지:%d"),UDiabloGameInstance::Get->m_NormalDgManager->m_nMyMaxStageLevel.GetValue())));
	m_AttackLv->SetText(FText::FromString(FString::Printf(TEXT("공격력 레벨.Lv%d"),UDiabloGameInstance::Get->m_PlayerUpgradeManager->GetAtkUp(EAttackType::BaseAttack).m_nLv.GetValue())));
	m_CriLv->SetText(FText::FromString(FString::Printf(TEXT("치명확률 레벨.Lv%d"),UDiabloGameInstance::Get->m_PlayerUpgradeManager->GetAtkUp(EAttackType::Critical).m_nLv.GetValue())));
	m_CriDmgLv->SetText(FText::FromString(FString::Printf(TEXT("치명데미지 레벨.Lv%d"),UDiabloGameInstance::Get->m_PlayerUpgradeManager->GetAtkUp(EAttackType::CriticalDmg).m_nLv.GetValue())));
	m_SCriLv->SetText(FText::FromString(FString::Printf(TEXT("슈퍼치명확률 레벨.Lv%d"),UDiabloGameInstance::Get->m_PlayerUpgradeManager->GetAtkUp(EAttackType::SuperCritical).m_nLv.GetValue())));
	m_SCriDmgLv->SetText(FText::FromString(FString::Printf(TEXT("슈퍼치명데미지 레벨.Lv%d"),UDiabloGameInstance::Get->m_PlayerUpgradeManager->GetAtkUp(EAttackType::SuperCriticalDmg).m_nLv.GetValue())));
	m_MagicLv->SetText(FText::FromString(FString::Printf(TEXT("마력폭발확률 레벨.Lv%d"),UDiabloGameInstance::Get->m_PlayerUpgradeManager->GetAtkUp(EAttackType::MagicBomb).m_nLv.GetValue())));
	m_MagicDmgLv->SetText(FText::FromString(FString::Printf(TEXT("마력폭발데미지 레벨.Lv%d"),UDiabloGameInstance::Get->m_PlayerUpgradeManager->GetAtkUp(EAttackType::MagicBombDmg).m_nLv.GetValue())));
	m_SMagicLv->SetText(FText::FromString(FString::Printf(TEXT("슈퍼마력폭발확률 레벨.Lv%d"),UDiabloGameInstance::Get->m_PlayerUpgradeManager->GetAtkUp(EAttackType::SuperMagicBomb).m_nLv.GetValue())));
	m_SMagicDmgLv->SetText(FText::FromString(FString::Printf(TEXT("슈퍼마력폭발데미지 레벨.Lv%d"),UDiabloGameInstance::Get->m_PlayerUpgradeManager->GetAtkUp(EAttackType::SuperMagicBombDmg).m_nLv.GetValue())));
	
	m_Skill01->SetText(FText::FromString(FString::Printf(TEXT("데스블로우 레벨.Lv%d"),UDiabloGameInstance::Get->m_PlayerUpgradeManager->GetSkillUp(ESkillType::DeathBlow).m_nLv.GetValue())));
	m_Skill02->SetText(FText::FromString(FString::Printf(TEXT("가시가루 레벨.Lv%d"),UDiabloGameInstance::Get->m_PlayerUpgradeManager->GetSkillUp(ESkillType::MagicBlade).m_nLv.GetValue())));
	m_Skill03->SetText(FText::FromString(FString::Printf(TEXT("휠윈드 레벨.Lv%d"),UDiabloGameInstance::Get->m_PlayerUpgradeManager->GetSkillUp(ESkillType::WhirlWind).m_nLv.GetValue())));
	m_Skill04->SetText(FText::FromString(FString::Printf(TEXT("지진 레벨.Lv%d"),UDiabloGameInstance::Get->m_PlayerUpgradeManager->GetSkillUp(ESkillType::EarthQuake).m_nLv.GetValue())));
	m_Skill05->SetText(FText::FromString(FString::Printf(TEXT("바람가루 레벨.Lv%d"),UDiabloGameInstance::Get->m_PlayerUpgradeManager->GetSkillUp(ESkillType::WindBlade).m_nLv.GetValue())));
	
	m_AttackSpeed->SetText(FText::FromString(FString::Printf(TEXT("초당 공격횟수:%.1f"),UDiabloGameInstance::Get->GetPlChar()->GetAttackSpeed())));

	int QuestAmountMin = UDiabloGameInstance::Get->m_QuestManager->GetQuest(EQuestType::PlayTime).QuestAmount;
	FTimespan Span(0,QuestAmountMin,0);
	FString StrPlayTime  = FString::Printf(TEXT("플레이타임:%d시간,%d분"),Span.GetHours(),Span.GetMinutes());
	m_TotalPlayTime->SetText(FText::FromString(StrPlayTime));
	
	m_TotalSkinUnlockPercent->SetText(FText::FromString(FString::Printf(TEXT("스킨해금도:%s"),*UDiabloGameInstance::Get->m_EquipManager->GetSkinUnlockStr())));
	m_TotalPetUnlockPercent->SetText(FText::FromString(FString::Printf(TEXT("펫해금도:%s"),*UDiabloGameInstance::Get->m_EquipManager->GetPetUnlockStr())));
	m_TotalWeaponUnlockPercent->SetText(FText::FromString(FString::Printf(TEXT("무기해금도:%s"),*UDiabloGameInstance::Get->m_EquipManager->GetWeaponUnlockStr())));
}

void UDiaStatPanel::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	if(InVisibility==ESlateVisibility::SelfHitTestInvisible || InVisibility==ESlateVisibility::Visible || InVisibility==ESlateVisibility::HitTestInvisible)
	{
		UpdateAll();
	}
}
