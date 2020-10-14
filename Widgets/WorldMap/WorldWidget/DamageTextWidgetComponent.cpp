// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageTextWidgetComponent.h"

#include "DiabloM.h"

void UDamageTextWidgetComponent::Init(AActor* base)
{
    m_Base=base;
}

void UDamageTextWidgetComponent::EndAnimation()
{
    AttachToComponent(m_Base->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    m_AttachedActor=m_Base;
    SetHiddenInGame(true);
}
