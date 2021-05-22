// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Item/EquipmentActor.h"


#include "PlayerVisual.generated.h"

class APlayerDiabloCharacter;
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Player")
	UChildActorComponent* m_PetComp;
protected:
	UPROPERTY()
	UAnimSequence* m_AnimSeq;
	UPROPERTY()
	UTextureRenderTarget2D* m_CaptureTexture;

	UPROPERTY()
	AEquipmentActor* m_WeaponActor;
	UPROPERTY()
	AEquipmentActor* m_WingActor;
	
protected:
	virtual void BeginPlay() override;

	void CreateSkMeshComponent(USkeletalMeshComponent** refSkComp,FName keyName,USceneComponent* root);
	
public:
	void ShowMesh();

	void HideMesh();

	void ShowMeshWithTick();

	void HideMeshWithTick();

	void OnMeshChanged(APlayerDiabloCharacter* charDia);

	USkeletalMeshComponent* GetSkMesh()
	{
		return m_MeshBody;
	}
};
