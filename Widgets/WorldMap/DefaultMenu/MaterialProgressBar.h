// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "MaterialProgressBar.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UMaterialProgressBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMaterialProgressBar(const FObjectInitializer& objInit);

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName m_NameProgressValue;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(ExposeOnSpawn="true"))
	UMaterialInterface* m_MatWant;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Transient)
	UMaterialInstanceDynamic* m_MatInst;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_ImageBar;
	
public:
	
	UFUNCTION(BlueprintCallable)
	void SetProgressValue(float v);
	
	virtual bool Initialize() override;

	void SetMatToBrush();
	
	void CreateDynamicMat();
};
