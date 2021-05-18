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
	
	m_ToggleFrameLimit->OnCheckStateChanged.AddDynamic(this,&UOptionPanel::OnToggleFrameLimit);
	
	m_SoundControlBGM->OnValueChanged.AddDynamic(this,&UOptionPanel::OnBGMSliderChanged);

	m_SoundControlSFX->OnValueChanged.AddDynamic(this,&UOptionPanel::OnSFXSliderChanged);


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

void UOptionPanel::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	if(InVisibility==ESlateVisibility::Collapsed || InVisibility==ESlateVisibility::Hidden)
	{
		//hide
		if(UDiabloGameInstance::Get->m_LoadedOptionSave)
		{
			bool Limit =  m_ToggleFrameLimit->IsChecked();

			float BGMV = m_SoundControlBGM->GetValue();

			float SFXV = m_SoundControlSFX->GetValue();
			
			UDiabloGameInstance::Get->m_LoadedOptionSave->m_bOptionFPSLimit = Limit;
			
			UDiabloGameInstance::Get->m_LoadedOptionSave->m_fOptionVolumeBGM = BGMV;
			
			UDiabloGameInstance::Get->m_LoadedOptionSave->m_fOptionVolumeSFX = SFXV;
		}
		
		UDiabloGameInstance::Get->UnloadSaveOptionSaveData();
	}
	else
	{
		UDiabloGameInstance::Get->LoadOptionSaveData();
		
		if(UDiabloGameInstance::Get->m_LoadedOptionSave)
		{
			UOptionSave* OptionSave = UDiabloGameInstance::Get->m_LoadedOptionSave;

			if(OptionSave->m_bOptionFPSLimit)
			{
				m_ToggleFrameLimit->SetCheckedState(ECheckBoxState::Checked);	
			}
			else
			{
				m_ToggleFrameLimit->SetCheckedState(ECheckBoxState::Unchecked);
			}
		
			m_SoundControlBGM->SetValue(OptionSave->m_fOptionVolumeBGM);

			m_SoundControlSFX->SetValue(OptionSave->m_fOptionVolumeSFX);
		}
	}
}
