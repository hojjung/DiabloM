// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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
	UFUNCTION(BlueprintCallable,Category="DiaLib")
	static FText GetFloatAsStringWithPrecision(float TheFloat, int32 Precision, bool IncludeLeadingZero = true)
	{
		float Rounded = roundf(TheFloat);
		if (FMath::Abs(TheFloat - Rounded) < FMath::Pow(10, -1 * Precision))
		{
			TheFloat = Rounded;
		}
		FNumberFormattingOptions NumberFormat;					//Text.h
		NumberFormat.MinimumIntegralDigits = (IncludeLeadingZero) ? 1 : 0;
		NumberFormat.MaximumIntegralDigits = 10000;
		NumberFormat.MinimumFractionalDigits = Precision;
		NumberFormat.MaximumFractionalDigits = Precision;
		return FText::AsNumber(TheFloat, &NumberFormat);
	}
	UFUNCTION(BlueprintCallable, Category = "DiaLib")
	static FVector2D GetWidgetCenterLocation(FGeometry parentGeo,UWidget * Widget)
	{
		FGeometry Geometry = parentGeo;
		FVector2D Position = Geometry.AbsoluteToLocal(Widget->GetCachedGeometry().GetAbsolutePosition()) + Widget->GetCachedGeometry().GetLocalSize() / 2.0f;
		return Position;
	}
};
