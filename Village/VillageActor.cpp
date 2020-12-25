// Fill out your copyright notice in the Description page of Project Settings.


#include "VillageActor.h"

// Sets default values
AVillageActor::AVillageActor(const FObjectInitializer& objInit):Super(objInit)
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent=CreateDefaultSubobject<USceneComponent>("Root");
	m_Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule00");
	m_Capsule->SetupAttachment(RootComponent);
	m_Capsule->InitCapsuleSize(34.0f, 88.0f);
	m_Capsule->SetCollisionProfileName("PickupItem");
	m_Capsule->CanCharacterStepUpOn = ECB_No;
	m_Capsule->SetShouldUpdatePhysicsVolume(true);
	m_Capsule->SetCanEverAffectNavigation(false);
	m_Capsule->bDynamicObstacle = true;
	
	
	m_Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	m_WidgetComp=CreateDefaultSubobject<UWidgetComponent>("WidgetComp00");
	m_WidgetComp->SetupAttachment(RootComponent);
	m_WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	m_WidgetComp->SetDrawAtDesiredSize(true);
}



void AVillageActor::Interact(AActor* instigator)
{
	
}

