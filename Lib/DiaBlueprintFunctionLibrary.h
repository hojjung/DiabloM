// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BigInt.h"
#include "DiabloM.h"
#include "Widget.h"
#include "DiaBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UDiaBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static const int UnitSymbolMax = 26;

	static const FText UnitSymbol[UnitSymbolMax];

	static FString GetAlphabetTextBigInt(const BigInt& vWant, int symbolCount = 0)
	{
		if(vWant==0)
		{
			return FString::FromInt(0);
		}
		BigInt Mile = 10000;

		BigInt Cache = vWant;

		BigInt Remain;

		TArray<BigInt> NumList;

		int Count = 0;

		while (Cache >= 1) //Mile
		{
			Cache.DivideWithRemainder(Mile, Remain);

			NumList.Emplace(Remain.ToInt());

			Count++;
		}

		FString RetStr;

		int MaxIter = Count - 1;

		int SymbolCounter = 0;

		for (int i = MaxIter; i >= 0; i--)
		{
			if(NumList[i]!=0)
			{
				RetStr.Append(FString::FromInt(NumList[i].ToInt()));
			}

			if (i < UnitSymbolMax) //자릿수초과시 그냥 합처서
			{
				RetStr.Append(UnitSymbol[i].ToString());

				SymbolCounter++;

				if (symbolCount > 0 && SymbolCounter >= symbolCount)
				{
					break;
				}
			}
		}

		return RetStr;
	}

	static const FTextFormat FormatT;

	UFUNCTION(BlueprintCallable, Category="DiaLib")
	static float SetFloatPrecision(float TheFloat, int32 Precision)
	{
		if (Precision <= 0)
		{
			return roundf(TheFloat);
		}

		Precision = FMath::Clamp(Precision, 1, 10);

		int32 PresRounded = round(FMath::Pow(10, Precision));

		float A = round(TheFloat * PresRounded);
		float B = PresRounded;

		return A / B;
	}

	UFUNCTION(BlueprintCallable, Category = "DiaLib")
	static FVector2D GetWidgetCenterLocation(FGeometry parentGeo, UWidget* Widget)
	{
		FGeometry Geometry = parentGeo;
		FVector2D Position = Geometry.AbsoluteToLocal(Widget->GetCachedGeometry().GetAbsolutePosition()) + Widget->
			GetCachedGeometry().GetLocalSize() / 2.0f;
		return Position;
	}

	UFUNCTION(BlueprintCallable, Category="Material")
	static UMaterialInstanceDynamic* CreateSetDynamicMaterial(UMeshComponent* meshComp, int matIndex)
	{
		auto* Mat = meshComp->GetMaterial(matIndex);
		auto* MatInstanceDynamic = UMaterialInstanceDynamic::Create(Mat, meshComp);
		meshComp->SetMaterial(matIndex, MatInstanceDynamic);

		return MatInstanceDynamic;
	}

	UFUNCTION(BlueprintCallable, Category = "Audio")
	static void SetAudioPlay(UAudioComponent* audioComp, float pitch = 1.f, float volume = 1.f,
	                         USoundBase* soundBase = nullptr)
	{
		if (soundBase)
		{
			audioComp->SetSound(soundBase);
		}
		audioComp->SetPitchMultiplier(pitch);
		audioComp->SetVolumeMultiplier(volume);
		audioComp->Play();
	}

	UFUNCTION(BlueprintCallable, Category = "Text")
	static FText GetAlphabetText(float vWant)
	{
		float Cache = roundf(vWant);

		float Thous = 1000;

		int Count = 0;

		while (Cache > Thous) //Mile
		{
			Cache /= Thous;
			Count++;
		}

		Cache = SetFloatPrecision(Cache, 1);

		FFormatOrderedArguments Args;

		Args.Add(Cache);

		Args.Add(UnitSymbol[Count]);

		return FText::Format(FormatT, Args);
	}


	static BigInt MultiplePercent(BigInt a, BigInt b, int aPrecisionCount = 1,int bPrecisionCount = 1)
	{
		a.Multiply(b);

		int PrecisionFactor = FMath::Pow(10, aPrecisionCount + bPrecisionCount);

		a.Divide(PrecisionFactor);

		return a;
	}
};
