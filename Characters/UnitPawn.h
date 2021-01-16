// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Attribute/BaseDiabloAttribute.h"
#include "Characters/UnitMovement.h"
#include "Navigation/PathFollowingComponent.h"
#include "AbilitySystem/Ability/DiabloAbility.h"
#include "Datas/CharacterDataTable.h"
#include "Perception/AIPerceptionComponent.h"

#include "UnitPawn.generated.h"

class UMobFSM_Swamer;
class UMobFSM_Shooter;
class UPlayerAutoPlayFSM;
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
class DIABLOM_API AUnitPawn : public APawn, public IAbilitySystemInterface
{
    friend UBaseDiabloAttribute;

    GENERATED_BODY()

public:
    AUnitPawn(const FObjectInitializer& objInit);
protected:
    UPROPERTY(EditAnywhere)
    ETeamID m_TeamID;
    UPROPERTY(EditAnywhere)
    bool m_bUseFSM;
    UPROPERTY(EditAnywhere, Category = Abilities)
    int m_nCharacterLevel;
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
    UPROPERTY(VisibleAnywhere, Category = Abilities)
    UDiabloAbilitySystemComp* m_AbilitySystemComponent;
    UPROPERTY(VisibleAnywhere, Category = Abilities)
    UBaseDiabloAttribute* m_AttributeSet;

protected:
    UPROPERTY(VisibleAnywhere, Category = "Character")
    UAnimSequenceBase* m_DeathMontage;
    UPROPERTY(VisibleAnywhere, Category = "Character")
    UAnimMontage* m_StunMontage;
    UPROPERTY(VisibleAnywhere, Category = "Character")
    UAnimSequenceBase* m_TookHitMontage;
    UPROPERTY(VisibleAnywhere, Category = "Character")
    TSubclassOf<UGameplayEffect> m_GEUnitStat;
    UPROPERTY(VisibleAnywhere, Category = "Character")
    UNavigationSystemV1* m_NavSys;
    UPROPERTY(VisibleAnywhere, Category = "Character")
    FText m_TextUnitName;
    UPROPERTY(VisibleAnywhere, Category = "Character")
    FGameplayTag m_TagDead;
    UPROPERTY(VisibleAnywhere, Category = "Character")
    FGameplayTag m_TagStun;
    UPROPERTY(VisibleAnywhere, Category = "Character")
    FGameplayTag m_TagEffectRemoveOnDeath;
    
    TWeakObjectPtr<AUnitPawn> m_FocusedEnemy;

    
    
    FCharacterDiedDelegate m_OnCharacterDied;

    FOnAttack m_OnStartAttack;

    //FOnAttack m_OnPressedAttack;

    FOnAttack m_OnEndAttack;
    
    float m_fTickDeltaTime;

    float m_fHitAnimCD;

    FGameplayAbilitySpecHandle m_BaseAttackHandle;

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaSeconds) override;

    void CreateSkMeshComponent(USceneComponent* rootWant, USkeletalMeshComponent** refSkComp, FName keyName);

    void SetUnitStatEffect();

    FPathFollowingRequestResult MoveTo(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr* OutPath = nullptr);

    void FindPathForMoveRequest(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query, FNavPathSharedPtr& OutPath) const;

    bool BuildPathfindingQuery(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query) const;

    FAIRequestID RequestMove(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr Path);

    virtual void OnDeathAnimEnd();
public:
    float GetAcceptRadiusToOther();

    float GetAcceptRadiusSelfOnly();
    
    bool virtual CanSeeTarget();
    
    UFUNCTION(BlueprintCallable)
    virtual void Die();
    
    FPathFollowingRequestResult MoveToLocation(FVector goalLocation,float additionalAcceptRadius=0.f);
    
    FPathFollowingRequestResult MoveToActor(AActor* goalTarget,float additionalAcceptRadius=0.f);

    UFUNCTION(BlueprintCallable,Category="Interact")
    virtual void StartAttack();
    UFUNCTION(BlueprintCallable,Category="Interact")
    virtual void EndAttack();

    virtual bool SetCharacterLevel(int NewLevel);

    virtual UPawnMovementComponent* GetMovementComponent() const override;

    void GetCapsuleSize(float& height, float& radius);

    virtual void PrintStats();

    float PlayAnimMontage(UAnimMontage* anim_montage, float InPlayRate = 1.f, FName StartSectionName = NAME_None);

    float PlayAnim(UAnimSequenceBase* animAsset,bool isLoop=false);

    void StopAnimMontage(UAnimMontage* AnimMontage);

    UAnimMontage* GetCurrentMontage();

    USkeletalMeshComponent* GetBodyMesh()
    {
        return m_SkBody;
    }

    FORCEINLINE UCapsuleComponent* GetCapsule()
    {
        return m_Capsule;
    }

    void DoBaseAttack();
    
    FRotator GetHomingRotToTarget();

public: //AttributeGetter
    void StopMove();
    
    virtual void HomingRotateToTarget();

    bool IsEulerAngleAcceptForTarget(float eulerAngle);

    bool IsDotAngleAcceptForTarget(float dotAngle);
    
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    
    UDiabloAbilitySystemComp* GetDiaAbilitySystem() const;

    float GetAttackSpeed() const;

    float GetAttackRange() const;

    
    
    virtual float GetHealth() const;

    float GetHpPercentOne() const;

    virtual float GetMaxHealth() const;

    virtual float GetMoveSpeed() const;

    virtual void FocusTarget(AUnitPawn* target);

    virtual bool IsAlive() const;

    UBaseDiabloAttribute* GetAttributeSet() const;

    FORCEINLINE int GetCharacterLevel() const
    {
        return m_nCharacterLevel;
    }

    FOnAttack& GetOnStartAttack()
    {
        return m_OnStartAttack;
    }

    FOnAttack& GetOnEndAttack()
    {
        return m_OnEndAttack;
    }

    FText& GetShowNameText()
    {
        return m_TextUnitName;
    }

    AUnitPawn* GetFocusedTarget()
    {
        return m_FocusedEnemy.Get();
    }

    TWeakObjectPtr<AUnitPawn>& GetFocusedTargetWeakPtr()
    {
        return m_FocusedEnemy;
    }

    FCharacterDiedDelegate& GetOnDied()
    {
        return  m_OnCharacterDied;
    }

    virtual FVector GetLastSeenLocation()
    {
        return FVector::ZeroVector;
    }


    friend UMobFSM_Swamer;
    friend UDiabloGameInstance;
    friend UPlayerAutoPlayFSM;
    friend UMobFSM_Shooter;

    virtual FVector GetVelocity() const override;

    FVector* GetVelocityPtr() const;
    void SetBlockMove();
    void SetUnblockMove();

    virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

    void PlayTookHitMontage();

    FORCEINLINE UPathFollowingComponent* GetPfComp()
    {
        return m_PFComp;
    }

    FORCEINLINE ETeamID GetTeamID()
    {
        return m_TeamID;
    }
};


