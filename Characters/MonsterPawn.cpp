#include "MonsterPawn.h"

#include "BaseWidgetBlueprint.h"
#include "DiabloPlayerController.h"
#include "DungeonMiniMap.h"
#include "GridFlowMiniMap.h"
#include "MobUnitMovement.h"
#include "PlayerDiabloCharacter.h"
#include "Logic/MonsterSensing.h"
#include "Logic/MobFSMBase.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/DiabloGameMode.h"
#include "Managers/DungeonManager.h"

AMonsterPawn::AMonsterPawn(const FObjectInitializer& objInit):
Super(objInit.SetDefaultSubobjectClass<UMobUnitMovement>("Movement00"))
{
    m_fDropRadius = 400.f;
    m_bUseFSM = false;
    m_Movement->m_bUseRVO = true;
    SetActorTickEnabled(true);
    SetActorTickInterval(0.2f);
    GetCapsule()->SetCapsuleRadius(24.f);
    m_SkBody->SetRelativeLocation(FVector(0, 0, -90.f));
    m_SkBody->SetRelativeRotation(FRotator(0, -90.f, 0));
    m_bIsPlaced = false;
    m_DropDataRow = nullptr;
    m_bIsMoving=false;
    m_CurrentNode=nullptr;
    m_bIsVisible=true;
    //
    m_StShadow = CreateDefaultSubobject<UStaticMeshComponent>("StShadow");
    static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundSt(
             TEXT("StaticMesh'/Game/Models/SM_CharM_Shadow.SM_CharM_Shadow'"));
    m_StShadow->SetStaticMesh(FoundSt.Object);
    m_StShadow->SetupAttachment(m_SkBody);
    m_StShadow->SetRelativeLocation(FVector(0,0,5.f));
    m_StShadow->SetRelativeScale3D(FVector(3.f,3.f,3.f));
    m_StShadow->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    //
    static ConstructorHelpers::FClassFinder<UUserWidget> FoundHpBar(
             TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/Elements/WB_ProgressBarParents.WB_ProgressBarParents_C'"));
    m_WorldHpBar = CreateDefaultSubobject<UFloatingStatusBarWidgetCompo>("WorldHpBar");
    m_WorldHpBar->SetWidgetClass(FoundHpBar.Class);
    m_WorldHpBar->SetDrawSize(FVector2D(150.f,22.f));
    FVector2D Pivot(0.5f,0.5f);
    m_WorldHpBar->SetPivot(Pivot);
    m_WorldHpBar->SetWidgetSpace(EWidgetSpace::Screen);
    m_WorldHpBar->SetupAttachment(m_Capsule);
    m_WorldHpBar->SetRelativeLocation(FVector(0,0,90));
    //m_WorldHpBar->Screen
    //
    m_Movement->NavAgentProps.AgentHeight=88.f;
    m_Movement->NavAgentProps.AgentRadius=24.f;
    //
    static ConstructorHelpers::FClassFinder<UGameplayEffect> FoundGEExp(
    TEXT("Blueprint'/Game/Blueprints/Abilities/GameEffect/GE_EXP.GE_EXP_C'"));
    m_GEExpReward = FoundGEExp.Class;
}

void AMonsterPawn::ShowStatusBar()
{
    m_WorldHpBar->SetVisibility(true);
}

void AMonsterPawn::HideStatusBar()
{
    m_WorldHpBar->SetVisibility(false);
}

bool AMonsterPawn::IsStatusBarActive()
{
    return m_WorldHpBar->IsVisible();
}

void AMonsterPawn::UpdateHealthBar(float perOne)
{
    m_WorldHpBar->SetHealthPercentage(perOne);
}

void AMonsterPawn::BeginPlay()
{
    Super::BeginPlay();
    if (m_bIsPlaced && !m_MonsterUnitHandle.IsNull())
    {
        InitMonster(m_MonsterUnitHandle, m_nCharacterLevel);
    }
    HideStatusBar();
}


void AMonsterPawn::InitMonster(FDataTableRowHandle unitID, int level)
{
    
    
    m_MonsterUnitHandle.RowName = unitID.RowName;
    
    const FMonsterTable* const UnitData = unitID.GetRow<FMonsterTable>("");

    m_DeathMontage = UnitData->m_DeathMontage;
    
    m_StunMontage = UnitData->m_StunMontage;
    
    m_TookHitMontage = UnitData->m_TookHitMontage;

    m_DropDataRow = UnitData->m_RewardDropTableHandle.GetRow<FMonsterItemDropRow>("");
    
    m_TextUnitName = UnitData->m_ShowingName;
    
    m_SkBody->SetSkeletalMesh(UnitData->m_Mesh);

    m_SkBody->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    m_SkBody->SetAnimInstanceClass(UnitData->m_AnimBP);
    
    m_GEUnitStat = UnitData->m_DefaultStatTable; //몬스터 랜덤 데이터가 마치 아이템 옵션처럼 몬스터에게 붙어야한다.

    SetCharacterLevel(level);
    
    check(m_GEUnitStat);
    
    SetUnitStatEffect();

    GetDiaAbilitySystem()->GetGameplayAttributeValueChangeDelegate(GetAttributeSet()->GetHealthAttribute()).AddUObject(
        this, &AMonsterPawn::SetHealthPercentage);

    FOnAttributeChangeData NotUse;
    SetHealthPercentage(NotUse);

    m_MonsterSense = NewObject<UMonsterSensing>(this, UMonsterSensing::StaticClass());
    m_MonsterSense->InitSense(this);

    if (UnitData->m_MobFSM != nullptr)
    {
        m_FSM = NewObject<UMobFSMBase>(this, UnitData->m_MobFSM,UnitData->m_MobFSM->GetFName(), RF_NoFlags,UnitData->m_MobFSM->GetDefaultObject());
        
        m_FSM->Init(this);
        
        m_bUseFSM = true;
    }
    else
    {
        SetActorTickEnabled(false); //Anim?
    }

    if (UnitData->m_BaseAttack)
    {
        FGameplayAbilitySpec BaseAttackHandle(UnitData->m_BaseAttack, level, INDEX_NONE, this);
        
        m_BaseAttackHandle = GetDiaAbilitySystem()->GiveAbility(BaseAttackHandle);
    }

    RegisterToQuadTreeBound();
    if(m_CurrentNode)
    {
        HideAll(false);
    }
}


void AMonsterPawn::GiveExpToPlayer()
{
    FGameplayEffectContextHandle Context = GetDiaAbilitySystem()->MakeEffectContext();
    FGameplayEffectSpecHandle ExpSpecHandle = GetDiaAbilitySystem()->MakeOutgoingSpec(
        m_GEExpReward, GetCharacterLevel(), Context);

    GetDiaAbilitySystem()->ApplyGameplayEffectSpecToTarget(*ExpSpecHandle.Data,
                                                           ADiabloPlayerController::Get->GetPlayerPawn()->
                                                           GetDiaAbilitySystem());
}

void AMonsterPawn::Die()
{
    if(GetCurrentNode())
    {
        GetCurrentNode()->RemoveElement(this);
    }

    if(m_SpawnedManager)
    {
        m_SpawnedManager->MonsterDead();
    }

    m_OnCharacterDied.Broadcast(this);

    SetActorTickEnabled(false);
    
    GetCapsule()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
    GetMovementComponent()->SetActive(false);
    GetMovementComponent()->SetComponentTickEnabled(false);

    m_bUseFSM = false;
    
    m_bIsMoving=false;

    if (IsValid(GetDiaAbilitySystem()))
    {
        GetDiaAbilitySystem()->CancelAllAbilities();

        FGameplayTagContainer EffectTagsToRemove;
        EffectTagsToRemove.AddTag(m_TagEffectRemoveOnDeath);
        int32 NumEffectsRemoved = GetDiaAbilitySystem()->RemoveActiveEffectsWithTags(EffectTagsToRemove);

        GetDiaAbilitySystem()->AddLooseGameplayTag(m_TagDead);

        GetDiaAbilitySystem()->RemoveGameplayCue(m_TagStun);
    }

    GiveExpToPlayer();
    
    RequestDropRewards();

    if (m_DeathMontage)
    {
        float AnimLength = PlayAnim(m_DeathMontage); //- 0.2f;

        if (GEngine->GetNetMode(GetWorld()) < NM_Client)
        {
            FTimerHandle TimerHandle_OnTimer;

            GetWorldTimerManager().SetTimer(TimerHandle_OnTimer, this, &AMonsterPawn::OnDeathAnimEnd,
                                            AnimLength,
                                            false);
        }
    }
    else
    {
        OnDeathAnimEnd();
    }
}

void AMonsterPawn::RequestDropRewards()
{
    if (!m_DropDataRow)
    {
        return;
    }

    UDiabloGameInstance::Get->GetRewardManager()->RequestMonsterDropItem(this, *m_DropDataRow, GetCharacterLevel());
}

void AMonsterPawn::OnDeathAnimEnd()
{

    Destroy();
}

void AMonsterPawn::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (m_bUseFSM)
    {
        m_FSM->TickFSM();
    }
    
    UpdateBound();
}

