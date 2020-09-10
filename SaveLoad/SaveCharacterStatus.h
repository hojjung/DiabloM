// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"


#include "Characters/DiabloPlayerController.h"
#include "GameFramework/SaveGame.h"
#include "SaveLoadManagerOld.h"
#include "SaveCharacterStatus.generated.h"

/**
 * //얘를 일단 전부 로드
 * 그다음 위젯 연동
 * 이름 과 레벨
 * 세이브로드 매니저랑 전체 캐릭터인포 위젯이랑 연동시킬것
 * 캐릭터 스테이터스만 로드?
 */


UCLASS()
class DIABLOM_API USaveCharacterStatus : public USaveGame
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere)
    ESaveVersion m_SaveVersion;
    UPROPERTY(EditAnywhere)
    int m_nSlotIndex;
    UPROPERTY(EditAnywhere)
    FString m_TextName;
    UPROPERTY(EditAnywhere)
    int m_nLevel=1;
    UPROPERTY(EditAnywhere)
    int m_IndexHair=0;
    UPROPERTY(EditAnywhere)
    int m_IndexFace=0;

public:
    void SaveCharStatSave(int index,int level,FText playerName);

    void SetCharStatLoad(ADiabloPlayerController* diaPl);
};

