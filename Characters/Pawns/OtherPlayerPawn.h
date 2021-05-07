// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "PlayerDiabloCharacter.h"

#include "PlayFabJsonObject.h"
#include "UnitPawn.h"
#include "Datas/PlayerUpgradeData.h"
#include "Interface/SkillUseCharacter.h"
#include "Managers/EquipManager.h"
#include "Widgets/CommonElement/FloatingTextWidgetComponent.h"

#include "OtherPlayerPawn.generated.h"

class UAutoSkillUse;
class AEquipmentActor;
/**
 * 비쥬얼로도 쓰여야하고 실제 AI로도 쓰여야한다.
 * PVP매니저가 점수기록
 */
UCLASS()
class DIABLOM_API AOtherPlayerPawn : public AUnitPawn,public ISkillUseCharacter
{
	GENERATED_BODY()
public:
	AOtherPlayerPawn(const FObjectInitializer& objInit);
	
	FOnFloatChange2 m_OnRageChanged;

	bool m_bUseFSM;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneCaptureComponent2D* m_Capture;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USpringArmComponent* m_Spring;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
	UChildActorComponent* m_PetComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UFloatingTextWidgetComponent* m_NameCard;
	UPROPERTY()
	UAutoSkillUse* m_AutoSkillUse; 
	
protected:
	float m_fSkillCastTime;
	
	FSkillSpec* m_CurrentCastingSkill;
	
	TSharedPtr<FStreamableHandle>  m_SkinMeshHandle;
	
	TArray<FUpgradeSpec> m_AryUpgradeSpec;
	
	TArray<FSkillSpec> m_ArySkillSpec;

	TArray<FSkillSpec*> m_ArySkillEquipped;
	
	FWeaponSpec m_WeaponSpec;

	FPetSpec m_PetSpec;
	
	
	UPROPERTY()
	UAnimSequence* m_AnimSeq;
	UPROPERTY()
	UTextureRenderTarget2D* m_CaptureTexture;
	UPROPERTY()
	AEquipmentActor* m_WeaponActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Player")
	TArray<TEnumAsByte< EObjectTypeQuery>> m_AryTargetingObjectType;

	FTimerHandle m_AttackTimer;
	
	TQueue<EDamageType> m_QueDmgType;

	UPROPERTY()
	float m_fMaxRage;
	UPROPERTY()
	float m_fGainRagePer;
	UPROPERTY()
	float m_fCurrentRage;
	UPROPERTY()
	float m_fBuff01MaxTime;
	UPROPERTY()
	float m_fBuff02MaxTime;
	UPROPERTY()
	float m_fBuff01DeltaCount;
	UPROPERTY()
	float m_fBuff02DeltaCount;
	UPROPERTY()
	float m_fAdditionalAttackSpeed;
	
	BigInt m_bnAdditionalSkillDmg;


protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void TriggerSkill(const FName& name,TArray<FHitResult>* aryHits=nullptr) override;	

	void StartBuff01(float sec);

	bool IsBuff01Available();

	void EndBuff01();

	void StartBuff02(float sec);

	bool IsBuff02Available();

	void EndBuff02();

	bool GetDmg(BigInt& outDmg, EDamagePopup& pp);

public:
	virtual void FocusTarget(AUnitPawn* target) override;
	
	void SetPVPPlayerPawn(UPlayFabJsonObject* statObj,UPlayFabJsonObject* skillObj, UPlayFabJsonObject* equipObj);

	void ShowMesh();

	void HideMesh();

	void ShowMeshWithTick();

	void HideMeshWithTick();
	//
	float PlayAttackMontage(float& currentCd,float maxCd,FName* sectionSkillName=nullptr);
	
public:
	void SetPetPositionForVisual();

	void SetPetPositionForBattle();

	virtual void TakeDmg(BigInt amount,AUnitPawn* attacker,EDamagePopup pp) override;

	virtual float TryAttack() override;

	virtual float PlaySkillMontageSection(FName& nameID,int nSectionIndex,float& currentCD,float maxCD) override;

	void ApplyDamageToTarget(const BigInt* additionalDmg = nullptr);
	
	void ApplyDamageToTargets(TArray<FHitResult>& aryTargets,const BigInt* additionalDmg = nullptr);

	void ApplyMoveSpeedToOrigin();

	void GainRagePoint();

	bool SpendRagePoint(float rage);


	void ApplyDamage(AUnitPawn* target,const BigInt& finalDmg,EDamagePopup& pp);


	virtual float GetRage() override
	{
		return m_fCurrentRage;
	}

	void ShowNameCard(const FString& name);

	FORCEINLINE TArray<FSkillSpec*>& GetEquippedSkill()
	{
		return m_ArySkillEquipped;
	}

	void UpdateRage();
};

