#include "PlayerDiabloAbilitySystemComp.h"

#include "Characters/PlayerDiabloCharacter.h"

UPlayerDiabloAbilitySystemComp::UPlayerDiabloAbilitySystemComp()
{
    m_SkillDataTableRow = nullptr;
    m_AryBaseSkill.Reset();
    m_AryPowerSkill.Reset();
    m_AryDefensvieSkill.Reset();
    m_ArySpecialSkill.Reset();
    m_AryMasterySkill.Reset();
    m_AryUltimateSkill.Reset();
    m_nSkillPoints = 10; //Test
    m_nTotalSkillPointSpents = 0;
}

void UPlayerDiabloAbilitySystemComp::BeginPlay()
{
    Super::BeginPlay();

    m_PlayerPawn = Cast<APlayerDiabloCharacter>(GetOwner());
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

int UPlayerDiabloAbilitySystemComp::GetSkillPoints()
{
    return m_nSkillPoints;
}

void UPlayerDiabloAbilitySystemComp::TickComponent(float DeltaTime, ELevelTick TickType,
                                                   FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
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
