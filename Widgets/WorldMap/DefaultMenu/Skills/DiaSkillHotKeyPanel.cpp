// My First Hack n Slash


#include "DiaSkillHotKeyPanel.h"

#include "Characters/DiabloPlayerController.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Objs/Actor/SkillIndicator.h"

void UDiaSkillHotKeyPanel::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    m_AryButtons.Empty();
    m_AryButtons.Add(m_SkillBtn1);
    m_AryButtons.Add(m_SkillBtn2);
    m_AryButtons.Add(m_SkillBtn3);
    m_AryButtons.Add(m_SkillBtn4);
    m_AryButtons.Add(m_SkillBtn5);


    FVector Loc = ADiabloPlayerController::Get->GetPlayerPawn()->GetActorLocation();
	
    FRotator Rot(0.f,0.f,0.f);
	
    FActorSpawnParameters Param;
	
    Param.bNoFail=true;

	check(m_ClassIndicator);
    m_Indicator = ADiabloPlayerController::Get->GetWorld()->SpawnActor<ASkillIndicator>(m_ClassIndicator,Loc,Rot,Param);
	ASkillIndicator::GetCurrent = m_Indicator;
	ASkillIndicator::GetCurrent->SetActorHiddenInGame(true);
}

void UDiaSkillHotKeyPanel::NativeDestruct()
{
	Super::NativeDestruct();
	ASkillIndicator::GetCurrent = nullptr;
}
