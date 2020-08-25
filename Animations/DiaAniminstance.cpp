// Fill out your copyright notice in the Description page of Project Settings.


#include "DiaAniminstance.h"
#include "Characters/UnitPawn.h"


float UDiaAniminstance::GetVelocitySqr() const
{
    return Cast<AUnitPawn>( TryGetPawnOwner())->GetVelocity().SizeSquared();
}

float UDiaAniminstance::GetVeloPercentOne() const
{
    AUnitPawn* owner=Cast<AUnitPawn>( TryGetPawnOwner());

    float MaxSpeedSqr = FMath::Square( owner->GetMoveSpeed());
    
    return GetVelocitySqr() / MaxSpeedSqr;
}
