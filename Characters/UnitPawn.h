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
//
#include "Navigation/PathFollowingComponent.h"
//
#include "Characters/DiabloPlayerController.h"
#include "Datas/CharacterDataTable.h"



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
public:
	UPROPERTY(EditAnywhere, Category = "Widget")
	FText m_TextUnitName;
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

	float m_fAttackCoolDown;
protected:
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

public://should go dataTable
	//TArray<TSubclassOf<UDiabloAbility>> m_StartGameplayAbilities;

	TSubclassOf<UGameplayEffect> m_GEUnitStat;
protected:
	virtual void BeginPlay() override;

	void CreateSkMeshComponent(USceneComponent* rootWant,USkeletalMeshComponent** refSkComp,FName keyName);
	
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
	UDiabloAbilitySystemComp * GetDiaAbilitySystem() const;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	FORCEINLINE int GetLevel()
	{
		return m_nCharacterLevel;
	}

	//
	void PrintStats();
	
	FActiveGameplayEffectHandle ApplyGameEffect(TSubclassOf<UGameplayEffect> gameEffect);

	virtual void SetUnitStat(FName unitID,int level);

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
	
	void RemoveSlottedGameplayAbilities(bool bRemoveAll);
	virtual void SetAttackSpeed(float get_attack_speed);
	bool IsAlive();

	FDelegateHandle InventoryOldUpdateHandle;
	FDelegateHandle InventoryOldLoadedHandle;
public:
	//데이터 테이블로 빼줘야함.

	virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, AUnitPawn* InstigatorCharacter, AActor* DamageCauser);
	virtual void HandleHealthChanged(float currentHealth,float maxHealth ,const struct FGameplayTagContainer& EventTags);
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

	FORCEINLINE UBaseDiabloAttribute* GetAttributeSet()
	{
		return m_AttributeSet;
	}
	
};
