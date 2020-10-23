#include "MonsterPawn.h"

#include "DiabloPlayerController.h"
#include "PlayerDiabloCharacter.h"
#include "Logic/MonsterSensing.h"
#include "Logic/MobFSMBase.h"

AMonsterPawn::AMonsterPawn(const FObjectInitializer& objInit): Super(objInit)
{
    m_bUseFSM = false;
    m_Movement->m_bUseRVO=true;
    SetActorTickEnabled(true);
    SetActorTickInterval(0.2f);
    GetCapsule()->SetCapsuleRadius(24.f);
    m_SkBody->SetRelativeLocation(FVector( 0,0,-90.f));
    m_SkBody->SetRelativeRotation(FRotator(0,-90.f,0));
    
    
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
    SetUnitStatEffect();
    
    GetDiaAbilitySystem()->GetGameplayAttributeValueChangeDelegate(GetAttributeSet()->GetHealthAttribute()).AddUObject(this, &AMonsterPawn::SetHealthPercentage);
    
    FOnAttributeChangeData NotUse;
    SetHealthPercentage(NotUse);

    m_MonsterSense = NewObject<UMonsterSensing>(this,UMonsterSensing::StaticClass());
    m_MonsterSense->InitSense(this);

    if(UnitData->m_MobFSM !=nullptr)
    {
        m_FSM = NewObject<UMobFSMBase>(this, UnitData->m_MobFSM,
        UnitData->m_MobFSM->GetFName(),RF_NoFlags,UnitData->m_MobFSM->GetDefaultObject());
        m_FSM->Init(this);
        m_bUseFSM = true;
    }
    else
    {
        SetActorTickEnabled(false);//Anim?
    }
    
    if(UnitData->m_BaseAttack)
    {
        FGameplayAbilitySpec BaseAttackHandle(UnitData->m_BaseAttack,level,INDEX_NONE,this);
        m_BaseAttackHandle = GetDiaAbilitySystem()->GiveAbility(BaseAttackHandle);
    }

    m_DeathMontage = UnitData->m_DeathMontage;
    m_StunMontage = UnitData->m_StunMontage;
    m_TookHitMontage= UnitData->m_TookHitMontage;
}

void AMonsterPawn::OnDeathAnimEnd()
{
    // 경험치 증가
    FGameplayEffectContextHandle Context = GetDiaAbilitySystem()->MakeEffectContext();
    FGameplayEffectSpecHandle ExpSpecHandle = GetDiaAbilitySystem()->MakeOutgoingSpec(m_GEExpReward, GetCharacterLevel(),Context);

    GetDiaAbilitySystem()->ApplyGameplayEffectSpecToTarget(*ExpSpecHandle.Data,
        ADiabloPlayerController::Get->GetPlayerPawn()->GetDiaAbilitySystem());

//Item?
    Destroy();
}

void AMonsterPawn::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    
    if(m_bUseFSM)
    {
        m_FSM->TickFSM();
    }
}

void AMonsterPawn::SetHealthPercentage(const FOnAttributeChangeData& data)
{
    UpdateHealthBar(GetHpPercentOne());
    PRINTF("HealthPer :%f", GetHpPercentOne());
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

void AMonsterPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    PRINTF("End - Mobs");
    switch (EndPlayReason)
    {
    case EEndPlayReason::Destroyed: PRINTF("Destroyed"); break;
    case EEndPlayReason::LevelTransition: PRINTF("LevelTrans"); break;
    case EEndPlayReason::EndPlayInEditor: PRINTF("Editor End"); break;
    case EEndPlayReason::RemovedFromWorld: PRINTF("RemoveWorld"); break;
    case EEndPlayReason::Quit: PRINTF("Quit"); break;
    default: ;
    }
   
}
