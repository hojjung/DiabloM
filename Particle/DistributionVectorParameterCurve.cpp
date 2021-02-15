#include "DistributionVectorParameterCurve.h"

#include "Distributions/DistributionVectorParticleParameter.h"
#include "Particles/ParticleSystemComponent.h"


FVector UDistributionVectorParameterCurve::GetValue(float F, UObject* Data, int32 Extreme, struct FRandomStream* InRandomStream) const
{
	//
	FVector ParamVector(0.f);
	bool bFoundParam = GetParamValue(Data, ParameterName, ParamVector);
	if(!bFoundParam)
	{
		ParamVector = Constant;
	}
	//
	FVector Val = ConstantCurve.Eval(F, FVector::ZeroVector);
	
	switch (LockedAxes)
	{
	case EDVLF_XY:
		return FVector(Val.X, Val.X, Val.Z);
	case EDVLF_XZ:
		return FVector(Val.X, Val.Y, Val.X);
	case EDVLF_YZ:
		return FVector(Val.X, Val.Y, Val.Y);
	case EDVLF_XYZ:
		return FVector(Val.X);
	case EDVLF_None:
	default:
		return Val;
	}
}

int32 UDistributionVectorParameterCurve::GetNumKeys() const
{
	return ConstantCurve.Points.Num();
}

int32 UDistributionVectorParameterCurve::GetNumSubCurves() const
{
	switch (LockedAxes)
	{
	case EDVLF_XY:
	case EDVLF_XZ:
	case EDVLF_YZ:
		return 2;
	case EDVLF_XYZ:
		return 1;
	}
	return 3;
}

FColor UDistributionVectorParameterCurve::GetSubCurveButtonColor(int32 SubCurveIndex, bool bIsSubCurveHidden) const
{
	// Check for array out of bounds because it will crash the program
	check(SubCurveIndex >= 0);
	check(SubCurveIndex < GetNumSubCurves());

	FColor ButtonColor;

	switch(SubCurveIndex)
	{
	case 0:
		// Red
		ButtonColor = bIsSubCurveHidden ? FColor(32, 0, 0) : FColor::Red;
		break;
	case 1:
		// Green
		ButtonColor = bIsSubCurveHidden ? FColor(0, 32, 0) : FColor::Green;
		break;
	case 2:
		// Blue
		ButtonColor = bIsSubCurveHidden ? FColor(0, 0, 32) : FColor::Blue;
		break;
	default:
		// A bad sub-curve index was given. 
		check(false);
		break;
	}

	return ButtonColor;
}

float UDistributionVectorParameterCurve::GetKeyIn(int32 KeyIndex)
{
	check( KeyIndex >= 0 && KeyIndex < ConstantCurve.Points.Num() );
	return ConstantCurve.Points[KeyIndex].InVal;
}

float UDistributionVectorParameterCurve::GetKeyOut(int32 SubIndex, int32 KeyIndex)
{
	check( SubIndex >= 0 && SubIndex < 3);
	check( KeyIndex >= 0 && KeyIndex < ConstantCurve.Points.Num() );
	
	if (SubIndex == 0)
	{
		return ConstantCurve.Points[KeyIndex].OutVal.X;
	}
	else
	if(SubIndex == 1)
	{
		if ((LockedAxes == EDVLF_XY) || (LockedAxes == EDVLF_XYZ))
		{
			return ConstantCurve.Points[KeyIndex].OutVal.X;
		}

		return ConstantCurve.Points[KeyIndex].OutVal.Y;
	}
	else 
	{
		if ((LockedAxes == EDVLF_XZ) || (LockedAxes == EDVLF_XYZ))
		{
			return ConstantCurve.Points[KeyIndex].OutVal.X;
		}
		else
		if (LockedAxes == EDVLF_YZ)
		{
			return ConstantCurve.Points[KeyIndex].OutVal.Y;
		}

		return ConstantCurve.Points[KeyIndex].OutVal.Z;
	}
}

FColor UDistributionVectorParameterCurve::GetKeyColor(int32 SubIndex, int32 KeyIndex, const FColor& CurveColor)
{
	check( SubIndex >= 0 && SubIndex < 3);
	check( KeyIndex >= 0 && KeyIndex < ConstantCurve.Points.Num() );

	if (SubIndex == 0)
	{
		return FColor::Red;
	}
	else if (SubIndex == 1)
	{
		return FColor::Green;
	}
	else
	{
		return FColor::Blue;
	}
}

