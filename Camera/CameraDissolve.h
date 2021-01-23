// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "CameraDissolve.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIABLOM_API UCameraDissolve : public USceneComponent
{
	GENERATED_BODY()

	public:
	UCameraDissolve();

protected:
	FVector m_RelativeSocketLocation;

	FQuat m_RelativeSocketRotation;

	FVector m_CompOrigin;

	FVector m_TargetPos;

	FVector m_PreviousPos;

	bool m_bWasBlocked;

	float m_fDissolvingTime;

	float m_fTimer;

	float m_fDissloveAmount;

	float m_fDissolveHoleRadius;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float m_fDissolveMaxAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float m_fDissolveHoleMaxRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float m_fCastSphereRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	FVector m_CastOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Lag, meta=( ClampMin="0.0", ClampMax="1000.0", UIMin = "0.0", UIMax = "1000.0"))
	float m_CameraLagSpeed;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float TargetArmLength;
	UPROPERTY()
	UMaterialParameterCollection* m_MatParamAsset;
	UPROPERTY()
	UMaterialParameterCollectionInstance* m_MatParamInstance;
	UPROPERTY()
	USceneComponent* m_TargetCam;

public:
	void Init(USceneComponent* camWantFollow);

	void StartDissolve();

	void EndDissolve();

	virtual FTransform GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace = RTS_World) const override;

protected:
	void ExecuteDissolve(float DeltaTime);

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void SetPosParameter();
	void SetValueParameter();
	//

	virtual void UpdateDesiredArmLocation(float DeltaTime);
};
