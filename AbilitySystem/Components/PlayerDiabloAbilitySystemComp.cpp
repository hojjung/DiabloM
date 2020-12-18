#include "PlayerDiabloAbilitySystemComp.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "TTTechNode.h"
#include "Managers/StartMap/PlayerCreateManager.h"

UPlayerDiabloAbilitySystemComp* UPlayerDiabloAbilitySystemComp::Get=nullptr;

UPlayerDiabloAbilitySystemComp::UPlayerDiabloAbilitySystemComp()
{
    m_TechTree = nullptr;
    m_SkillDataTableRow = nullptr;
    m_AryBaseSkill.Reset();
    m_AryPowerSkill.Reset();
    m_AryDefensvieSkill.Reset();
    m_ArySpecialSkill.Reset();
    m_AryMasterySkill.Reset();
    m_AryUltimateSkill.Reset();
    m_nSkillPoints = 10; //Test
    m_nTotalSkillPointSpents = 0;

    UPlayerDiabloAbilitySystemComp::Get=this;
}

void UPlayerDiabloAbilitySystemComp::BeginPlay()
{
    Super::BeginPlay();

    m_PlayerPawn = Cast<APlayerDiabloCharacter>(GetOwner());
    
    m_TechManager= NewObject<UTechTreeManager>();

    
}


int UPlayerDiabloAbilitySystemComp::GetSkillPoints()
{
    return m_nSkillPoints;
}


void UPlayerDiabloAbilitySystemComp::SetLoadedSkillData(TArray<FSkillDataSpec>& skill1, TArray<FSkillDataSpec>& skill2,
                                                        TArray<FSkillDataSpec>& skill3, TArray<FSkillDataSpec>& skill4,
                                                        TArray<FSkillDataSpec>& skill5,
                                                        TArray<FSkillDataSpec>& skill6)
{
    SetSkillFromSaveData(m_AryBaseSkill, skill1);
    SetSkillFromSaveData(m_AryPowerSkill, skill2);
    SetSkillFromSaveData(m_AryDefensvieSkill, skill3);
    SetSkillFromSaveData(m_ArySpecialSkill, skill4);
    SetSkillFromSaveData(m_AryMasterySkill, skill5);
    SetSkillFromSaveData(m_AryUltimateSkill, skill6);
}

void UPlayerDiabloAbilitySystemComp::LevelupSkill(FSkillDataSpec* skillSpec)
{
    PRINTF("Skill Learn Pressed");

    if (GetSkillPoints() <= 0)
    {
        PRINTF("SkillLearn Fail - No SkillPoints");
        return;
    }

    if (!skillSpec->IsLevelupable(m_PlayerPawn->GetCharacterLevel()))
    {
        PRINTF("SkillLearn Fail - Not Require Levels");
        return;;
    }

    skillSpec->m_nCurrentLevel++;

    m_nSkillPoints--;

    m_nTotalSkillPointSpents++;

    m_OnSkillLevelChanged.Broadcast(skillSpec);
    //현재 장착된 스킬 업데이트

    //저장
} //GetDiaAbilitySystem()->TryActivateAbility(m_PotionHandle);

void UPlayerDiabloAbilitySystemComp::EquipSkill(FSkillDataSpec* skillSpec)
{
    check(skillSpec->m_SkillDataPtr->m_SkillAbility);

    FGameplayAbilitySpec Spec = FGameplayAbilitySpec(skillSpec->m_SkillDataPtr->m_SkillAbility,
                                                     skillSpec->m_nCurrentLevel, -1, this);

    FGameplayAbilitySpecHandle Handle = GiveAbility(Spec);

    m_EquippedSkill.Add(skillSpec, Handle);
}

bool UPlayerDiabloAbilitySystemComp::UnequipSkill(FSkillDataSpec* skillSpec)
{
    if (!CheckAlreadyEquipped(skillSpec))
    {
        return false;
    }

    if (!IsCooldownAvailable(skillSpec))
    {
        return false;
    }

    ClearAbility(m_EquippedSkill[skillSpec]);
    m_EquippedSkill.Remove(skillSpec);
    int Index = skillSpec->m_nEquipIndex;
    skillSpec->m_nEquipIndex = -1;
    m_OnSkillChanged.Broadcast(skillSpec, Index);

    return true;
}

bool UPlayerDiabloAbilitySystemComp::IsCooldownAvailable(FSkillDataSpec* skillSpec)
{
    FGameplayAbilitySpec* RunningSpec = FindAbilitySpecFromHandle(m_EquippedSkill[skillSpec]);

    float RemainCD = RunningSpec->Ability->GetCooldownTimeRemaining(AbilityActorInfo.Get());

    if (RemainCD > 0.f)
    {
        return false;
    }

    return true;
}

FGameplayAbilitySpec* UPlayerDiabloAbilitySystemComp::UseSkill(FSkillDataSpec* skillSpec)
{
    if (!skillSpec)
    {
        return nullptr;
    }

    FGameplayAbilitySpecHandle Handle = m_EquippedSkill[skillSpec];

    if (TryActivateAbility(Handle))
    {
        return FindAbilitySpecFromHandle(Handle);
    }

    return nullptr;
}

bool UPlayerDiabloAbilitySystemComp::CheckAlreadyEquipped(FSkillDataSpec* skillSpec)
{
    return m_EquippedSkill.Contains(skillSpec);
}

