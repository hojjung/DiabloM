#include "CharCreate.h"

#include "CharSelect.h"
#include "Characters/StartMap/PlayerCreateController.h"

void UCharCreate::NativePreConstruct()
{
    Super::NativePreConstruct();
    m_PlManager=nullptr;
    m_FormatT= FText::FromString("{0}/{1}");
}

void UCharCreate::Init(UPlayerCreateManager* plManager)
{
    m_PlManager=plManager;
    m_PlManager->m_OnVisualChange.AddUObject(this,&UCharCreate::OnVisualChanged);
    m_ClassItemSelect->m_BtnLeft->OnClicked.AddDynamic(this,&UCharCreate::DecreaseItem);
    m_ClassItemSelect->m_BtnRight->OnClicked.AddDynamic(this,&UCharCreate::IncreaseItem);
    m_NameBox->OnTextChanged.AddDynamic(this,&UCharCreate::UpdateNameText);
    m_NameBox->OnTextCommitted.AddDynamic(this,&UCharCreate::UpdateNameTextCommit);
    m_BtnContinue->OnClicked.AddDynamic(this,&UCharCreate::Continue);
}

void UCharCreate::OnVisualChanged(const FCurrentCharData& visual_change)
{
    m_ClassItemSelect->m_TextTypeName->SetText(visual_change.m_TextNameClass);
    m_ClassItemSelect->m_TextTypeCount->SetText(GetFormatCount(m_PlManager->m_IndexItem,m_PlManager->m_AryInitItem.Num()));
}

void UCharCreate::DecreaseItem()
{
    m_PlManager->DecreaseItem();
}

void UCharCreate::IncreaseItem()
{
    m_PlManager->IncreaseItem();
}

void UCharCreate::UpdateNameText(const FText& text)
{
    if(m_NameBox->GetText().IsEmpty())
    {
        m_BtnContinue->SetIsEnabled(false);
        return;
    }

    if(m_NameBox->GetText().ToString().Len() > 15)
    {
        FString TheString= m_NameBox->GetText().ToString();
        m_NameBox->SetText(FText::FromString(UKismetStringLibrary::GetSubstring(TheString,0,14)));
    }
    
    m_PlManager->m_CurrentTextName=m_NameBox->GetText();
    
    m_BtnContinue->SetIsEnabled(true);
}


void UCharCreate::UpdateNameTextCommit(const FText& text, ETextCommit::Type type)
{
    UpdateNameText(text);
}

void UCharCreate::Continue()
{
    PRINTF("CharCreate Contine");
    m_NameBox->SetText(FText());
    int Index=m_PlManager->DoneCreateCharcter();
    m_BtnContinue->SetIsEnabled(false);
    m_PlManager->m_CurrentTextName=FText();
    //Cast<APlayerCreateController>( GetOwningPlayer())->GetPlayerVisual()->HideMesh();
    m_CharSelect->m_AryCharInfoSlot[Index]->SelectSlot();
}

void UCharCreate::BackCancel()
{
    PRINTF("BackCancel");
    m_NameBox->SetText(FText());
    m_BtnContinue->SetIsEnabled(false);
    m_PlManager->m_CurrentTextName=FText();
}

void UCharCreate::StartCreation()
{
    m_BtnContinue->SetIsEnabled(false);
    m_PlManager->m_CurrentTextName=FText();
}

FText UCharCreate::GetFormatCount(int index, int aryMax)
{
    index++;//for show index
    FFormatOrderedArguments Args;
    Args.Add(index);
    Args.Add(aryMax);
    return FText::Format(m_FormatT, Args);
}
