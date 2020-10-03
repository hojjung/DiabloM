// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultFSM.h"

UDefaultFSM::UDefaultFSM()
{
    m_OwnerUnit=nullptr;
}

void UDefaultFSM::Init(AUnitPawn* pawnUnit)
{
    m_OwnerUnit=pawnUnit;
}

void UDefaultFSM::TickFSM()
{
}
