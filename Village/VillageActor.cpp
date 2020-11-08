// Fill out your copyright notice in the Description page of Project Settings.


#include "VillageActor.h"

// Sets default values
AVillageActor::AVillageActor()
{
	PrimaryActorTick.bCanEverTick = false;
	m_Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule00");
	m_Capsule->InitCapsuleSize(34.0f, 88.0f);
	//m_Capsule->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	m_Capsule->SetCollisionProfileName("PickupItem");
	m_Capsule->CanCharacterStepUpOn = ECB_No;
	m_Capsule->SetShouldUpdatePhysicsVolume(true);
	m_Capsule->SetCanEverAffectNavigation(false);
	m_Capsule->bDynamicObstacle = true;
	RootComponent=m_Capsule;
	
	m_Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}



void AVillageActor::Interact(AActor* instigator)
{
	
}

