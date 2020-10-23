#include "DamageTextWidgetComponent.h"
#include "DiabloM.h"

void UDamageTextWidgetComponent::Init(AActor* base)
{
    m_Base=base;
    m_AttachedActor=m_Base;
}

void UDamageTextWidgetComponent::AttachToActor(AUnitPawn* newRoot)
{
    AttachToComponent(newRoot->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    newRoot->m_AttachedTextPopup=this;
    m_AttachedActor=newRoot;
}

void UDamageTextWidgetComponent::EndAnimation()
{
    AttachToComponent(m_Base->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    m_AttachedActor=m_Base;
    SetHiddenInGame(true);
    PRINTF("EndAnimation");
}
