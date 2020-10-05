// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterPawn.h"

AMonsterPawn::AMonsterPawn(const FObjectInitializer& objInit):Super(objInit)
{
    
}

void AMonsterPawn::BeginPlay()
{
    Super::BeginPlay();
    m_NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

    if (!m_MonsterUnitHandle.IsNull())
    {
        SetUnitStat(m_MonsterUnitHandle, 1);
    }
}

void AMonsterPawn::SetUnitStat(FDataTableRowHandle unitID, int level)
{
    SetCharacterLevel(level);
    const FMonsterTable* const UnitData = unitID.GetRow<FMonsterTable>("");
    m_TextUnitName = UnitData->m_ShowingName;
    m_SkBody->SetSkeletalMesh(UnitData->m_Mesh);
    m_SkBody->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    m_SkBody->SetAnimInstanceClass(UnitData->m_AnimBP);
    m_GEUnitStat = UnitData->m_DefaultStatTable;//몬스터 랜덤 데이터가 마치 아이템 옵션처럼 몬스터에게 붙어야한다.
    check(m_GEUnitStat);
    m_DeathMontage = UnitData->m_DeathMontage;
    SetUnitStatEffect();
    GetAttributeSet()->m_OnStatChanged.AddUObject(this,&AMonsterPawn::SetHealthPercentage);
    SetHealthPercentage(this);
    
}
void AMonsterPawn::SetHealthPercentage(AUnitPawn* target)
{
   UpdateHealthBar(target->GetHpPercentOne());
    PRINTF("HealthPer :%f",target->GetHpPercentOne());
}
