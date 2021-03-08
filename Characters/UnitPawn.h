// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"

#include "GameFramework/Pawn.h"
#include "Characters/UnitMovement.h"
#include "Navigation/PathFollowingComponent.h"
#include "Datas/CharacterDataTable.h"
#include "Logic/FSMTick.h"
#include "Perception/AIPerceptionComponent.h"
#include "Widgets/CommonElement/DamageTextWidgetComponent.h"


#include "UnitPawn.generated.h"

class UDamageTextWidgetComponent;
class UDiabloGameInstance;
DECLARE_MULTICAST_DELEGATE(FOnAttack);
DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, class AUnitPawn*);

class UNavigationSystemV1;

UENUM(BlueprintType)
enum class ETeamID :uint8
{
    Player,
    Monster
};
UCLASS()
class DIABLOM_API AUnitPawn : public APawn
{
    GENERATED_BODY()

public:
    AUnitPawn(const FObjectInitializer& objInit);
protected:
    UPROPERTY(EditAnywhere)
    ETeamID m_TeamID;
    UPROPERTY(EditAnywhere)
    bool m_bUseFSM;
    UPROPERTY(EditAnywhere, Category = Abilities)
    float m_fMoveAcceptRadius;
    UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Character")
    UCapsuleComponent* m_Capsule;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Character")
    USkeletalMeshComponent* m_SkBody;
    UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Movement")
    UUnitMovement* m_Movement;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "AI")
    UPathFollowingComponent* m_PFComp;
    UPROPERTY()
    UFSMTick* m_TickFSM;

    float m_fAttackRange;

    float m_fAttackSpeed;

    int m_nAccuracyLevel;
public:
    UPROPERTY()
    UAnimMontage* m_BaseAttackAnim;
    
    UPROPERTY(VisibleAnywhere, Category = "Character")
    UNavigationSystemV1* m_NavSys;
    UPROPERTY(VisibleAnywhere, Category = "Character")
    FText m_TextUnitName;
    
    
    TWeakObjectPtr<AUnitPawn> m_FocusedEnemy;
    
    float m_fTickDeltaTime;

    float m_fHitAnimCD;

    float m_fAttackCDConstant;

    float m_fAttackCD;

    BigInt m_fCurrentHP;

    BigInt m_fMaxHP;

    

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaSeconds) override;

    void CreateSkMeshComponent(USceneComponent* rootWant, USkeletalMeshComponent** refSkComp, FName keyName);

    FPathFollowingRequestResult MoveTo(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr* OutPath = nullptr);

    void FindPathForMoveRequest(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query, FNavPathSharedPtr& OutPath) const;

    bool BuildPathfindingQuery(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query) const;

    FAIRequestID RequestMove(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr Path);

public://virtual
    virtual void OnDeathAnimEnd();

    virtual void Die();

    bool virtual CanSeeTarget();

    virtual float TryAttack();

    virtual void TakeDmg(BigInt amount,AUnitPawn* attacker,EDamagePopup pp);

    virtual FVector GetLastSeenLocation();

    virtual float GetAttackSpeed();

    virtual float GetAttackRange();

public:

    
    void GetHP(BigInt& cH,BigInt& mH);
    
    void UpdateMoveSpeed() const;
    
    float GetAcceptRadiusToOther();

    float GetAcceptRadiusSelfOnly();
    
    virtual FPathFollowingRequestResult MoveToLocation(FVector goalLocation,float additionalAcceptRadius=0.f);
    
    FPathFollowingRequestResult MoveToActor(AActor* goalTarget,float additionalAcceptRadius=0.f);

    virtual UPawnMovementComponent* GetMovementComponent() const override;

    void GetCapsuleSize(float& height, float& radius);

    float PlayAnimMontage(UAnimMontage* anim_montage, float InPlayRate = 1.f, FName StartSectionName = NAME_None);

    float PlayAnim(UAnimSequenceBase* animAsset,bool isLoop=false);

    void StopAnimMontage(UAnimMontage* AnimMontage);

    UAnimMontage* GetCurrentMontage();

    FORCEINLINE UCapsuleComponent* GetCapsule()
    {
        return m_Capsule;
    }

    FRotator GetHomingRotToTarget();

    void StopMove();
    
    virtual void HomingRotateToTarget();
    
    bool IsEulerAngleAcceptForTarget(float eulerAngle);

    bool IsDotAngleAcceptForTarget(float dotAngle);
    
    float GetHpPercentOne() const;

    virtual void FocusTarget(AUnitPawn* target);

    virtual bool IsAlive() const;

    AUnitPawn* GetFocusedTarget()
    {
        return m_FocusedEnemy.Get();
    }

    TWeakObjectPtr<AUnitPawn>& GetFocusedTargetWeakPtr()
    {
        return m_FocusedEnemy;
    }

    virtual FVector GetVelocity() const override;
    
    FVector* GetVelocityPtr() const;
    
    void SetBlockMove();
    
    void SetUnblockMove();

    

    FORCEINLINE UPathFollowingComponent* GetPfComp()
    {
        return m_PFComp;
    }

    FORCEINLINE ETeamID GetTeamID()
    {
        return m_TeamID;
    }

    FORCEINLINE USkeletalMeshComponent* GetSkMeshComp()
    {
        return m_SkBody;
    }

    float GetMoveSpeed();
    
    bool IsMoving();
    
    virtual int GetAccuLevel();

    UUnitMovement* GetMoveComp()
    {
        return m_Movement;
    }
};


