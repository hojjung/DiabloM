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
    m_HairSelect->m_BtnLeft->OnClicked.AddDynamic(this,&UCharCreate::DecreaseHair);
    m_HairSelect->m_BtnRight->OnClicked.AddDynamic(this,&UCharCreate::IncreaseHair);
    m_FaceSelect->m_BtnLeft->OnClicked.AddDynamic(this,&UCharCreate::DecreaseFace);
    m_FaceSelect->m_BtnRight->OnClicked.AddDynamic(this,&UCharCreate::IncreaseFace);
    m_ArmorSelect->m_BtnLeft->OnClicked.AddDynamic(this,&UCharCreate::DecreaseArmor);
    m_ArmorSelect->m_BtnRight->OnClicked.AddDynamic(this,&UCharCreate::IncreaseArmor);
    m_ItemSelect->m_BtnLeft->OnClicked.AddDynamic(this,&UCharCreate::DecreaseItem);
    m_ItemSelect->m_BtnRight->OnClicked.AddDynamic(this,&UCharCreate::IncreaseItem);
    m_PerkSelect->m_BtnLeft->OnClicked.AddDynamic(this,&UCharCreate::DecreasePerk);
    m_PerkSelect->m_BtnRight->OnClicked.AddDynamic(this,&UCharCreate::IncreasePerk);
    m_NameBox->OnTextChanged.AddDynamic(this,&UCharCreate::UpdateNameText);
    m_NameBox->OnTextCommitted.AddDynamic(this,&UCharCreate::UpdateNameTextCommit);
    m_BtnContinue->OnClicked.AddDynamic(this,&UCharCreate::Continue);
}

void UCharCreate::OnVisualChanged(const FCurrentCharData& visual_change)
{
    m_HairSelect->m_TextTypeName->SetText(visual_change.m_TextNameHair);
    m_HairSelect->m_TextTypeCount->SetText(GetFormatCount(m_PlManager->m_IndexHair,m_PlManager->m_AryHair.Num()));
    //
    m_FaceSelect->m_TextTypeName->SetText(visual_change.m_TextNameFace);
    m_FaceSelect->m_TextTypeCount->SetText(GetFormatCount(m_PlManager->m_IndexFace,m_PlManager->m_AryFace.Num()));
    //
    m_ArmorSelect->m_TextTypeName->SetText(visual_change.m_TextNameArmor);
    m_ArmorSelect->m_TextTypeCount->SetText(GetFormatCount(m_PlManager->m_IndexArmor,m_PlManager->m_AryArmor.Num()));
    //
    //
    m_ItemSelect->m_TextTypeName->SetText(visual_change.m_TextNameItem);
    m_ItemSelect->m_TextTypeCount->SetText(GetFormatCount(m_PlManager->m_IndexItem,m_PlManager->m_AryItem.Num()));
    //
    m_PerkSelect->m_TextTypeName->SetText(visual_change.m_TextNamePerk);
    m_PerkSelect->m_TextTypeCount->SetText(GetFormatCount(m_PlManager->m_IndexPerk,m_PlManager->m_AryPerk.Num()));
    
}

void UCharCreate::DecreaseHair()
{
    m_PlManager->DecreaseHair();
}

void UCharCreate::IncreaseHair()
{
    m_PlManager->IncreaseHair();
}

void UCharCreate::DecreaseFace()
{
    m_PlManager->DecreaseFace();
}

void UCharCreate::IncreaseFace()
{
    m_PlManager->IncreaseFace();
}

void UCharCreate::DecreaseArmor()
{
    m_PlManager->DecreaseArmor();
}

void UCharCreate::IncreaseArmor()
{
    m_PlManager->IncreaseArmor();
}


void UCharCreate::DecreaseItem()
{
    m_PlManager->DecreaseItem();
}

void UCharCreate::IncreaseItem()
{
    m_PlManager->IncreaseItem();
}

void UCharCreate::DecreasePerk()
{
    m_PlManager->DecreasePerk();
}

void UCharCreate::IncreasePerk()
{
    m_PlManager->IncreasePerk();
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

    //SelectSlot
    //
    //focus need
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
