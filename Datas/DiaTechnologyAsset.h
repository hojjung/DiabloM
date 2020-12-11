#pragma once
#include "DiabloM.h"
#include "Types/TechnologyAsset.h"
#include "SkillDataTable.h"
#include "DiaTechnologyAsset.generated.h"


class UDiabloAbility;
UCLASS()
class DIABLOM_API UDiaTechnologyAsset : public UTechnologyAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UDiabloAbility> m_TalentAbility;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_TalentShowingName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_FormatSkillDesc;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=( MultiLine="true" ))
	FText m_FormatSkillPreviewLevelup;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_TextSkillAdditionalEffect;
	//요구 스탠스 및 주무기 보조무기 종류
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nRequirePointToUnlock;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nMaxSkillLevel;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)//cost  표시 어떻게?
	TArray<FLevelupableScaleFloat> m_ArySkillValue;//블프랑 코스트호환?//음수?
};



//시작하면 모든 데이터 테이블에 맞춰서 스펙을 만들어야함,저장되는것은 인덱스와 레벨뿐이다
USTRUCT(BlueprintType)
struct FTalentDataSpec
{
	GENERATED_BODY()
public:
	FTalentDataSpec(): m_nCurrentLevel(0), m_TalentDataPtr(nullptr)
	{
	}

	FTalentDataSpec(const UDiaTechnologyAsset* data)
	{
		m_nCurrentLevel=0;
		m_TalentDataPtr=data;
	}
	UPROPERTY(EditAnywhere)
	int m_nCurrentLevel;

	UPROPERTY()
	const UDiaTechnologyAsset* m_TalentDataPtr;


public:
	int GetRequirePointToUnlock()
	{
		return m_TalentDataPtr->m_nRequirePointToUnlock;
	}
	//
	FText GetDescFormatText() const
	{
		FFormatOrderedArguments Args;
		
		for(auto& SFloat : m_TalentDataPtr->m_ArySkillValue)
		{
			if(SFloat.m_bUseRandRange)
			{
				Args.Add( SFloat.GetRangeFormatText(m_nCurrentLevel));
			}
			else
			{
				Args.Add( SFloat.m_fScaleFloat.GetValueAtLevel(m_nCurrentLevel));
			}
		}
		
		FTextFormat FormatT = m_TalentDataPtr->m_FormatSkillDesc;
 
		return FText::Format(FormatT, Args);
	}

	FText GetLevelupPreviewFormatText() const
	{
		int NextSkillLevel = m_nCurrentLevel+1;
		
		FFormatOrderedArguments Args;

		for(auto& SFloat : m_TalentDataPtr->m_ArySkillValue)
		{
			if(!SFloat.m_bIsLevelupable)
			{
				continue;
			}

			if(SFloat.m_bUseRandRange)
			{
				Args.Add( SFloat.GetRangeFormatText(NextSkillLevel));
			}
			else
			{
				Args.Add( SFloat.m_fScaleFloat.GetValueAtLevel(NextSkillLevel));
			}
		}
 
		FTextFormat FormatT = m_TalentDataPtr->m_FormatSkillPreviewLevelup;
 
		return FText::Format(FormatT, Args);
	}


	bool IsUnlockable(int totalPointSpent)
	{
		return GetRequirePointToUnlock()<=totalPointSpent;
	}
};