#include "MonsterPawn.h"

AMonsterPawn::AMonsterPawn(const FObjectInitializer& objInit): Super(objInit)
{
    m_bUseFSM = true;
}

void AMonsterPawn::BeginPlay()
{
    Super::BeginPlay();

    if (!m_MonsterUnitHandle.IsNull())
    {
        InitMonster(m_MonsterUnitHandle, m_nCharacterLevel);
    }
}

void AMonsterPawn::InitMonster(FDataTableRowHandle unitID, int level)
{
    SetCharacterLevel(level);
    const FMonsterTable* const UnitData = unitID.GetRow<FMonsterTable>("");
    m_TextUnitName = UnitData->m_ShowingName;
    m_SkBody->SetSkeletalMesh(UnitData->m_Mesh);
    m_SkBody->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    m_SkBody->SetAnimInstanceClass(UnitData->m_AnimBP);
    m_GEUnitStat = UnitData->m_DefaultStatTable; //몬스터 랜덤 데이터가 마치 아이템 옵션처럼 몬스터에게 붙어야한다.
    check(m_GEUnitStat);
    m_DeathMontage = UnitData->m_DeathMontage;
    SetUnitStatEffect();
    GetAttributeSet()->m_OnStatChanged.AddUObject(this, &AMonsterPawn::SetHealthPercentage);
    SetHealthPercentage(this);

    m_MonsterSense = NewObject<UMonsterSensing>(this, UMonsterSensing::StaticClass());
    m_MonsterSense->InitSense(this);

    if(UnitData->m_BaseAttack)
    {
        FGameplayAbilitySpec BaseAttackHandle(UnitData->m_BaseAttack,level,INDEX_NONE,this);
        m_BaseAttackHandle = GetDiaAbilitySystem()->GiveAbility(BaseAttackHandle);
    }
}

void AMonsterPawn::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    m_MonsterSense->Tick();
}

void AMonsterPawn::SetHealthPercentage(AUnitPawn* target)
{
    UpdateHealthBar(target->GetHpPercentOne());
    PRINTF("HealthPer :%f", target->GetHpPercentOne());
}

bool AMonsterPawn::HasDropItem()
{
    //TODO Drop table make
    return true;
}

void AMonsterPawn::FocusTarget(AUnitPawn* target)
{
    m_FocusedEnemy = target;
}

bool AMonsterPawn::CanSeeTarget()
{
    if (!GetFocusedTarget())
    {
        return false;
    }
    
    return m_MonsterSense->LineOfSightTo(GetFocusedTarget(),FVector::ZeroVector, true);
}

FVector AMonsterPawn::GetLastSeenLocation()
{
    return m_MonsterSense->m_LastPlayerSeen;
}
