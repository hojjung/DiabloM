#pragma once
#include "DiabloM.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "EquipmentData.generated.h"


UCLASS()
class DIABLOM_API UEquipmentData : public UObject
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FPlayerSkinTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName m_NameID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMesh* m_PlayerSkin = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAnimInstance> m_AnimBP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimSequence* m_VisualIdleAnim;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* m_BaseAttackAnim;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(UIMin = "1.0", UIMax = "3.3"))
	float m_fAttackSpeedMultiple = 1.f;

	virtual BigInt GetValue(int level) const
	{
		return m_fAttackSpeedMultiple *100.f;
	}
};






