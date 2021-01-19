// My First Hack n Slash


#include "DiaSkillHotKeyPanel.h"

#include "Characters/DiabloPlayerController.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Objs/Actor/SkillIndicator.h"

void UDiaSkillHotKeyPanel::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    m_ArySkillButtons.Empty();
    m_ArySkillButtons.Add(m_SkillBtn1);
    m_ArySkillButtons.Add(m_SkillBtn2);
    m_ArySkillButtons.Add(m_SkillBtn3);
    m_ArySkillButtons.Add(m_SkillBtn4);
    m_ArySkillButtons.Add(m_SkillBtn5);


    FVector Loc = ADiabloPlayerController::Get->GetPlayerPawn()->GetActorLocation();
	
    FRotator Rot(0.f,0.f,0.f);
	
    FActorSpawnParameters Param;
	
    Param.bNoFail=true;

	check(m_ClassIndicator);
    m_Indicator = ADiabloPlayerController::Get->GetWorld()->SpawnActor<ASkillIndicator>(m_ClassIndicator,Loc,Rot,Param);
	ASkillIndicator::GetCurrent = m_Indicator;
	ASkillIndicator::GetCurrent->SetActorHiddenInGame(true);

	for(UDiaSkillUseButton* Btn : m_ArySkillButtons)
	{
		Btn->m_OnPressed.AddUObject(this,&UDiaSkillHotKeyPanel::DisableAllKeys);
		Btn->m_OnReleased.AddUObject(this,&UDiaSkillHotKeyPanel::EnableAllKeys);
	}

	m_AryWidgets.Empty();
	m_AryWidgets.Add(m_SkillBtn1);
	m_AryWidgets.Add(m_SkillBtn2);
	m_AryWidgets.Add(m_SkillBtn3);
	m_AryWidgets.Add(m_SkillBtn4);
	m_AryWidgets.Add(m_SkillBtn5);
	m_AryWidgets.Add(m_InteractButton);
	m_AryWidgets.Add(m_PotionButton);
}

void UDiaSkillHotKeyPanel::DisableAllKeys(UWidget* exceptThis)
{
	for(UWidget* Btn : m_AryWidgets)
	{
		if(Btn == exceptThis)
		{
			continue;
		}

		Btn->SetIsEnabled(false);
	}
}

void UDiaSkillHotKeyPanel::EnableAllKeys(UWidget* source)
{
	for(UWidget* Btn : m_AryWidgets)
	{
		Btn->SetIsEnabled(true);		
	}
}

void UDiaSkillHotKeyPanel::NativeDestruct()
{
	Super::NativeDestruct();
	ASkillIndicator::GetCurrent = nullptr;
}
