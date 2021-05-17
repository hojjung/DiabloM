// My First Hack n Slash


#include "OptionPanel.h"

#include "GameFramework/GameUserSettings.h"
#include "Managers/DiabloGameInstance.h"

UOptionPanel::UOptionPanel(const FObjectInitializer& obj):Super(obj)
{
	//SoundClass'/Engine/EngineSounds/Music.Music'
	//SoundClass'/Engine/EngineSounds/SFX.SFX'
	//SoundMix'/Game/Sound/SM_Music.SM_Music'
	//SoundMix'/Game/Sound/SM_SFX.SM_SFX'
	static ConstructorHelpers::FObjectFinder<USoundClass> Found01(
               TEXT("SoundClass'/Engine/EngineSounds/Music.Music'"));
	static ConstructorHelpers::FObjectFinder<USoundClass> Found02(
			TEXT("SoundClass'/Engine/EngineSounds/SFX.SFX'"));
	static ConstructorHelpers::FObjectFinder<USoundMix> Found03(
			TEXT("SoundMix'/Game/Sound/SM_Music.SM_Music'"));
	static ConstructorHelpers::FObjectFinder<USoundMix> Found04(
			TEXT("SoundMix'/Game/Sound/SM_SFX.SM_SFX'"));
	
    m_SoundClassBGM = Found01.Object;
	m_SoundClassSFX = Found02.Object;
	m_SoundMixBGM = Found03.Object;
	m_SoundMixSFX = Found04.Object;
}

void UOptionPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UKismetSystemLibrary::ControlScreensaver(false);
	
	UpdateVersionNameText();
	
	m_ToggleFrameLimit->SetCheckedState(ECheckBoxState::Checked);
	
	m_ToggleFrameLimit->OnCheckStateChanged.AddDynamic(this,&UOptionPanel::OnToggleFrameLimit);
	
	m_SoundControlBGM->OnValueChanged.AddDynamic(this,&UOptionPanel::OnBGMSliderChanged);

	m_SoundControlSFX->OnValueChanged.AddDynamic(this,&UOptionPanel::OnSFXSliderChanged);

	m_SoundControlBGM->SetValue(1);

	m_SoundControlSFX->SetValue(1);
}

void UOptionPanel::OnToggleFrameLimit(bool v)
{
	if(v)
	{
		UGameUserSettings::GetGameUserSettings()->SetFrameRateLimit(33.f);
	}
	else
	{
		UGameUserSettings::GetGameUserSettings()->SetFrameRateLimit(-1);
	}

	UGameUserSettings::GetGameUserSettings()->ApplyNonResolutionSettings();
}

void UOptionPanel::UpdateVersionNameText()
{
	m_TextVersionName->SetText(FText::FromString(FString::Printf(TEXT("버전정보:%s"),*UDiabloGameInstance::Get->m_PlayfabManager->m_CurrentVersionName)));
}

void UOptionPanel::OnBGMSliderChanged(float v)
{
	UGameplayStatics::SetSoundMixClassOverride(this,m_SoundMixBGM,m_SoundClassBGM,v);
	UGameplayStatics::PushSoundMixModifier(this,m_SoundMixBGM);
}

void UOptionPanel::OnSFXSliderChanged(float v)
{
	UGameplayStatics::SetSoundMixClassOverride(this,m_SoundMixSFX,m_SoundClassSFX,v);
	UGameplayStatics::PushSoundMixModifier(this,m_SoundMixSFX);
}
