// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Characters/UnitPawn.h"
#include "Components/WidgetComponent.h"
#include "DamageTextWidgetComponent.generated.h"

/**
 * 
 */

UENUM(Blueprintable)
enum class EDamagePopup: uint8
{
    NormalLeft,
    NormalRight,
    CritcalLeft,
    CritcalRight,
    PlayerHurt,
    Miss,
    Length
};
UCLASS()
class DIABLOM_API UDamageTextWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
   protected:
    UPROPERTY()
    const AActor* m_Base;
    UPROPERTY()
    const AActor* m_AttachedActor;
public:
    void Init(AActor* base);
    void AttachToActor(AUnitPawn* newRoot);
    
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetDamageText(const FText& Damage);
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void StartAnimation(EDamagePopup dmg);

    UFUNCTION(BlueprintCallable)
    void EndAnimation();

    FORCEINLINE const AActor* GetAttachedActor() const
    {
        return m_AttachedActor;
    }
};
