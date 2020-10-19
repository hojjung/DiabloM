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
#include "Logic/MobFSM_Swamer.h"


#include "UnitPawn.generated.h"

class UDiabloGameInstance;
DECLARE_MULTICAST_DELEGATE(FOnAttack);
DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, class AUnitPawn*);

class UNavigationSystemV1;
UCLASS()
class DIABLOM_API AUnitPawn : public APawn, public IAbilitySystemInterface
{
    friend UBaseDiabloAttribute;

    GENERATED_BODY()

public:
    AUnitPawn(const FObjectInitializer& objInit);

protected:
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
    UAnimMontage* m_DeathMontage;
    UPROPERTY(VisibleAnywhere, Category = "Character")
    TSubclassOf<UGameplayEffect> m_GEUnitStat;
    UPROPERTY(VisibleAnywhere, Category = "Character")
    UNavigationSystemV1* m_NavSys;
    UPROPERTY(VisibleAnywhere, Category = "Character")
    FText m_TextUnitName;
    UPROPERTY(VisibleAnywhere, Category = "Character")
    FGameplayTag m_DeadTag;
    UPROPERTY(VisibleAnywhere, Category = "Character")
    FGameplayTag m_EffectRemoveOnDeathTag;
    
    TWeakObjectPtr< AUnitPawn> m_FocusedEnemy;
    
    FCharacterDiedDelegate m_OnCharacterDied;

    FOnAttack m_OnStartAttack;

    //FOnAttack m_OnPressedAttack;

    FOnAttack m_OnEndAttack;
    
    float m_fTickDeltaTime;

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
    
    FPathFollowingRequestResult MoveToLocation(FVector goalLocation);
    
    FPathFollowingRequestResult MoveToActor(AActor* goalTarget);

    UFUNCTION(BlueprintCallable,Category="Interact")
    virtual void StartAttack();
    UFUNCTION(BlueprintCallable,Category="Interact")
    virtual void EndAttack();

    virtual bool SetCharacterLevel(int NewLevel);

    virtual UPawnMovementComponent* GetMovementComponent() const override;

    void GetCapsuleSize(float& height, float& radius);

    virtual void PrintStats();

    float PlayAnimMontage(UAnimMontage* anim_montage, float InPlayRate = 1.f, FName StartSectionName = NAME_None);

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

public: //AttributeGetter

    virtual void HomingRotateToTarget();
    
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    
    UDiabloAbilitySystemComp* GetDiaAbilitySystem() const;

    float GetAttackSpeed() const;
    
    virtual float GetHealth() const;

    float GetHpPercentOne() const;

    virtual float GetMaxHealth() const;

    virtual float GetMoveSpeed() const;

    virtual bool HasDropItem();

    virtual void FocusTarget(AUnitPawn* target);

    virtual bool IsAlive();

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

    virtual FVector GetVelocity() const override;
};
