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
    m_nAvoidLevel=0;
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
    //
    m_HittenAudio= CreateDefaultSubobject<UAudioComponent>("Audio01");
    m_DeathAudio= CreateDefaultSubobject<UAudioComponent>("Audio022");
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

}

void AMonsterPawn::BeginPlay()
{
    Super::BeginPlay();

    m_TickFSM = NewObject<UFSMTick>(this, UFSMTick::StaticClass());
    m_TickFSM->Init(this);
    
    m_WorldHpBar->SetComponentTickEnabled(false);

    m_PlCon = Cast<ADiabloPlayerController>( UGameplayStatics::GetPlayerController(this,0));
}

void AMonsterPawn::DataInject(const FMonsterEntity* monster_table, const BigInt& hp)
{
    m_bDeathAnimEnd = false;
    
    GetWorldTimerManager().ClearTimer(m_DeathTimer);
    
    const FMonsterEntity* const UnitData = monster_table;

    m_BaseAttackAnim =  UnitData->m_BaseAttackAnim;

    m_DeathMontage = UnitData->m_DeathMontage;

    m_SpawnAnim = UnitData->m_SpawnAnim;
    
    m_TookHitMontage = UnitData->m_TookHitMontage;

    m_TextUnitName = UnitData->m_ShowingName;
    
    m_SkBody->SetSkeletalMesh(UnitData->m_Mesh);

    m_SkBody->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    
    m_SkBody->SetAnimInstanceClass(UnitData->m_AnimBP);

    m_HittenSound = UnitData->m_HittenSound;

    m_DeathSound = UnitData->m_DeathSound;
    

    m_fAttackRange = UnitData->m_fAttackRange;

    m_fAttackSpeed = UnitData->m_fAttackSpeed;

    m_fMaxHP = hp;
    
    m_fCurrentHP = m_fMaxHP;

    m_nAvoidLevel = UnitData->m_nAvoidLevel;

    UpdateHealthBar(GetHpPercentOne());

    m_SkBody->SetScalarParameterValueOnMaterials("Visibility",1.f);

    if(m_SpawnAnim)
    {
        PlayAnimMontage(m_SpawnAnim);
    }
    m_TickFSM->Init(this);

    m_HittenAudio->SetSound(m_HittenSound);
    m_DeathAudio->SetSound(m_DeathSound);
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
    //UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_DeathSound, GetActorLocation(), 1, 1);
    
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
    FVector NewHide;
    NewHide.X=0.f;
    NewHide.Y=0.f;
    NewHide.Z = 90000.f;
    
    m_fCurrentHP=-1;
    
    SetActorLocation(NewHide);
    
    StopAnimMontage(m_DeathMontage);
    SetAcive(false);
    m_bDeathAnimEnd = true;
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

void AMonsterPawn::TakeDmg(BigInt amount, AUnitPawn* attacker)
{
    if(!GetFocusedTarget())
    {
        FocusTarget(attacker);
    }

    float AccuPercent;
    
    if(!CalculateAccuracy(attacker->GetAccuLevel(),AccuPercent))
    {
        m_PlCon->ShowDamageNumber(100.f-AccuPercent,this,EDamagePopup::Miss);
        return;
    }
    
    PlayTookHitMontage();
    PlayHitFlash();
    PlayHittenSound();

    if(!IsStatusBarActive())
    {
        ShowStatusBar();
    }
        
    m_fCurrentHP.Subtract(amount);

    m_PlCon->ShowDamageNumber(amount,this,EDamagePopup::NormalRight);

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
    return !IsHidden() && m_fCurrentHP > 0 ; 
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

bool AMonsterPawn::CalculateAccuracy(int attackerAccu, float& missPercent)
{
    float TargetAvoid = m_nAvoidLevel;
    float InstigatorAccuracy = attackerAccu;
    
    float BlockRate = FMath::RandRange(1.f, TargetAvoid); //21을 높이면 회피확률이 는다.
    float HitRate = (10.f + FMath::RandRange(0.f, InstigatorAccuracy)) -
        FMath::RandRange(1.f, TargetAvoid + 1.f); //여기서 방관 적용 가능

    bool HitSuccess = false;
    if (BlockRate < HitRate)
    {
        //아무리 높아도 5%확률로 빗나감
        HitSuccess =  1 != FMath::RandRange(1, 20);
    }
    else
    {
        HitSuccess = 1 == FMath::RandRange(1, 20);
    }

    missPercent = CalcuSameLevelAvgAccuracy(attackerAccu);
    //아무리 낮아도 5%확률로 맞음
    return HitSuccess;
}

float AMonsterPawn::CalcuSameLevelAvgAccuracy(int attackerAccu)
{
    float TargetAvoid = m_nAvoidLevel;
    float A0 = (10 ) - TargetAvoid; //10
    float A1 = (9 ) + attackerAccu; //9
    float B0 = 1.f; //1
    float B1 = 20.f + TargetAvoid; //20

    float OuterRight = FMath::Max(B1, A1);
    float OuterLeft = FMath::Min(B0, A0);
    float InnerRight = FMath::Min(B1, A1);
    float InnerLeft = FMath::Max(B0, A0);
    float Percentage = FMath::Max(0.f, FMath::Min(
                                      1.f, (InnerLeft - B0 + (InnerRight - InnerLeft) * 0.5f + OuterRight - B1) / (
                                          OuterRight - OuterLeft)));

    float Result = UDiaBlueprintFunctionLibrary::SetFloatPrecision((Percentage * 100), 1);
    return FMath::Clamp(Result, 5.f, 95.f);
}

