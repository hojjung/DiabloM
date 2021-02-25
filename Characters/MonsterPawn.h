#pragma once
#include "DiabloM.h"
#include "DiabloPlayerController.h"
#include "Characters/UnitPawn.h"
#include "Widgets/CommonElement/FloatingStatusBarWidgetCompo.h"
#include "MonsterPawn.generated.h"

class UDungeonManager;
/**
 * 
 */
class UMobFSMBase;
class UMonsterSensing;
UCLASS()
class DIABLOM_API AMonsterPawn : public AUnitPawn
{
	GENERATED_BODY()
public:
	AMonsterPawn(const FObjectInitializer& objInit);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* m_StShadow;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UFloatingStatusBarWidgetCompo* m_WorldHpBar;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* m_HittenAudio;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* m_DeathAudio;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* m_CoinAudio;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* m_Particle;
protected:
	FTimerHandle m_DeathTimer;
	UPROPERTY(VisibleAnywhere, Category = "Character")
	UAnimMontage* m_DeathMontage;
	UPROPERTY(VisibleAnywhere, Category = "Character")
	UAnimMontage* m_TookHitMontage;
	UPROPERTY()
	UAnimMontage* m_SpawnAnim;
	//UPROPERTY()
	//UMonsterSensing* m_MonsterSense;
	UPROPERTY()
	ADiabloPlayerController* m_PlCon;

	bool m_bDeathAnimEnd;

	int m_nAvoidLevel;

public: //need more monster
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;
	
    bool IsStatusBarActive();
	
	void UpdateHealthBar(float perOne);
	void HideStatusBar();

	virtual void Die() override;
	
	virtual void OnDeathAnimEnd() override;
	void PlayHittenSound();
	void ShowStatusBar();

	virtual void TakeDmg(BigInt amount,AUnitPawn* attacker) override;

	virtual FVector GetLastSeenLocation() override;

	//void SetHealthPercentage(const FOnAttributeChangeData& data);
	void PlayTookHitMontage();

	virtual void FocusTarget(AUnitPawn* target) override;

public:
    void PlayHitFlash();

	void DataInject(const FMonsterEntity* monster_table, const BigInt& hp);

	virtual bool IsAlive() const override;

	bool IsReadyToPool();

	void SetAcive(bool v);

	bool CalculateAccuracy(int attackerAccu,float& missPercent);

	float CalcuSameLevelAvgAccuracy(int attackerAccu);

};
