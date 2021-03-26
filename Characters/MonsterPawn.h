#pragma once
#include "DiabloM.h"
#include "DiabloPlayerController.h"
#include "Characters/UnitPawn.h"
#include "Datas/DungeonDataTable.h"
#include "Widgets/CommonElement/FloatingStatusBarWidgetCompo.h"
#include "MonsterPawn.generated.h"

class UDungeonManager;
/**
 * 
 */
class UMobFSMBase;
class UMonsterSensing;

UENUM()
enum EMonsterType
{
	Normal,
	Treasure,
	Boss
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMonsterDead,AMonsterPawn*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMonsterHit,float);
UCLASS()
class DIABLOM_API AMonsterPawn : public AUnitPawn
{
	GENERATED_BODY()
public:
	AMonsterPawn(const FObjectInitializer& objInit);

	EMonsterType m_MonsterType;
	
	FOnMonsterDead m_OnDead;

	FOnMonsterHit m_OnTookDmg;
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* m_HitParticle;
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

	BigInt m_fGoldBounty;

	const FItemDropTableRow* m_DropTable;

	bool m_bDeathAnimEnd;

protected:
	void RequestDropItem();

	void RequestGetGoldBounty();

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

	virtual void TakeDmg(BigInt amount,AUnitPawn* attacker,EDamagePopup pp) override;

	virtual FVector GetLastSeenLocation() override;

	//void SetHealthPercentage(const FOnAttributeChangeData& data);
	void PlayTookHitMontage();

	virtual void FocusTarget(AUnitPawn* target) override;

public:
    void PlayHitFlash();

	void DataInject(const FMonsterEntity* monster_table, const BigInt& hp,const BigInt& gold,EMonsterType type,const FItemDropTableRow* dropTable,float statFactor =1.f,float scaleFactor=1.f);

	virtual bool IsAlive() const override;

	bool IsReadyToPool();

	void SetAcive(bool v);

	virtual float TryAttack() override;
};
