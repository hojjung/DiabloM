// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "UObject/NoExportTypes.h"
#include "Datas/QuestData.h"
#include "QuestManager.generated.h"

UENUM()
enum class EQuestType :uint8
{
	MonsterKill,//monsterkill
	PlayTime,//playtime
	GachaCount,//gacha count
	AttackLv,//attacklv
	CriLv,//crilv
	CriDmgLv,//cridmglv
	SCriLv,//srcrilv
	SCriDmgLv,//scridmglv
	MaLv,//malv
	MaDmgLv,//madmglv
	SMaLv,//smalv
	SMaDmgLv,//smadmglv
	LENGTH
};
UCLASS()
class DIABLOM_API UQuestManager : public UObject
{
	GENERATED_BODY()
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FQuestUpdate,int);

	FQuestUpdate m_OnQuestUpdate;
	
protected:
	TArray<FQuestDataSpec> m_AryQuestData;

	UPROPERTY()
	int m_nWaitingGemStones;
	
public:
	void SetQuestDataFromServer(const FString& strQuest);

	FORCEINLINE TArray<FQuestDataSpec>& GetQuestData()
	{
		return m_AryQuestData;
	}

	bool CompleteQuest(int index);

	FQuestDataSpec& GetQuest(EQuestType type);

	void AddQuestCount(EQuestType type);

	void AddGemStones(int gemStone);

	void RequestGemStoneUploadToServer();

	FString GetQuestDataStr();
};
