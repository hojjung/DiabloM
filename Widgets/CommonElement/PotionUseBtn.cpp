// My First Hack n Slash


#include "PotionUseBtn.h"


#include "CooldownProgress.h"
#include "AbilitySystem/Components/PlayerDiabloAbilitySystemComp.h"

void UPotionUseBtn::Init(APlayerDiabloCharacter* playerPawn, UPlayerDiabloAbilitySystemComp* diaComp)
{
	m_Player = playerPawn;

	m_PlayerDiaComp = diaComp;

	m_UseBtn->OnClicked.AddDynamic(this,&UPotionUseBtn::UsePotion);

	ClearCooldown();
}

void UPotionUseBtn::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(!m_GaSpec||m_fMaxCD<=0.f)
	{
		return;
	}

	float CD = m_GaSpec->Ability->GetCooldownTimeRemaining(m_PlayerDiaComp->AbilityActorInfo.Get());
	
	if(CD <= 0.f)
	{
		ClearCooldown();
		
		return;
	}
	
	m_SkillCooldown->SetCooldownProgress(CD,m_fMaxCD);//0이 끝임
}

void UPotionUseBtn::ClearCooldown()
{
	m_fMaxCD=0.f;
	m_SkillCooldown->SetCooldownProgress(0.f,0.f);
}

void UPotionUseBtn::UsePotion()
{
	FGameplayAbilitySpec* AbilSpec = m_Player->DrinkPotion();

	if(!AbilSpec)
	{
		return;
	}

	m_GaSpec = AbilSpec;
	
	m_fMaxCD = m_GaSpec->Ability->GetCooldownTimeRemaining(m_PlayerDiaComp->AbilityActorInfo.Get());

	if(m_fMaxCD>0.f)
	{
		m_SkillCooldown->StartCooldown();
	}
}
