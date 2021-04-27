#include "MonsterPawn.h"
#include "DiabloPlayerController.h"
#include "MobUnitMovement.h"
#include "PlayerDiabloCharacter.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"
#include "Logic/MonsterSensing.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/DungeonManager.h"

AMonsterPawn::AMonsterPawn(const FObjectInitializer& objInit):
Super(objInit.SetDefaultSubobjectClass<UMobUnitMovement>("Movement00"))
{
    m_bDeathAnimEnd=true;
    m_bUseFSM = true;
    m_Movement->m_bUseRVO = true;
    SetActorTickEnabled(true);
    SetActorTickInterval(0.2f);
    GetCapsule()->SetCapsuleRadius(24.f);
    m_SkBody->SetRelativeLocation(FVector(0, 0, -90.f));
    m_SkBody->SetRelativeRotation(FRotator(0, -90.f, 0));
    //
    m_StShadow = CreateDefaultSubobject<UStaticMeshComponent>("StShadow");
    static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundSt(
             TEXT("StaticMesh'/Game/Models/SM_CharM_Shadow.SM_CharM_Shadow'"));
    m_StShadow->SetStaticMesh(FoundSt.Object);
    m_StShadow->SetupAttachment(m_SkBody);
    m_StShadow->SetRelativeLocation(FVector(0,0,5.f));
    m_StShadow->SetRelativeScale3D(FVector(4.f));
    m_StShadow->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    m_StShadow->SetCanEverAffectNavigation(false);
    //
    //
    static ConstructorHelpers::FClassFinder<UUserWidget> FoundHpBar(
             TEXT("WidgetBlueprint'/Game/Blueprints/Widget/WorldWidget/WB_MonsterHealthBar.WB_MonsterHealthBar_C'"));
    m_WorldHpBar = CreateDefaultSubobject<UFloatingStatusBarWidgetCompo>("WorldHpBar");
    m_WorldHpBar->SetWidgetClass(FoundHpBar.Class);
    m_WorldHpBar->SetDrawSize(FVector2D(150.f,22.f));
    FVector2D Pivot(0.5f,0.5f);
    m_WorldHpBar->SetPivot(Pivot);
    m_WorldHpBar->SetWidgetSpace(EWidgetSpace::Screen);
    m_WorldHpBar->SetupAttachment(m_Capsule);
    m_WorldHpBar->SetRelativeLocation(FVector(0,0,90));
    m_WorldHpBar->SetCanEverAffectNavigation(false);
    //
    m_HittenAudio= CreateDefaultSubobject<UAudioComponent>("Audio01");
    m_HittenAudio->SetupAttachment(RootComponent);
    m_DeathAudio= CreateDefaultSubobject<UAudioComponent>("Audio02");
    m_DeathAudio->SetupAttachment(RootComponent);
    m_CoinAudio= CreateDefaultSubobject<UAudioComponent>("Audio03");
    m_CoinAudio->SetupAttachment(RootComponent);
    //m_WorldHpBar->Screen
    //
    m_Movement->NavAgentProps.AgentHeight=88.f;
    m_Movement->NavAgentProps.AgentRadius=24.f;
    //

    m_SkBody->CastShadow = false;

    m_TeamID = ETeamID::Monster;

    m_fCurrentHP = 0;
    m_fMaxHP = 0;
    //
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> FoundSkMesh(
              TEXT("SkeletalMesh'/Game/Models/ParagonMeshs/Greystone_SK.Greystone_SK'"));

    m_SkBody->SetSkeletalMesh(FoundSkMesh.Object);

    //ParticleSystem'/Game/03_VisualEffect/PS_CoinDrop.PS_CoinDrop'

    static ConstructorHelpers::FObjectFinder<UParticleSystem> FoundCoinParticle(
          TEXT("ParticleSystem'/Game/03_VisualEffect/PS_CoinDrop.PS_CoinDrop'"));
    m_Particle = CreateDefaultSubobject<UParticleSystemComponent>("ParticleCoin01");
    m_Particle->SetupAttachment(RootComponent);
    m_Particle->SetTemplate(FoundCoinParticle.Object);
    m_Particle->SetAutoActivate(false);
    m_Particle->Deactivate();
    //
    static ConstructorHelpers::FObjectFinder<UParticleSystem> FoundHitParticle(
         TEXT("ParticleSystem'/Game/03_VisualEffect/P_Hit.P_Hit'"));
    m_HitParticle = CreateDefaultSubobject<UParticleSystemComponent>("ParticleHit02");
    m_HitParticle->SetupAttachment(RootComponent);
    m_HitParticle->SetTemplate(FoundHitParticle.Object);
    m_HitParticle->SetRelativeLocation(FVector(0,0,70.f));
    m_HitParticle->SetAutoActivate(false);
    m_HitParticle->SetRelativeScale3D(FVector(1));
    m_HitParticle->Deactivate();
    

    //soundcoin//SoundWave'/Game/Sound/Coins_01.Coins_01'
    static ConstructorHelpers::FObjectFinder<USoundBase> FoundSound1(
          TEXT("SoundWave'/Game/Sound/Coins_01.Coins_01'"));
    //soundhitten//SoundWave'/Game/Sound/Fantasy_Game_Weapon_Impact.Fantasy_Game_Weapon_Impact'
    static ConstructorHelpers::FObjectFinder<USoundBase> FoundSound2(
          TEXT("SoundWave'/Game/Sound/Fantasy_Game_Weapon_Impact.Fantasy_Game_Weapon_Impact'"));
    //soundeath//SoundWave'/Game/Sound/Fantasy_Game_Creature_Growl_3.Fantasy_Game_Creature_Growl_3'
    static ConstructorHelpers::FObjectFinder<USoundBase> FoundSound3(
          TEXT("SoundWave'/Game/Sound/Fantasy_Game_Creature_Growl_3.Fantasy_Game_Creature_Growl_3'"));
    m_HittenAudio->SetSound(FoundSound2.Object);
    m_CoinAudio->SetSound(FoundSound1.Object);
    m_DeathAudio->SetSound(FoundSound3.Object);
    m_DeathAudio->SetAutoActivate(false);
    m_HittenAudio->SetAutoActivate(false);
    m_CoinAudio->SetAutoActivate(false);
    m_CoinAudio->VolumeMultiplier = 10.f;
}

