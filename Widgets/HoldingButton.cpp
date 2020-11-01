// My First Hack n Slash


#include "HoldingButton.h"

#include "Managers/DiabloGameMode.h"

UHoldingButton::UHoldingButton(): m_fDeltaTimer(0), m_fHoldingTime(0)
{
    OnPressed.AddDynamic(this,&UHoldingButton::OnPressHold);

    OnReleased.AddDynamic(this,&UHoldingButton::OnReleaseHold);

    UseHoldEvent(m_fHoldingTime);
}

void UHoldingButton::UseHoldEvent(float holdTime)
{
    m_fDeltaTimer=0.f;
    
    m_fHoldingTime=holdTime;

    PRINTF("HoldeTimeSet:%f",m_fHoldingTime);
}

void UHoldingButton::OnPressHold()
{
    m_fDeltaTimer=0.f;
    
    m_TickDeleHandle=ADiabloGameMode::Get->GetOnDeltaTick().AddUObject(this,&UHoldingButton::CustomTick);
}

void UHoldingButton::OnReleaseHold()
{
    if(m_fDeltaTimer>m_fHoldingTime)
    {
        OnHoldTimeDone.Broadcast();
    }

    m_fDeltaTimer=0.f;

    ADiabloGameMode::Get->GetOnDeltaTick().Remove(m_TickDeleHandle);
}

void UHoldingButton::CustomTick(float deltatime)
{
    m_fDeltaTimer+=deltatime;

    
}

