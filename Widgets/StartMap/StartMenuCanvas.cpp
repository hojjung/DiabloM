// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMenuCanvas.h"

#include "Managers/DiabloGameInstance.h"


void UStartMenuCanvas::InitStartMenu()
{
    PRINTF("InitStartMenu");
    m_CharCreate->Init( GetGameInstance<UDiabloGameInstance>()->m_PlCreateManager);

}
