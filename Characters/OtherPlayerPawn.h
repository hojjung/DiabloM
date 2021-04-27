// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "PlayFabJsonObject.h"
#include "Characters/UnitPawn.h"
#include "Datas/PlayerUpgradeData.h"
#include "Managers/EquipManager.h"

#include "OtherPlayerPawn.generated.h"

class AEquipmentActor;
/**
 * 비쥬얼로도 쓰여야하고 실제 AI로도 쓰여야한다.
 * PVP매니저가 점수기록
 */
UCLASS()
class DIABLOM_API AOtherPlayerPawn : public AUnitPawn
{
	GENERATED_BODY()
public:
	AOtherPlayerPawn(const FObjectInitializer& objInit);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneCaptureComponent2D* m_Capture;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USpringArmComponent* m_Spring;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
	UChildActorComponent* m_PetComp;
	
protected:
	TSharedPtr<FStreamableHandle>  m_SkinMeshHandle;
	
	TArray<FUpgradeSpec> m_AryBaseAtkUpgrade;
	
	TArray<const FSkillUpgradeDataRow*> m_AryUpgradeSkill;

	FWeaponSpec m_WeaponSpec;

	FPetSpec m_PetSpec;
	
	TArray<FSkillSpec> m_AryEquippedSkillSpec;
	
	UPROPERTY()
	UAnimSequence* m_AnimSeq;
	UPROPERTY()
	UTextureRenderTarget2D* m_CaptureTexture;
	UPROPERTY()
	AEquipmentActor* m_WeaponActor;

protected:
	virtual void BeginPlay() override;

public:
	void SetPVPPlayerPawn(UPlayFabJsonObject* statObj, UPlayFabJsonObject* equipObj);

	void ShowMesh();

	void HideMesh();

	void ShowMeshWithTick();

	void HideMeshWithTick();
};