void AMonsterPawn::BeginPlay()
{
    Super::BeginPlay();

    m_TickFSM = NewObject<UFSMTick>(this, UFSMTick::StaticClass());
    m_TickFSM->Init(this);
    
    m_WorldHpBar->SetComponentTickEnabled(false);

    m_PlCon = Cast<ADiabloPlayerController>( UGameplayStatics::GetPlayerController(this,0));
}

void AMonsterPawn::RequestDropItem()
{
}

void AMonsterPawn::RequestGetGoldBounty()
{
    UDiabloGameInstance::Get->m_GoldManager->AddGold(m_fGoldBounty);
}

void AMonsterPawn::DataInject(const FMonsterEntity* monster_table, const BigInt& hp,const BigInt& gold,EMonsterType type,const FItemDropTableRow* dropTable,float statFactor ,float scaleFactor,float goldFactor)//droptable
{
    m_DropTable = dropTable;
    
    m_MonsterType = type;

    if(m_MonsterType == EMonsterType::Boss)
    {
        FocusTarget(Cast<AUnitPawn>( UGameplayStatics::GetPlayerPawn(this,0)));
    }
    
    m_bDeathAnimEnd = false;
    
    GetWorldTimerManager().ClearTimer(m_DeathTimer);
    
    const FMonsterEntity* const UnitData = monster_table;
    
    m_Movement->SetMoveSpeed(UnitData->m_fMoveSpeed);

    m_BaseAttackAnim =  UnitData->m_BaseAttackAnim;

    m_DeathMontage = UnitData->m_DeathMontage;

    m_SpawnAnim = UnitData->m_SpawnAnim;
    
    m_TookHitMontage = UnitData->m_TookHitMontage;

    m_TextUnitName = UnitData->m_ShowingName;

    m_SkBody->EmptyOverrideMaterials();
    
    m_SkBody->SetSkeletalMesh(UnitData->m_MonsterMeshSoft.Get());

    m_SkBody->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    
    m_SkBody->SetAnimInstanceClass(UnitData->m_AnimBP);

    m_fAttackRange = UnitData->m_fAttackRange;

    m_fAttackSpeed = UnitData->m_fAttackSpeed;

    m_fGoldBounty = gold;
    m_fGoldBounty.Multiply(goldFactor);

    m_fMaxHP = hp;
    m_fMaxHP.Multiply(statFactor);
    
    m_fCurrentHP = m_fMaxHP;

    UpdateHealthBar(GetHpPercentOne());

    m_SkBody->SetScalarParameterValueOnMaterials("Visibility",1.f);

    m_SkBody->SetRelativeScale3D(FVector(scaleFactor));

    if(m_SpawnAnim)
    {
        PlayAnimMontage(m_SpawnAnim);
    }
    
    m_TickFSM->Init(this);
  
    SetAcive(true);

    m_TickFSM->ResetStartPosition(GetActorLocation());
}

