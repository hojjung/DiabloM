#include "MonsterPawn.h"
#include "DiabloPlayerController.h"
#include "DungeonMiniMap.h"
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
    m_bHasShownEver=false;
    m_bUseFSM = false;
    m_Movement->m_bUseRVO = true;
    SetActorTickEnabled(true);
    SetActorTickInterval(0.2f);
    GetCapsule()->SetCapsuleRadius(24.f);
    m_SkBody->SetRelativeLocation(FVector(0, 0, -90.f));
    m_SkBody->SetRelativeRotation(FRotator(0, -90.f, 0));
    m_bIsPlaced = false;
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
    m_StShadow->SetCanEverAffectNavigation(false);
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
    m_WorldHpBar->SetCanEverAffectNavigation(false);
    //m_WorldHpBar->Screen
    //
    m_Movement->NavAgentProps.AgentHeight=88.f;
    m_Movement->NavAgentProps.AgentRadius=24.f;
    //
    static ConstructorHelpers::FClassFinder<UGameplayEffect> FoundGEExp(
    TEXT("Blueprint'/Game/Blueprints/Abilities/GameEffect/GE_EXP.GE_EXP_C'"));
    m_GEExpReward = FoundGEExp.Class;

    m_SkBody->CastShadow = false;
}

void AMonsterPawn::ShowStatusBar()
{
    m_WorldHpBar->SetHiddenInGame(false);
}

void AMonsterPawn::HideStatusBar()
{
    m_WorldHpBar->SetHiddenInGame(true);
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


void AMonsterPawn::InitMonster(FDataTableRowHandle unitID, int level,UDungeonManager* dgManager)
{
    m_TeamID = ETeamID::Monster;
    
    m_SpawnedManager = dgManager;
    
    m_MonsterUnitHandle.RowName = unitID.RowName;
    
    const FMonsterTable* const UnitData = unitID.GetRow<FMonsterTable>("");

    m_DeathMontage = UnitData->m_DeathMontage;

    m_SpawnAnim = UnitData->m_SpawnAnim;
    
    m_TookHitMontage = UnitData->m_TookHitMontage;

    m_TextUnitName = UnitData->m_ShowingName;
    
    m_SkBody->SetSkeletalMesh(UnitData->m_Mesh);

    m_SkBody->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    
    m_SkBody->SetAnimInstanceClass(UnitData->m_AnimBP);
    
    //m_PhyAsset
    
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
        m_bUseFSM = false;
        //SetActorTickEnabled(false); //Anim?
    }

    if (UnitData->m_BaseAttack)
    {
        FGameplayAbilitySpec BaseAttackHandle(UnitData->m_BaseAttack, level, INDEX_NONE, this);
        
        m_BaseAttackHandle = GetDiaAbilitySystem()->GiveAbility(BaseAttackHandle);
    }

    RegisterToQuadTreeBound();
    
    if(m_CurrentNode)
    {
        HideAll();
    }

    m_AttributeSet->m_OnDmgTook.AddUObject(this,&AMonsterPawn::PlayHitFlash);
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
    FMonsterTypeRow* MobType = GetMonsterDataTable().m_TypeHandle.GetRow<FMonsterTypeRow>("");

    UGameplayStatics::PlaySoundAtLocation(GetWorld(), MobType->m_DeathSound, GetActorLocation(), 1, 1);
    
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

    FTimerHandle TimerHandle_OnTimer;

    if (m_DeathMontage)
    {
        float AnimLength = PlayAnim(m_DeathMontage); //- 0.2f;

        if (GEngine->GetNetMode(GetWorld()) < NM_Client)
        {
            GetWorldTimerManager().SetTimer(TimerHandle_OnTimer, this, &AMonsterPawn::OnDeathAnimEnd,AnimLength,false);
        }
    }
    else
    {
        OnDeathAnimEnd();
    }
}

void AMonsterPawn::RequestDropRewards()
{
    UDiabloGameInstance::Get->GetRewardManager()->RequestMonsterDropItem(this, GetCharacterLevel());
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

void AMonsterPawn::ShowAll( )
{
    if(m_bIsVisible)
    {
        return;	
    }

    if(!m_bHasShownEver)
    {
        m_bHasShownEver=true;

        if(m_SpawnAnim)
        {
            PlayAnimMontage(m_SpawnAnim,1);    
        }
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

void AMonsterPawn::HideAll()
{
    if(!m_bHasShownEver)
    {
        SetActorTickEnabled(false);
        m_Movement->SetComponentTickEnabled(false);
        m_MonsterSense->SetSensingUpdatesEnabled(false);
        m_PFComp->SetComponentTickEnabled(false);
    }
    
    m_SkBody->SetComponentTickEnabled(false);//이게여기있으면 몬스터 애니가 멈춤//그래도 퍼포먼스
    m_StShadow->SetComponentTickEnabled(false);
    m_WorldHpBar->SetComponentTickEnabled(false);
    
    SetActorEnableCollision(false);

    SetActorHiddenInGame(true);
    m_bIsVisible=false;
    HideStatusBar();
}

void AMonsterPawn::SetNode(QuadtreeNode* quadtree_node)
{
    m_CurrentNode = quadtree_node;
  
}

QuadtreeNode* AMonsterPawn::GetCurrentNode()
{
    return m_CurrentNode;
}


void AMonsterPawn::UpdateBound() //여기하는중,하는중이였네,
{
    if (m_CurrentNode) //TODO Need Erase
    {
        if (!GetCurrentNode()->IsPositionInsideNode(GetActorLocation()))//현재 노드 밖으로 캐릭터가 나갔다.
        {
            RegisterToQuadTreeBound();

            if(!m_CurrentNode)
             {
                return;//Fail
             }

            if(ADiabloGameMode::Get->CheckActorInVisibleNode(this))
            {
                ShowAll();
            }
            else
            {
                HideAll();
            }
        }
    }
    else
    {
        RegisterToQuadTreeBound();
    }
}

void AMonsterPawn::PlayHitFlash(float notUseDmg)
{
	FName TimeParamName = "StartTime";
	
	float TimeSec = UGameplayStatics::GetTimeSeconds(GetWorld());

	m_SkBody->SetScalarParameterValueOnMaterials(TimeParamName, TimeSec);

    FMonsterTypeRow* MobType = GetMonsterDataTable().m_TypeHandle.GetRow<FMonsterTypeRow>("");

    if (MobType->m_HittenSound)
    {
        if (MobType->m_HittenSound->IsLooping())
        {
            PRINTF("SoundISLooping");
            return;
        }

        UGameplayStatics::PlaySoundAtLocation(GetWorld(), MobType->m_HittenSound, GetActorLocation(), 1, 1);
    }
}

const FMonsterTable& AMonsterPawn::GetMonsterDataTable() const
{
    return *m_MonsterUnitHandle.GetRow<FMonsterTable>("");
}
