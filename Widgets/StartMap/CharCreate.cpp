
#include "CharCreate.h"




void UCharCreate::NativePreConstruct()
{
    Super::NativePreConstruct();
    m_PlManager=nullptr;
    m_FormatT= FText::FromString("{0}/{1}");
}

void UCharCreate::Init(PlayerCreateManager* plManager)
{
    m_PlManager=plManager;
    m_PlManager->m_OnVisualChange.AddUObject(this,&UCharCreate::OnVisualChanged);
    m_HairSelect->m_BtnLeft->OnClicked.AddDynamic(m_PlManager,&PlayerCreateManager::DecreaseHair);
    m_HairSelect->m_BtnRight->OnClicked.AddDynamic(m_PlManager,&PlayerCreateManager::IncreaseHair);
}

void UCharCreate::OnVisualChanged(const FCurrentCharData& visual_change)
{
    m_HairSelect->m_TextTypeName->SetText(visual_change.m_CurrentHair->m_ShowingName);
    m_HairSelect->m_TextTypeCount->SetText(GetFormatCount(m_PlManager->m_IndexHair,m_PlManager->m_AryHair.Num()));
}

FText UCharCreate::GetFormatCount(int index, int aryMax)
{
    index++;//for show index
    FFormatOrderedArguments Args;
    Args.Add(index);
    Args.Add(aryMax);
    return FText::Format(m_FormatT, Args);
}
