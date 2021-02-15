// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "Distributions/DistributionVectorParameterBase.h"
#include "DistributionVectorParameterCurve.generated.h"

/**
 * 
 */
UCLASS(collapsecategories, hidecategories=Object, editinlinenew)
class DIABLOM_API UDistributionVectorParameterCurve : public UDistributionVectorParameterBase
{
	GENERATED_BODY()
	
	 /** Keyframe data for each component (X,Y,Z) over time. */
	 UPROPERTY(EditAnywhere, Category=DistributionVectorConstantCurve)
	 FInterpCurveVector ConstantCurve;
	
	 //Begin UDistributionVector Interface
	 virtual FVector	GetValue( float F = 0.f, UObject* Data = NULL, int32 LastExtreme = 0, struct FRandomStream* InRandomStream = NULL ) const override;
	 virtual	void	GetRange(FVector& OutMin, FVector& OutMax) const override;
	 //End UDistributionVector Interface
	
	 //~ Begin FCurveEdInterface Interface
	 virtual int32		GetNumKeys() const override;
	 virtual int32		GetNumSubCurves() const override;
	 virtual FColor	GetSubCurveButtonColor(int32 SubCurveIndex, bool bIsSubCurveHidden) const override;
	 virtual float	GetKeyIn(int32 KeyIndex) override;
	 virtual float	GetKeyOut(int32 SubIndex, int32 KeyIndex) override;
	 virtual void	GetInRange(float& MinIn, float& MaxIn) const override;
	 virtual void	GetOutRange(float& MinOut, float& MaxOut) const override;
	 virtual FColor	GetKeyColor(int32 SubIndex, int32 KeyIndex, const FColor& CurveColor) override;
	 virtual EInterpCurveMode	GetKeyInterpMode(int32 KeyIndex) const override;
	 virtual void	GetTangents(int32 SubIndex, int32 KeyIndex, float& ArriveTangent, float& LeaveTangent) const override;
	 virtual float	EvalSub(int32 SubIndex, float InVal) override;
	 virtual int32		CreateNewKey(float KeyIn) override;
	 virtual void	DeleteKey(int32 KeyIndex) override;
	 virtual int32		SetKeyIn(int32 KeyIndex, float NewInVal) override;
	 virtual void	SetKeyOut(int32 SubIndex, int32 KeyIndex, float NewOutVal) override;
	 virtual void	SetKeyInterpMode(int32 KeyIndex, EInterpCurveMode NewMode) override;
	 virtual void	SetTangents(int32 SubIndex, int32 KeyIndex, float ArriveTangent, float LeaveTangent) override;
	 //~ End FCurveEdInterface Interface
	
	 //~ Begin UDistributionVectorParameterBase Interface
	 virtual bool GetParamValue(UObject* Data, FName ParamName, FVector& OutVector) const override;
	//	~ End UDistributionVectorParameterBase Interface

	virtual bool IsPostLoadThreadSafe() const override {return true;};
};