void AMonsterPawn::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    m_fHitAnimCD-=DeltaSeconds;

    m_TickFSM->TickFSM();
}

bool AMonsterPawn::IsStatusBarActive()
{
    return m_WorldHpBar->IsComponentTickEnabled();
}

void AMonsterPawn::UpdateHealthBar(float perOne)
{
    m_WorldHpBar->SetHealthPercentage(perOne);
    m_OnTookDmg.Broadcast(perOne);
}


void AMonsterPawn::HideStatusBar()
{
    m_WorldHpBar->SetComponentTickEnabled(false);
    m_WorldHpBar->SetHiddenInGame(true);
    m_WorldHpBar->SetVisibility(false);
    m_WorldHpBar->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Hidden);
}

void AMonsterPawn::Die()
{
    APlayerDiabloCharacter* Pawn = Cast<APlayerDiabloCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    Pawn->FocusTarget(nullptr);
    //Pawn->GainRagePoint();

    UDiabloGameInstance::Get->m_QuestManager->AddQuestCount(EQuestType::MonsterKill);
    
    UDiabloGameInstance::Get->m_MonsterSpawn->AddKillCount();
    RequestDropItem();
    RequestGetGoldBounty();
    
    m_Particle->Activate(true);
    m_CoinAudio->Play();
    m_DeathAudio->Play();
    HideStatusBar();
    
    SetActorTickEnabled(false);
    SetActorEnableCollision(false);
    GetMovementComponent()->SetActive(false);
    GetMovementComponent()->SetComponentTickEnabled(false);
    FocusTarget(nullptr);
    
    if (m_DeathMontage)
    {
        float AnimLength = PlayAnimMontage(m_DeathMontage) - 0.2f; //- 0.2f;
    
        if (GEngine->GetNetMode(GetWorld()) < NM_Client)
        {
            GetWorldTimerManager().SetTimer(m_DeathTimer, this, &AMonsterPawn::OnDeathAnimEnd,AnimLength,false);
        }
    }
    else
    {
        OnDeathAnimEnd();
    }
}


void AMonsterPawn::OnDeathAnimEnd()
{
    m_Particle->Activate(false);
    FVector NewHide;
    NewHide.X=10000.f;
    NewHide.Y=10000.f;
    NewHide.Z = 1000.f;
    
    m_fCurrentHP=-1;
    
    SetActorLocation(NewHide);
    
    StopAnimMontage(m_DeathMontage);
    SetAcive(false);
    m_bDeathAnimEnd = true;

    m_OnDead.Broadcast(this);
    //Destroy();
}

