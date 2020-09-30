// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Attribute/BaseDiabloAttribute.h"
//
#include "Characters/UnitMovement.h"
//
#include "Navigation/PathFollowingComponent.h"
//
#include "AbilitySystem/Ability/DiabloAbility.h"
#include "Characters/DiabloPlayerController.h"
#include "Datas/CharacterDataTable.h"


#include "UnitPawn.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, class AUnitPawn*, Character);

class UNavigationSystemV1;
UCLASS()
class DIABLOM_API AUnitPawn : public APawn, public IAbilitySystemInterface
{
    // Friended to allow access to handle functions above
    friend UBaseDiabloAttribute;

    GENERATED_BODY()

public:
    AUnitPawn(const FObjectInitializer& objInit);
public:
    UPROPERTY(EditAnywhere, Category = "Widget")
    FText m_TextUnitName;
    UPROPERTY(BlueprintAssignable, Category = "GASDocumentation|GDCharacter")
    FCharacterDiedDelegate OnCharacterDied;
    FDelegateHandle InventoryOldUpdateHandle;
    FDelegateHandle InventoryOldLoadedHandle;
    

protected:
    FGameplayTag DeadTag;
    FGameplayTag EffectRemoveOnDeathTag;
    UPROPERTY()
    UAnimMontage* m_DeathMontage;
    TArray<TSubclassOf<UDiabloAbility>> m_GrantedSkillAbilities;
    TArray<TSubclassOf<UDiabloAbility>> m_GrantedMasteryAbilities;
    TArray<TSubclassOf<UDiabloAbility>> m_GrantedItemAbilities;
    UPROPERTY()
    TSubclassOf<UGameplayEffect> m_GEUnitStat;
    UPROPERTY(EditAnywhere, Category = Unit)
    FName m_NameUnitID;
    UPROPERTY(EditAnywhere, Category = Abilities)
    int m_nCharacterLevel;
    UPROPERTY()
    ADiabloPlayerController* m_PlayerCon;
    UPROPERTY()
    FVector m_Input;
    UPROPERTY(EditAnywhere, Category = Abilities)
    float m_fMoveAcceptRadius;
    UPROPERTY()
    UNavigationSystemV1* m_NavSys;

    float m_fAttackCoolDown;

    UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Character")
    UCapsuleComponent* m_Capsule;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Character")
    USkeletalMeshComponent* m_SkBody;
    UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Movement")
    UUnitMovement* m_Movement;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "AI")
    UPathFollowingComponent* m_PFComp;
    //
    UPROPERTY(VisibleAnywhere, Category = Abilities)
    UDiabloAbilitySystemComp* m_AbilitySystemComponent;
    UPROPERTY(VisibleAnywhere, Category = Abilities)
    UBaseDiabloAttribute* m_AttributeSet;

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaSeconds) override;

    void CreateSkMeshComponent(USceneComponent* rootWant, USkeletalMeshComponent** refSkComp, FName keyName);

public:
    UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDCharacter")
    virtual void FinishDying();
    UFUNCTION(BlueprintCallable)
    void MoveToLocation(FVector goalLocation);
    UFUNCTION(BlueprintCallable)
    void MoveToActor(AActor* goalTarget);
    UFUNCTION(BlueprintCallable,Category="Interact")
    virtual void AttackInput(float pressed);

    virtual void SetUnitStat(FName unitID, int level);
    
    void SetUnitStatEffect();

    virtual bool SetCharacterLevel(int NewLevel);
    //
    void RemoveAllGameplayAbilities();

    void PrintStats();

    void MoveForward(float AxisValue);

    void MoveRight(float AxisValue);

    virtual void SetAttackSpeed(float get_attack_speed);

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    virtual UPawnMovementComponent* GetMovementComponent() const override;

    void GetCapsuleSize(float& height, float& radius);

    UDiabloAbilitySystemComp* GetDiaAbilitySystem() const;

    virtual float GetHealth() const;

    float GetHpPercentOne() const;

    virtual float GetMaxHealth() const;

    virtual float GetMoveSpeed() const;

    bool IsAlive();

    FORCEINLINE UBaseDiabloAttribute* GetAttributeSet()
    {
        return m_AttributeSet;
    }

    FORCEINLINE UCapsuleComponent* GetCapsule()
    {
        return m_Capsule;
    }

    FORCEINLINE int GetLevel()
    {
        return m_nCharacterLevel;
    }

    float PlayAnimMontage(UAnimMontage* anim_montage,float InPlayRate = 1.f, FName StartSectionName = NAME_None);
    
    void StopAnimMontage(UAnimMontage* AnimMontage);

    UAnimMontage * GetCurrentMontage();

    
    virtual void Die();

    
    

};