void UPlayerDiabloAbilitySystemComp::CreateClassSkillSpecs(const FSkillDataHandle& skillDataHandle)
{
    m_SkillDataTableRow = skillDataHandle.GetRow<FSkillDataRow>("SkillDataNotFound-PlayerGASComp");

    check(m_SkillDataTableRow);

    for (const FSkillData& SkillData : m_SkillDataTableRow->m_AryBaseSkillBelt)
    {
        m_AryBaseSkill.Emplace(FSkillDataSpec(0, &SkillData));
    }

    for (const FSkillData& SkillData : m_SkillDataTableRow->m_AryDefensiveSkillBelt)
    {
        m_AryDefensvieSkill.Emplace(FSkillDataSpec(0, &SkillData));
    }

    for (const FSkillData& SkillData : m_SkillDataTableRow->m_AryMasterySkillBelt)
    {
        m_AryMasterySkill.Emplace(FSkillDataSpec(0, &SkillData));
    }
    for (const FSkillData& SkillData : m_SkillDataTableRow->m_AryPowerSkillBelt)
    {
        m_AryPowerSkill.Emplace(FSkillDataSpec(0, &SkillData));
    }

    for (const FSkillData& SkillData : m_SkillDataTableRow->m_ArySpecialSkillBelt)
    {
        m_ArySpecialSkill.Emplace(FSkillDataSpec(0, &SkillData));
    }

    for (const FSkillData& SkillData : m_SkillDataTableRow->m_AryUltimateSkillBelt)
    {
        m_AryUltimateSkill.Emplace(FSkillDataSpec(0, &SkillData));
    }
}

void UPlayerDiabloAbilitySystemComp::ResetTech()
{
    m_TechManager->ResetTech();

    m_OnTalentChanged.Broadcast();

    //RemoveTalent
    for(auto& TalentEquipped :m_EquippedTalent)
    {
        ClearAbility(TalentEquipped.Value);
    }

    m_EquippedTalent.Reset();
}

void UPlayerDiabloAbilitySystemComp::OnTalentLevelChanged(UTechnologyAsset* tech, int level)
{
    UDiaTechnologyAsset* DiaTechAsset = Cast<UDiaTechnologyAsset>(tech);

    if(DiaTechAsset->m_bIsTierBaseNode)
    {
        return;
    }

    if(!DiaTechAsset->m_TalentAbility)
    {
        return;
    }

    if(!DiaTechAsset->m_bIsUnlocked)
    {
        return;
    }

    FGameplayAbilitySpecHandle Handle;

    if(level<=0)
    {
        m_EquippedTalent.RemoveAndCopyValue(DiaTechAsset,Handle);
        
        ClearAbility(Handle);

        return;
    }

    if(m_EquippedTalent.Contains(DiaTechAsset))
    {
        m_EquippedTalent.RemoveAndCopyValue(DiaTechAsset,Handle);
        
        ClearAbility(Handle);
    }

    PRINTF("Add-TalentLevel:%d",level);
    
    FGameplayAbilitySpec Spec = FGameplayAbilitySpec(DiaTechAsset->m_TalentAbility,level, -1, this);
        
    Handle = GiveAbility(Spec);

    TryActivateAbility(Handle);
        
    m_EquippedTalent.Add(DiaTechAsset,Handle);    
}

void UPlayerDiabloAbilitySystemComp::SetSkillFromSaveData(TArray<FSkillDataSpec>& my,
                                                          const TArray<FSkillDataSpec>& loadedData)
{
    for (int i = 0; i < my.Num(); i++)
    {
        my[i].m_nCurrentLevel = loadedData[i].m_nCurrentLevel;
        my[i].m_nEquipIndex = loadedData[i].m_nEquipIndex;
    }
}


int UPlayerDiabloAbilitySystemComp::GetTalentPoint()
{
    return m_TechManager->GetAssignedTechTree()->m_nRemainTalentPoint;
}

int UPlayerDiabloAbilitySystemComp::GetTotalTalentPoint()
{
    return m_TechManager->GetAssignedTechTree()->m_nTotalSpentTalentPoint;
}

bool UPlayerDiabloAbilitySystemComp::IsTalentUnlock(UDiaTechnologyAsset* tech)
{
    return m_TechManager->IsTechnologyUnlocked(tech);
}

void UPlayerDiabloAbilitySystemComp::LevelupTalent(UDiaTechnologyAsset* talentTech)
{
    if(m_TechManager->LevelupTechnology(talentTech))
    {
        m_OnTalentChanged.Broadcast();
    }
}

void UPlayerDiabloAbilitySystemComp::UnlockTalent(UDiaTechnologyAsset* tech)
{
    m_TechManager->UnlockTechnology(tech);
}

UTechnologyTree* UPlayerDiabloAbilitySystemComp::GetCurrentTalentTree()
{
    return m_TechManager->GetAssignedTechTree();
}

void UPlayerDiabloAbilitySystemComp::AssignTechTreeWidget(UTechnologyTree* tree,UTechTreeWidget* widget)
{
    m_TechManager->AssignTechTreeWidget(tree,widget);
    m_TechManager->LoadTechStateFromSlot(UTechnologySaveGame::TalentSaveSlotName,UPlayerCreateManager::Get->m_CurrentSelectSlot);
    m_TechManager->GetAssignedTechTree()->OnTechLevelChanged.AddUObject(this,&UPlayerDiabloAbilitySystemComp::OnTalentLevelChanged);

    for(auto& TechTuple : m_TechManager->GetAssignedTechTree()->UnlockedTechnologies)
    {
        OnTalentLevelChanged(TechTuple.Key,TechTuple.Key->m_nCurrentTalentLevel);
    }
}

void UPlayerDiabloAbilitySystemComp::SetLoadedTalentTree(FName className,UTechnologySaveGame* saveGame)
{
    m_ClassName =className;
    //m_TechManager->InitTechTree(tree);
    //m_TechManager->LoadTechFromObject(saveGame);
}

FName UPlayerDiabloAbilitySystemComp::GetCharacterClass()
{
    return m_ClassName;
}



