// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PlayerVisual.h"
#include "GameFramework/PlayerController.h"
#include "Widgets/StartMap/StartMenuCanvas.h"

#include "PlayerCreateController.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API APlayerCreateController : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerCreateController();
	
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UStartMenuCanvas> m_ClassStartMenu; 
	UPROPERTY()
	UStartMenuCanvas* m_StartCanvas;
	
	
public:
	virtual void BeginPlay() override;
	
	void InitWidget();	
	
	APlayerVisual* GetPlayerVisual();
	
};
