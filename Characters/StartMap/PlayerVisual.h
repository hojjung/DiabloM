// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SkeletalMesh.h"
#include "GameFramework/Pawn.h"
#include "Managers/StartMap/PlayerCreateManager.h"

#include "PlayerVisual.generated.h"

struct FCurrentCharData;

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
	USkeletalMeshComponent* m_MeshFace;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	USkeletalMeshComponent* m_MeshBelt;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	USkeletalMeshComponent* m_MeshGlove;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	USkeletalMeshComponent* m_MeshShoe;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	USkeletalMeshComponent* m_MeshHeadGear;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	USkeletalMeshComponent* m_MeshShoulderPad;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	USkeletalMeshComponent* m_MeshHair;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* m_MeshBackpack;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* m_MeshRightHand;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* m_MeshLeftHand;
protected:
	UPROPERTY()
	UPlayerCreateManager* m_PlCreateManager;//smartponter?

	UPROPERTY()
	UAnimSequence* m_AnimSeq;
	UPROPERTY()
	USkeletalMesh* m_DefaultBodyMesh;
	UPROPERTY()
	USkeletalMesh* m_DefaultGloveMesh;
	UPROPERTY()
	USkeletalMesh* m_DefaultShoeMesh;
protected:
	void CreateSkMeshComponent(USkeletalMeshComponent** refSkComp,FName keyName);
	
	
	virtual void BeginPlay() override;
	void SetBodyAnim();

	void OnMeshVisualChanged(const FCurrentCharData& charData);

	void SetDefaultBodyMesh();

	void SetDefaultShoeMesh();

	void SetDefaultGloveMesh();
public:
	void ShowMesh();

	void HideMesh();

	
};