void UDistributionVectorParameterCurve::GetInRange(float& MinIn, float& MaxIn) const
{
	if( ConstantCurve.Points.Num() == 0 )
	{
		MinIn = 0.f;
		MaxIn = 0.f;
	}
	else
	{
		float Min = BIG_NUMBER;
		float Max = -BIG_NUMBER;
		for (int32 Index = 0; Index < ConstantCurve.Points.Num(); Index++)
		{
			float Value = ConstantCurve.Points[Index].InVal;
			if (Value < Min)
			{
				Min = Value;
			}
			if (Value > Max)
			{
				Max = Value;
			}
		}
		MinIn = Min;
		MaxIn = Max;
	}
}

void UDistributionVectorParameterCurve::GetOutRange(float& MinOut, float& MaxOut) const
{
	FVector MinVec, MaxVec;
	ConstantCurve.CalcBounds(MinVec, MaxVec, FVector::ZeroVector);

	switch (LockedAxes)
	{
	case EDVLF_XY:
		MinVec.Y = MinVec.X;
		MaxVec.Y = MaxVec.X;
		break;
	case EDVLF_XZ:
		MinVec.Z = MinVec.X;
		MaxVec.Z = MaxVec.X;
		break;
	case EDVLF_YZ:
		MinVec.Z = MinVec.Y;
		MaxVec.Z = MaxVec.Y;
		break;
	case EDVLF_XYZ:
		MinVec.Y = MinVec.X;
		MinVec.Z = MinVec.X;
		MaxVec.Y = MaxVec.X;
		MaxVec.Z = MaxVec.X;
		break;
	case EDVLF_None:
	default:
		break;
	}

	MinOut = MinVec.GetMin();
	MaxOut = MaxVec.GetMax();
}

EInterpCurveMode UDistributionVectorParameterCurve::GetKeyInterpMode(int32 KeyIndex) const
{
	check( KeyIndex >= 0 && KeyIndex < ConstantCurve.Points.Num() );
	return ConstantCurve.Points[KeyIndex].InterpMode;
}

void UDistributionVectorParameterCurve::GetTangents(int32 SubIndex, int32 KeyIndex, float& ArriveTangent, float& LeaveTangent) const
{
	check( SubIndex >= 0 && SubIndex < 3);
	check( KeyIndex >= 0 && KeyIndex < ConstantCurve.Points.Num() );

	if(SubIndex == 0)
	{
		ArriveTangent = ConstantCurve.Points[KeyIndex].ArriveTangent.X;
		LeaveTangent = ConstantCurve.Points[KeyIndex].LeaveTangent.X;
	}
	else if(SubIndex == 1)
	{
		ArriveTangent = ConstantCurve.Points[KeyIndex].ArriveTangent.Y;
		LeaveTangent = ConstantCurve.Points[KeyIndex].LeaveTangent.Y;
	}
	else if(SubIndex == 2)
	{
		ArriveTangent = ConstantCurve.Points[KeyIndex].ArriveTangent.Z;
		LeaveTangent = ConstantCurve.Points[KeyIndex].LeaveTangent.Z;
	}
}

float UDistributionVectorParameterCurve::EvalSub(int32 SubIndex, float InVal)
{
	check( SubIndex >= 0 && SubIndex < 3);

	FVector OutVal = ConstantCurve.Eval(InVal, FVector::ZeroVector);

	if(SubIndex == 0)
		return OutVal.X;
	else if(SubIndex == 1)
		return OutVal.Y;
	else
		return OutVal.Z;
}

int32 UDistributionVectorParameterCurve::CreateNewKey(float KeyIn)
{	
	FVector NewKeyVal = ConstantCurve.Eval(KeyIn, FVector::ZeroVector);
	int32 NewPointIndex = ConstantCurve.AddPoint(KeyIn, NewKeyVal);
	ConstantCurve.AutoSetTangents(0.f);

	bIsDirty = true;

	return NewPointIndex;
}

void UDistributionVectorParameterCurve::DeleteKey(int32 KeyIndex)
{
	check( KeyIndex >= 0 && KeyIndex < ConstantCurve.Points.Num() );
	ConstantCurve.Points.RemoveAt(KeyIndex);
	ConstantCurve.AutoSetTangents(0.f);

	bIsDirty = true;
}

