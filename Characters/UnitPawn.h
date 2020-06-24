// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Attribute/BaseDiabloAttribute.h"
#include "AbilitySystem/DiabloAbilitySystemComp.h"
#include "AbilitySystem/DiabloAbility.h"
//
#include "Characters/UnitMovement.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
//
#include "Navigation/PathFollowingComponent.h"
#include "Perception/AIPerceptionListenerInterface.h"
#include "GenericTeamAgentInterface.h"
#include "AI/Navigation/NavAgentInterface.h"
//
#include "Characters/DiabloPlayerController.h"


#include "UnitPawn.generated.h"


class UNavigationSystemV1;
UCLASS()
class DIABLOM_API AUnitPawn : public APawn, public IAbilitySystemInterface
{
	// Friended to allow access to handle functions above
	friend UBaseDiabloAttribute;

	GENERATED_BODY()

public:
	AUnitPawn(const FObjectInitializer& objInit);

protected:
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
protected:
	UPROPERTY(VisibleAnywhere, Category = "Character")
	UCapsuleComponent* m_Capsule;
	UPROPERTY(VisibleAnywhere, Category = "Character")
	USkeletalMeshComponent* m_SkMesh;
	UPROPERTY(VisibleAnywhere,Category="Movement")
	UUnitMovement* m_Movement;
	UPROPERTY(VisibleAnywhere, Category = "AI")
	UPathFollowingComponent* m_PFComp;
	//
	UPROPERTY(VisibleAnywhere, Category = Abilities)
	UDiabloAbilitySystemComp* m_AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, Category = Abilities)
	UBaseDiabloAttribute* m_AttributeSet;

public://should go dataTable
	TArray<TSubclassOf<UDiabloAbility>> m_StartGameplayAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UGameplayEffect>> m_PassiveGameplayEffects;
protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void MoveToLocation(FVector goalLocation);
	UFUNCTION(BlueprintCallable)
	void MoveToActor(AActor* goalTarget);

	virtual void Tick(float DeltaSeconds) override;



	virtual UPawnMovementComponent* GetMovementComponent() const override;

	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

	void GetCapsuleSize(float& height,float & radius);

	FORCEINLINE UCapsuleComponent* GetCapsule()
	{
		return m_Capsule;
	}

	// IAbilitySystemInterface을(를) 통해 상속됨
	virtual UAbilitySystemComponent * GetAbilitySystemComponent() const override;

	FORCEINLINE int GetLevel()
	{
		return m_nCharacterLevel;
	}

	//
	void PrintStats();

	virtual void SetUnit(FName unitID);

	virtual float GetHealth() const;

	float GetHpPercentOne()const;

	virtual float GetMaxHealth() const;

	virtual float GetMoveSpeed() const;

	virtual bool SetCharacterLevel(int NewLevel);
	//
	void AddStartupGameplayAbilities();

	void RemoveStartupGameplayAbilities();

	bool ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation = true);

	void GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UDiabloAbility*>& ActiveAbilities);

	bool GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration);
	//
	//UFUNCTION(BlueprintCallable, Category = "Abilities")
	//bool ActivateAbilitiesWithItemSlot(FRPGItemSlot ItemSlot, bool bAllowRemoteActivation = true);

	/** Returns a list of active abilities bound to the item slot. This only returns if the ability is currently running */
//	UFUNCTION(BlueprintCallable, Category = "Abilities")
	//void GetActiveAbilitiesWithItemSlot(FRPGItemSlot ItemSlot, TArray<URPGGameplayAbility*>& ActiveAbilities);

	//void OnItemSlotChanged(FRPGItemSlot ItemSlot, URPGItem* Item);

	//void RefreshSlottedGameplayAbilities();

	//void AddSlottedGameplayAbilities();

	//void FillSlottedAbilitySpecs(TMap<FRPGItemSlot, FGameplayAbilitySpec>& SlottedAbilitySpecs);

	void RemoveSlottedGameplayAbilities(bool bRemoveAll);

	FDelegateHandle InventoryUpdateHandle;
	FDelegateHandle InventoryLoadedHandle;
public:
	//데이터 테이블로 빼줘야함.

	virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, AUnitPawn* InstigatorCharacter, AActor* DamageCauser);
	virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleManaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, AUnitPawn* InstigatorCharacter, AActor* DamageCauser);
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	UFUNCTION(BlueprintImplementableEvent)
	void OnManaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	UFUNCTION(BlueprintImplementableEvent)
	void OnMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);



};
