// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Datas/DungeonDataTable.h"

#include "MapInfoPopup.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UMapInfoPopup : public UUserWidget
{
	GENERATED_BODY()
	
	//Current Dungeon Monster Level
	//-1 ~ +1 레벨 몬스터 젠
	//획득 가능 아이템?
	//클리어 보상, 골드 ,아이템,
	//던전이름
	//던전 아이콘
	
	//TArray UserWidget = 팝업창 끌때 삭제해야함
public:
	void OpenPopup(const FDungeonDataRow* dg_data);
};
