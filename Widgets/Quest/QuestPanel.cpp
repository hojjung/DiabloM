#include "QuestPanel.h"

#include "Managers/DiabloGameInstance.h"

void UQuestPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnClose->OnClicked.AddDynamic(this,&UQuestPanel::ClosePanel);

	m_AryQuestBtns.Reset();

	int index = 0;

	for(FQuestDataSpec& QuestDataSpec :  UDiabloGameInstance::Get->m_QuestManager->GetQuestData())
	{
		UQuestButton* CreatedQuestBtn = CreateWidget<UQuestButton>(this,m_ClassQuest);

		CreatedQuestBtn->Init(index,&QuestDataSpec);

		index++;

		m_GridPanel->AddChildToWrapBox(CreatedQuestBtn);

		CreatedQuestBtn->SetPadding(FMargin(10,10,10,10));

		m_AryQuestBtns.Add(CreatedQuestBtn);

		UDiabloGameInstance::Get->m_QuestManager->m_OnQuestUpdate.AddUObject(this,&UQuestPanel::UpdateQuest);
	}
}

void UQuestPanel::ClosePanel()
{
	SetVisibility(ESlateVisibility::Collapsed);
	UDiabloGameInstance::Get->m_ShopManager->ShowBannerAD(true);
}

void UQuestPanel::UpdateQuest(int index)
{
	m_AryQuestBtns[index]->UpdateQuestWidget();
}

