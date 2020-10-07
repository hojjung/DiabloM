// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	static const FTextFormat FormatT;
	
	static const  FText UnitSymbol[9];

	
	UFUNCTION(BlueprintCallable,Category="DiaLib")
	static float SetFloatPrecision(float TheFloat, int32 Precision)
	{
		if(Precision<=0)
		{
			return roundf(TheFloat);
		}
		
		Precision = FMath::Clamp(Precision,1,10);
		
		int32 PresRounded =round(FMath::Pow(10,Precision));

		float A= round( TheFloat*PresRounded);
		float B=PresRounded;
		
		return A / B;
	}
	UFUNCTION(BlueprintCallable, Category = "DiaLib")
	static FVector2D GetWidgetCenterLocation(FGeometry parentGeo,UWidget * Widget)
	{
		FGeometry Geometry = parentGeo;
		FVector2D Position = Geometry.AbsoluteToLocal(Widget->GetCachedGeometry().GetAbsolutePosition()) + Widget->GetCachedGeometry().GetLocalSize() / 2.0f;
		return Position;
	}

	UFUNCTION(BlueprintCallable,Category="Material")
	static UMaterialInstanceDynamic* CreateSetDynamicMaterial(UMeshComponent* meshComp, int matIndex)
	{
		auto* Mat = meshComp->GetMaterial(matIndex);
		auto* MatInstanceDynamic= UMaterialInstanceDynamic::Create(Mat, meshComp);
		meshComp->SetMaterial(matIndex, MatInstanceDynamic);

		return MatInstanceDynamic;
	}

	UFUNCTION(BlueprintCallable, Category = "Audio")
    static void SetAudioPlay(UAudioComponent* audioComp, float pitch = 1.f , float volume = 1.f,USoundBase* soundBase = nullptr)
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
		
		float Cache = vWant;
		float Thous = 1000;
		int Count=0;

		while (Cache> Thous)//Mile
			{
			Cache /= Thous;
			Count++;
			}

		Cache=SetFloatPrecision(Cache,1);
		FFormatOrderedArguments Args;
		Args.Add(Cache);
		Args.Add(UnitSymbol[Count]);
		
		return FText::Format(FormatT, Args);
	}
};