void AMonsterPawn::PlayHittenSound()
{
    m_HittenAudio->Play();
}

void AMonsterPawn::ShowStatusBar()
{
    m_WorldHpBar->SetComponentTickEnabled(true);
    m_WorldHpBar->SetHiddenInGame(false);
    m_WorldHpBar->SetVisibility(true);
    m_WorldHpBar->GetUserWidgetObject()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void AMonsterPawn::TakeDmg(BigInt amount, AUnitPawn* attacker,EDamagePopup pp)
{
    if(!GetFocusedTarget())
    {
        FocusTarget(attacker);
    }

    PlayTookHitMontage();
    PlayHitFlash();
    PlayHittenSound();
    m_HitParticle->Activate(true);

    if(!IsStatusBarActive() && m_MonsterType != EMonsterType::Boss)
    {
        ShowStatusBar();
    }
        
    m_fCurrentHP.Subtract(amount);

    m_PlCon->ShowDamageNumber(amount,this,pp);

    if(m_fCurrentHP.IsLessThanZero()||m_fCurrentHP.IsZero())
    {
        UpdateHealthBar(0.f);
        Die();

        return;
    }

    UpdateHealthBar(GetHpPercentOne());

    
}

FVector AMonsterPawn::GetLastSeenLocation()
{
    return FVector::ZeroVector;
}

void AMonsterPawn::PlayTookHitMontage()
{
    if(m_TookHitMontage && m_fHitAnimCD<0.f)
    {
        PlayAnimMontage(m_TookHitMontage,1);

        m_fHitAnimCD = FMath::RandRange(1.5f,2.5f);
    }
}


void AMonsterPawn::FocusTarget(AUnitPawn* target)
{
    m_FocusedEnemy = target;
}

void AMonsterPawn::PlayHitFlash()
{
	FName TimeParamName = "StartTime";
	
	float TimeSec = UGameplayStatics::GetTimeSeconds(GetWorld());

	m_SkBody->SetScalarParameterValueOnMaterials(TimeParamName, TimeSec);

}



bool AMonsterPawn::IsAlive() const
{
    return m_fCurrentHP > 0 ; 
}

bool AMonsterPawn::IsReadyToPool()
{
    return m_bDeathAnimEnd;
}

void AMonsterPawn::SetAcive(bool v)
{
    SetActorHiddenInGame(!v);
    SetActorEnableCollision(v);
    SetActorTickEnabled(v);
    GetMovementComponent()->SetActive(v);
    GetMovementComponent()->SetComponentTickEnabled(v);
    
    if(!v)
    {
    //    HideStatusBar();
        FocusTarget(nullptr);
    }
}

float AMonsterPawn::TryAttack()
{
    if(m_BaseAttackAnim&&m_fAttackCD<0.f)
    {
        float AnimMongLen = PlayAnimMontage(m_BaseAttackAnim,1*m_fAttackSpeed,NAME_None);

        if(AnimMongLen>m_fAttackCDConstant)
        {
            m_fAttackCD = AnimMongLen - 0.1f;
        }
        else
        {
            m_fAttackCD =m_fAttackCDConstant;
        }

        return AnimMongLen;;
    }

    return 0.f;
    // PlayAnimMontage(m_BaseAttackAnim, 1 * GetAttackSpeed(),sectionSkillName? *sectionSkillName: SectionName);
    //
    // float AnimMongLen = m_BaseAttackAnim->GetSectionLength(sectionSkillName?m_BaseAttackAnim->GetSectionIndex(*sectionSkillName): (int)DmgType) / GetAttackSpeed();
    //
    // if(AnimMongLen>maxCd && !bUseMagic)
    // {
    //     AnimMongLen=maxCd;
    // }
    //
    // currentCd = AnimMongLen-0.1f;//-0.1f;
    // //
    // GetWorldTimerManager().ClearTimer(m_AttackTimer);
}

