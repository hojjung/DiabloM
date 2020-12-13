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
	FText m_TalentShowingName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_FormatSkillDesc;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=( MultiLine="true" ))
	FText m_FormatSkillPreviewLevelup;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_TextSkillAdditionalEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UDiabloAbility> m_TalentAbility;
	//요구 스탠스 및 주무기 보조무기 종류
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nRequirePointToUnlock;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)//cost  표시 어떻게?
	TArray<FLevelupableScaleFloat> m_ArySkillValue;//블프랑 코스트호환?//음수?
	


	FText GetTalentDesc()
	{
		FFormatOrderedArguments Args;
		
		for(auto& SFloat : m_ArySkillValue)
		{
			if(SFloat.m_bUseRandRange)
			{
				Args.Add( SFloat.GetRangeFormatText(m_nCurrentTalentLevel));
			}
			else
			{
				Args.Add( SFloat.m_fScaleFloat.GetValueAtLevel(m_nCurrentTalentLevel));
			}
		}
		
		FTextFormat FormatT = m_FormatSkillDesc;
 
		return FText::Format(FormatT, Args);
	}

	FText GetLevelupPreviewFormatText()
	{
		int NextSkillLevel = m_nCurrentTalentLevel+1;
		
		FFormatOrderedArguments Args;

		for(auto& SFloat : m_ArySkillValue)
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
 
		return FText::Format(m_FormatSkillPreviewLevelup, Args);
	}
};


