#include "DgToVillagePortal.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/DungeonManager.h"


void ADgToVillagePortal::BeginPlay()
{
	AVillageActor::BeginPlay();

	SetPortal(true);
}


void ADgToVillagePortal::Interact(AActor* instigator)
{
	TWeakObjectPtr<ADiabloPlayerController> DiaPC = ADiabloPlayerController::Get;

	DiaPC.Get()->OpenMapSelectMenu(true);

	//맵선택 위젯 보여줘야함
	//DungeonM->PortalToVillage();
}
