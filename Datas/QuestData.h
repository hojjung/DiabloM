// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "UObject/NoExportTypes.h"
#include "QuestData.generated.h"

UCLASS()
class DIABLOM_API UQuestData : public UObject
{
	GENERATED_BODY()
public:
	static UDataTable* GetQuestData;

	UQuestData()
	{
		static ConstructorHelpers::FObjectFinder<UDataTable> Found(
			TEXT("DataTable'/Game/DataTables/QuestDataTable.QuestDataTable'"));

		GetQuestData = Found.Object;
	}
};

USTRUCT(BlueprintType) //���̵�,Ƽ��
struct FQuestLevelData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nRequireData = 10;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nRewardGemStone = 15;
};

USTRUCT(BlueprintType) //���̵�,Ƽ��
struct FQuestDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_TextQuestName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine = true))
	FString m_DescFormat = "Lv.{0}";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FQuestLevelData> m_AryQuestData;

	FText GetMaxFormatDesc(int questLv) const
	{
		FTextFormat Format = FText::FromString(m_DescFormat);

		FFormatOrderedArguments Args;

		Args.Add(m_AryQuestData[questLv].m_nRequireData);

		FText TT = FText::Format(Format, Args);

		return TT;
	}

	FText GetCurrentFormatDesc(int currentValue) const
	{
		FTextFormat Format = FText::FromString(m_DescFormat);

		FFormatOrderedArguments Args;

		Args.Add(currentValue);

		FText TT = FText::Format(Format, Args);

		return TT;
	}
};

USTRUCT(BlueprintType) //���̵�,Ƽ��
struct FQuestDataSpec
{
	GENERATED_BODY()

public:
	FQuestDataRow* m_Data;

	UPROPERTY()
	int m_nCurrentLv;
	UPROPERTY()
	int m_nCurrentRequirePoint;

	FString ParseToStr()
	{
		FString StrW;
		StrW.AppendInt(m_nCurrentLv);
		StrW.Append(":");
		StrW.AppendInt(m_nCurrentRequirePoint);
		StrW.Append(":");

		return StrW;
	}

	void ParseFromStr(FString strServer)
	{
		TArray<FString> OutStrAry;

		strServer.ParseIntoArray(OutStrAry,TEXT(":"));

		m_nCurrentLv = FCString::Atoi(*OutStrAry[0]);

		m_nCurrentRequirePoint = FCString::Atoi(*OutStrAry[1]);
	}

	bool TryComplete()
	{
		if (IsMaxLv())
		{
			return false;
		}

		if (!IsCompletable())
		{
			return false;
		}

		m_nCurrentLv++;
		
		return true;
	}

	bool IsCompletable()
	{
		return m_nCurrentRequirePoint >= m_Data->m_AryQuestData[m_nCurrentLv].m_nRequireData;
	}

	bool IsMaxLv()
	{
		return m_Data->m_AryQuestData.Num() <= m_nCurrentLv + 1; // 100 ,99
	} //10개,최대인덱스 9,현재 인덱스 8

	int GetMaxRequireValue()
	{
		return m_Data->m_AryQuestData[m_nCurrentLv].m_nRequireData;
	}

	int GetCurrentData()
	{
		return m_nCurrentRequirePoint;
	}

	float GetGaugePercent()
	{
		float Current = GetCurrentData();

		float Max = GetMaxRequireValue();
		
		return FMath::Clamp(Current / Max,0.f,1.f);
	}

	FText GetGaugeFormatTxt()
	{
		FTextFormat Format = FText::FromString("{0}/{1}");

		FFormatOrderedArguments Args;

		Args.Add(m_Data->GetCurrentFormatDesc(GetCurrentData()));
	
		Args.Add(m_Data->GetMaxFormatDesc(m_nCurrentLv));

		FText TT = FText::Format(Format, Args);

		return TT;
	}

	int GetCompletePrize()
	{
		return m_Data->m_AryQuestData[m_nCurrentLv].m_nRewardGemStone;
	}
};
