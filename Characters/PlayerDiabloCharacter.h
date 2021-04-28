// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "WeakInterfacePtr.h"
#include "Characters/UnitPawn.h"
#include "Managers/DiabloCheatManager.h"
#include "Managers/MonsterSpawnManager.h"
#include "Widgets/CommonElement/FloatingTextWidgetComponent.h"


#include "PlayerDiabloCharacter.generated.h"

class AEquipmentActor;
struct FPetSpec;
struct FAccessorySpec;
struct FWingSpec;
struct FWeaponSpec;
struct FPlayerClassSpec;
class UEquipManager;
class UPlayerUpgradeManager;
class UDiaStatPanel;
class ADiabloPlayerController;
class UCameraDissolve;
class UPlayerSensing;
class UDefaultFSM;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnFloatChange,float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnFloatChange2,float,float);
DECLARE_MULTICAST_DELEGATE(FOnMove);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnFocusTargetChanged,AUnitPawn*);
DECLARE_MULTICAST_DELEGATE(FOnPlVisualChanged);




UENUM(BlueprintType)
enum class EDamageType :uint8
{
	Base01,
	Base02,
    Critical01,
    Critical02,
	Magic01,
	Magic02,
	Length
};



UCLASS( BlueprintType, Blueprintable)
class DIABLOM_API APlayerDiabloCharacter : public AUnitPawn
{
	GENERATED_BODY()
	
	friend UDiabloCheatManager;
	friend UPlayerSensing;
	friend UDiabloGameInstance;
public:
	APlayerDiabloCharacter(const FObjectInitializer& objInit);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnMeshChanged,APlayerDiabloCharacter*);
public:
	FOnMove m_OnMove;

	FOnFocusTargetChanged m_OnFocusTarget;

	FOnFloatChange2 m_OnRageChanged;
	
	FOnMeshChanged m_OnMeshChanged;

	
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Player")
	TArray<TEnumAsByte< EObjectTypeQuery>> m_AryTargetingObjectType;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Player")
	UCameraDissolve* m_DissolveCam;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Player")
	UCameraComponent* m_TopCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Player")
	UChildActorComponent* m_PetComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UFloatingTextWidgetComponent* m_NameCard;
	//
protected:
	UPROPERTY()
	UPlayerUpgradeManager* m_PlUpgradeManager;
	UPROPERTY()
	UEquipManager* m_EquipManager;
	UPROPERTY()
	UPlayerSensing* m_PlayerSense;
	UPROPERTY()
	ADiabloPlayerController* m_PlayerCon;
	
	const FPlayerClassSpec* m_PlayerEntityData;

	FVector m_Input;

	TWeakObjectPtr<AUnitPawn> m_FocusOutlinePawn;;


	bool m_bIsManualMove;

	FTimerHandle m_AttackTimer;
	
	TQueue<EDamageType> m_QueDmgType;

	UPROPERTY()
	AEquipmentActor* m_CreatedWing;
	UPROPERTY()
	AEquipmentActor* m_CreatedWeapon;
	UPROPERTY()
	AEquipmentActor* m_CreatedPet;

	float m_fMaxRage;

	float m_fGainRagePer;

	float m_fCurrentRage;

	float m_fBuff01MaxTime;

	float m_fBuff02MaxTime;

	float m_fBuff01DeltaCount;

	float m_fBuff02DeltaCount;

	BigInt m_bnAdditionalSkillDmg;

	float m_fAdditionalAttackSpeed;

	TSharedPtr<FStreamableHandle>  m_SkinMeshHandle;
	
protected:
	virtual void BeginPlay() override;
	
	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void SetBaseAttackData(float viewAngle,float viewRadius,float focusRange);

	virtual FVector GetLastSeenLocation() override;

	virtual float TryAttack() override;//공격하려는 의지

	//몽타쥬 플레이
	//몽타쥬를 플레이하기전에 마력폭발과 치명타 여부가 결정되어있음
	//FName CalculateCritical();
	
	float PlayAttackMontage(float& currentCd,float maxCd,FName* sectionSkillName=nullptr);
	//몽타쥬 트리거
	virtual void TakeDmg(BigInt amount,AUnitPawn* attacker,EDamagePopup pp) override;

	void ApplyDamage(AUnitPawn* target,const BigInt& finalDmg,EDamagePopup& pp);

	bool GetDmg(BigInt& outDmg,EDamagePopup& pp);

	//
	void StartBuff01(float sec);

	bool IsBuff01Available();

	void EndBuff01();

	void StartBuff02(float sec);

	bool IsBuff02Available();

	void EndBuff02();

public:
	void PlayerClassDataInject(const FPlayerClassSpec& spec);
	
	void WeaponDataInject(const FWeaponSpec& spec);
	
	void WingDataInject(const FWingSpec& spec);
	
	void AccessoryDataInject(const FAccessorySpec& spec);
	
	void PetDataInject(const FPetSpec& spec);

public:
	virtual void FocusTarget(AUnitPawn* target) override;
	
	
	void ShowOutlineOnTarget(AUnitPawn* Unit);
	
	void HideOutlineOnTarget();

	virtual float GetAttackSpeed() override;

	ADiabloPlayerController* GetDiaController();

	virtual void Die()override;
	
	virtual void OnDeathAnimEnd()override;
	
	FORCEINLINE const TArray<TEnumAsByte< EObjectTypeQuery>>& GetAryTarget()
	{
		return m_AryTargetingObjectType;
	}

	void ClearFocusedTarget(AUnitPawn* target);

	virtual bool IsAlive() const override;
	
	UFUNCTION(BlueprintCallable)
	void PlayColorEffect(const FLinearColor& colorWant,float effectLength);
	
	float PlaySkillMontageSection(FName& nameID,int nSectionIndex,float& currentCD,float maxCD);

public:
	friend UDiabloGameInstance;
	
	friend UDiaStatPanel;

	void ApplyDamageToTarget(const BigInt* additionalDmg = nullptr);
	
    void ApplyDamageToTargets(TArray<FHitResult>& aryTargets,const BigInt* additionalDmg = nullptr);

	void ApplyMoveSpeedToOrigin();

	FVector GetCameraLoc() const
	{
		return m_TopCamera->GetComponentLocation();
	}

	bool GetIsManualMove() const
	{
		return m_bIsManualMove;
	}

	friend UMonsterSpawnManager;

	void SetManualMoveLocation(FVector goalLocation);

	void GainRagePoint();

	bool SpendRagePoint(float rage);


	float GetRage()
	{
		return m_fCurrentRage;
	}

	virtual void TriggerSkill(const FName& name,TArray<FHitResult>* aryHits=nullptr) override;

	FORCEINLINE UFSMTick* GetTickFSM()
	{
		return m_TickFSM;
	}

	FORCEINLINE const FPlayerClassSpec* GetPlayerEntityData()
	{
		return  m_PlayerEntityData;
	};

	FORCEINLINE  AEquipmentActor* GetCreatedWing()
	{
		return  m_CreatedWing;
	};

	FORCEINLINE  AEquipmentActor* GetCreatedWeapon()
	{
		return  m_CreatedWeapon;
	};

	FORCEINLINE  UChildActorComponent* GetPetComponent()
	{
		return  m_PetComp;
	};
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void ShowNameCard(const FString& name);
};