void AMonsterPawn::SetHealthPercentage(const FOnAttributeChangeData& data)
{
    UpdateHealthBar(GetHpPercentOne());
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

    return m_MonsterSense->LineOfSightTo(GetFocusedTarget(), FVector::ZeroVector, true);
}

FVector AMonsterPawn::GetLastSeenLocation()
{
    return m_MonsterSense->m_LastPlayerSeen;
}

FVector AMonsterPawn::GetActorLocation()
{
    return AActor::GetActorLocation();
}

void AMonsterPawn::RegisterToQuadTreeBound()
{
    ADiabloGameMode::Get->RegisterQuadElement(this);
}

void AMonsterPawn::ShowAll(bool hasBeenShowed)
{
    if(m_bIsVisible)
    {
        return;	
    }
	
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);
    SetActorTickEnabled(true);
    m_SkBody->SetComponentTickEnabled(true);
    m_Movement->SetComponentTickEnabled(true);
    m_MonsterSense->SetSensingUpdatesEnabled(true);
    m_PFComp->SetComponentTickEnabled(true);
    m_StShadow->SetComponentTickEnabled(true);
    m_WorldHpBar->SetComponentTickEnabled(true);
    m_bIsVisible=true;
}

void AMonsterPawn::HideAll(bool hasBeenShowed)
{
    if(!hasBeenShowed)
    {
        SetActorTickEnabled(false);
        m_Movement->SetComponentTickEnabled(false);
        m_MonsterSense->SetSensingUpdatesEnabled(false);
        m_PFComp->SetComponentTickEnabled(false);
        
    }
    m_SkBody->SetComponentTickEnabled(false);
    m_StShadow->SetComponentTickEnabled(false);
    m_WorldHpBar->SetComponentTickEnabled(false);
    
    SetActorEnableCollision(false);

    SetActorHiddenInGame(true);
    m_bIsVisible=false;

}

void AMonsterPawn::SetNode(QuadtreeNode* quadtree_node)
{
    m_CurrentNode = quadtree_node;
  
}

QuadtreeNode* AMonsterPawn::GetCurrentNode()
{
    return m_CurrentNode;
}


void AMonsterPawn::UpdateBound()//여기하는중
{
    if(m_CurrentNode)//TODO Need Erase
    {
        if(!GetCurrentNode()->IsPositionInsideNode(GetActorLocation()))
        {
            RegisterToQuadTreeBound();

            if(m_CurrentNode)//Succed register tree
            {
                ShowAll(true);
            }
        }
    }
    else//out of bound
    {
        RegisterToQuadTreeBound();

        //ShowAll(true);

    }
}
