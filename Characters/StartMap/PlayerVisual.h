// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SkeletalMesh.h"
#include "GameFramework/Pawn.h"
#include "Managers/StartMap/PlayerCreateManager.h"

#include "PlayerVisual.generated.h"

struct FCurrentCharData;
class USceneCaptureComponent2D;
UCLASS()
class DIABLOM_API APlayerVisual : public APawn
{
	GENERATED_BODY()
	
public:	
	APlayerVisual();
	
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	USkeletalMeshComponent* m_MeshBody;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	USceneCaptureComponent2D* m_Capture;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	USpringArmComponent* m_Spring;
	
protected:
	UPROPERTY()
	UPlayerCreateManager* m_PlCreateManager;//smartponter?
	UPROPERTY()
	UAnimSequence* m_AnimSeq;
	UPROPERTY()
	UTextureRenderTarget2D* m_CaptureTexture;
	UPROPERTY()
	TArray<AEquipmentActor*> m_AryVisualEquipment;

	FDelegateHandle Handle1;

	FDelegateHandle Handle2;
	
protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void CreateSkMeshComponent(USkeletalMeshComponent** refSkComp,FName keyName,USceneComponent* root);
	
	void OnMeshVisualChanged(const FCurrentCharData& charData);//TODO Add Slot reader,1003

public:
	void ShowMesh();

	void HideMesh();

	void ShowMeshWithTick();

	void HideMeshWithTick();
};