int32 UDistributionVectorParameterCurve::SetKeyIn(int32 KeyIndex, float NewInVal)
{
	check( KeyIndex >= 0 && KeyIndex < ConstantCurve.Points.Num() );
	int32 NewPointIndex = ConstantCurve.MovePoint(KeyIndex, NewInVal);
	ConstantCurve.AutoSetTangents(0.f);

	bIsDirty = true;

	return NewPointIndex;
}

void UDistributionVectorParameterCurve::SetKeyOut(int32 SubIndex, int32 KeyIndex, float NewOutVal) 
{
	check( SubIndex >= 0 && SubIndex < 3);
	check( KeyIndex >= 0 && KeyIndex < ConstantCurve.Points.Num() );

	if(SubIndex == 0)
		ConstantCurve.Points[KeyIndex].OutVal.X = NewOutVal;
	else if(SubIndex == 1)
		ConstantCurve.Points[KeyIndex].OutVal.Y = NewOutVal;
	else 
		ConstantCurve.Points[KeyIndex].OutVal.Z = NewOutVal;

	ConstantCurve.AutoSetTangents(0.f);

	bIsDirty = true;
}

void UDistributionVectorParameterCurve::SetKeyInterpMode(int32 KeyIndex, EInterpCurveMode NewMode) 
{
	check( KeyIndex >= 0 && KeyIndex < ConstantCurve.Points.Num() );
	
	ConstantCurve.Points[KeyIndex].InterpMode = NewMode;
	ConstantCurve.AutoSetTangents(0.f);

	bIsDirty = true;
}

void UDistributionVectorParameterCurve::SetTangents(int32 SubIndex, int32 KeyIndex, float ArriveTangent, float LeaveTangent)
{
	check( SubIndex >= 0 && SubIndex < 3);
	check( KeyIndex >= 0 && KeyIndex < ConstantCurve.Points.Num() );

	if(SubIndex == 0)
	{
		ConstantCurve.Points[KeyIndex].ArriveTangent.X = ArriveTangent;
		ConstantCurve.Points[KeyIndex].LeaveTangent.X = LeaveTangent;
	}
	else if(SubIndex == 1)
	{
		ConstantCurve.Points[KeyIndex].ArriveTangent.Y = ArriveTangent;
		ConstantCurve.Points[KeyIndex].LeaveTangent.Y = LeaveTangent;
	}
	else if(SubIndex == 2)
	{
		ConstantCurve.Points[KeyIndex].ArriveTangent.Z = ArriveTangent;
		ConstantCurve.Points[KeyIndex].LeaveTangent.Z = LeaveTangent;
	}

	bIsDirty = true;
}

// DistributionVector interface
void UDistributionVectorParameterCurve::GetRange(FVector& OutMin, FVector& OutMax) const
{
	FVector MinVec, MaxVec;
	ConstantCurve.CalcBounds(MinVec, MaxVec, FVector::ZeroVector);

	switch (LockedAxes)
	{
	case EDVLF_XY:
		MinVec.Y = MinVec.X;
		MaxVec.Y = MaxVec.X;
		break;
	case EDVLF_XZ:
		MinVec.Z = MinVec.X;
		MaxVec.Z = MaxVec.X;
		break;
	case EDVLF_YZ:
		MinVec.Z = MinVec.Y;
		MaxVec.Z = MaxVec.Y;
		break;
	case EDVLF_XYZ:
		MinVec.Y = MinVec.X;
		MinVec.Z = MinVec.X;
		MaxVec.Y = MaxVec.X;
		MaxVec.Z = MaxVec.X;
		break;
	case EDVLF_None:
	default:
		break;
	}

	OutMin = MinVec;
	OutMax = MaxVec;
}

bool UDistributionVectorParameterCurve::GetParamValue(UObject* Data, FName ParamName, FVector& OutVector) const
{
	bool bFoundParam = false;

	UParticleSystemComponent* ParticleComp = Cast<UParticleSystemComponent>(Data);
	if(ParticleComp)
	{
		bFoundParam = ParticleComp->GetAnyVectorParameter(ParameterName, OutVector);
	}
	return bFoundParam;
}
