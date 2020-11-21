
#include "DiaAniminstance.h"
#include "Characters/UnitPawn.h"


void FDiaAnimInstanceProxy::InitializeObjects(UAnimInstance* InAnimInstance)
{
    Super::InitializeObjects(InAnimInstance);
    
    m_DiaAnim = Cast<UDiaAniminstance>(InAnimInstance);
}

void FDiaAnimInstanceProxy::Update(float DeltaSeconds)
{
    m_DiaAnim->UpdateVelocity();
}

void UDiaAniminstance::NativeBeginPlay()
{
    Super::NativeBeginPlay();
    
    m_Owner=Cast<AUnitPawn>( TryGetPawnOwner());
    m_UnitPawnVelocityFromMoveComp=m_Owner->GetVelocityPtr();
}

void UDiaAniminstance::UpdateVelocity()
{
    m_fVelocity=GetVeloPercentOne();
}

float UDiaAniminstance::GetVelocitySqr() const
{
    return m_UnitPawnVelocityFromMoveComp->SizeSquared();
}

float UDiaAniminstance::GetVeloPercentOne() const
{
    float MaxSpeedSqr = FMath::Square( m_Owner->GetMoveSpeed());
    
    return GetVelocitySqr() / MaxSpeedSqr;
}
