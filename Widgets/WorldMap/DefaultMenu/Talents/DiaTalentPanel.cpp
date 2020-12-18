#include "DiaTalentPanel.h"
#include "TechTreeWidget.h"
#include "TTTechNode.h"

#define LOCTEXT_NAMESPACE "DiaTalentPanel"

void UDiaTalentPanel::Init(UPlayerDiabloAbilitySystemComp* player)
{
    m_FormatTalentPoint =LOCTEXT("TalentPanelPointAvailable","Points Available:{0}");
	
    m_FormatTotalTalentPoint =LOCTEXT("TalentPanelPointTotal","Total Points Spent:{0}");
    
    m_PlayerTalentComp=player;

    m_SelectedTalentTechtree=nullptr;

    m_WarriorTechTreeWidget->SetVisibility(ESlateVisibility::Collapsed);
    m_HunterTechTreeWidget->SetVisibility(ESlateVisibility::Collapsed);
    m_MageTechTreeWidget->SetVisibility(ESlateVisibility::Collapsed);
    
    if(m_PlayerTalentComp->GetCharacterClass()=="Warrior")
    {
          m_SelectedTalentTechtree = m_WarriorTechTreeWidget;  
    }
    else if(m_PlayerTalentComp->GetCharacterClass()=="Hunter")
    {
        m_SelectedTalentTechtree = m_HunterTechTreeWidget;  
    }
    else if(m_PlayerTalentComp->GetCharacterClass()=="Mage")
    {
        m_SelectedTalentTechtree = m_MageTechTreeWidget;
    }

    m_SelectedTalentTechtree->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

    UTechnologyTree* TemplateTree =m_SelectedTalentTechtree->GetTechTree();
    m_PlayerTalentComp->AssignTechTreeWidget(TemplateTree,m_SelectedTalentTechtree);

    m_PlayerTalentComp->m_OnTalentChanged.AddUObject(this,&UDiaTalentPanel::UpdateAvailablePoint);
    m_PlayerTalentComp->m_OnTalentChanged.AddUObject(this,&UDiaTalentPanel::UpdateTotalPoint);
    m_PlayerTalentComp->m_OnTalentChanged.AddUObject(m_SelectedTalentTechtree,&UTechTreeWidget::UpdateSlotWidgets);
    m_PlayerTalentComp->m_OnTalentChanged.AddUObject(m_SelectedTalentTechtree,&UTechTreeWidget::UpdateTreeConnections);
    
    m_ResetButton->OnClicked.AddDynamic(this,&UDiaTalentPanel::ResetTalentPoint);

    UpdateAvailablePoint();
    
    UpdateTotalPoint();
}

TArray<UTechNodeWidget*> UDiaTalentPanel::GetTechNodeWidgets()
{
    return m_SelectedTalentTechtree->GetTechNodeWidgets();
}

void UDiaTalentPanel::UpdateAvailablePoint()
{
    FFormatOrderedArguments Args;

    Args.Add(m_PlayerTalentComp->GetTalentPoint());
	
    FText Result = FText::Format(m_FormatTalentPoint, Args);
    
    m_TextPointAvailable->SetText(Result);
}

void UDiaTalentPanel::UpdateTotalPoint()
{
    FFormatOrderedArguments Args;

    Args.Add(m_PlayerTalentComp->GetTotalTalentPoint());
	
    FText Result = FText::Format(m_FormatTotalTalentPoint, Args);

    m_TextPointTotalSpent->SetText(Result);
}

void UDiaTalentPanel::ResetTalentPoint()
{
    m_PlayerTalentComp->ResetTech();

    UpdateTotalPoint();
    UpdateAvailablePoint();
}

#undef LOCTEXT_NAMESPACE
