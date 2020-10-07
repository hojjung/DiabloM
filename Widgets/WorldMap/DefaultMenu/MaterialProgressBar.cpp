// Fill out your copyright notice in the Description page of Project Settings.


#include "MaterialProgressBar.h"

#include "Materials/MaterialInstanceDynamic.h"

UMaterialProgressBar::UMaterialProgressBar(const FObjectInitializer& objInit):Super(objInit)
{
    m_MatWant=nullptr;
    m_MatInst=nullptr;
}

bool UMaterialProgressBar::Initialize()
{
    Super::Initialize();
    if(!m_ImageBar)
    {
        return false;
    }
    CreateDynamicMat();
    SetMatToBrush();
    return true;
}

void UMaterialProgressBar::SetMatToBrush()
{
    FSlateBrush NewBrush;
    NewBrush.SetResourceObject(m_MatInst);
    m_ImageBar->SetBrush(NewBrush);
}

void UMaterialProgressBar::CreateDynamicMat()
{
    m_MatInst=UMaterialInstanceDynamic::Create(m_MatWant, this);
}

void UMaterialProgressBar::SetProgressValue(float v)
{
    m_MatInst->SetScalarParameterValue(m_NameProgressValue,v);
}
