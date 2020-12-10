#include "DiabloCheatManager.h"
#include "DungeonManager.h"
#include "AbilitySystem/Components/PlayerDiabloAbilitySystemComp.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Item/Inventory.h"
#include "SaveLoad/SaveLoadManager.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/StartMap/PlayerCreateManager.h"


UDiabloCheatManager::UDiabloCheatManager()
{
	static ConstructorHelpers::FClassFinder<UGameplayEffect> FoundEffect(
      TEXT("Blueprint'/Game/Blueprints/Abilities/GameEffect/GE_BasherStun.GE_BasherStun_C'"));
	m_StunEffect=FoundEffect.Class;
	//Blueprint'/Game/Blueprints/Abilities/GameEffect/GE_Stun.GE_Stun'
}

void UDiabloCheatManager::InitCheatManager()
{
	Super::InitCheatManager();
	
	PRINTF("Cheat Manager Init !");
	
	m_GameManager = GetWorld()->GetGameInstance<UDiabloGameInstance>();
	
	m_PlayerController = Cast<ADiabloPlayerController>(GetWorld()->GetFirstPlayerController());
	
	if(m_PlayerController)
		m_Player = Cast<APlayerDiabloCharacter>(m_PlayerController->GetPawn());
}

void UDiabloCheatManager::PrintPlayerStats()
{
	m_PlayerController->PrintStat();
}

void UDiabloCheatManager::PrintInventoryOld()
{
	m_PlayerController->PrintInven();
}

void UDiabloCheatManager::PrintEquipment()
{
	m_PlayerController->PrintEquipment();
}

void UDiabloCheatManager::SaveSkill()
{
	TWeakObjectPtr<ADiabloPlayerController> DiaPC = ADiabloPlayerController::Get;
	TWeakObjectPtr<APlayerDiabloCharacter> DiaPl = DiaPC->GetPlayerPawn();
	UPlayerDiabloAbilitySystemComp* Gas = Cast<UPlayerDiabloAbilitySystemComp
	>(DiaPl.Get()->GetAbilitySystemComponent());
	USaveLoadManager::Get->SaveSkill(UPlayerCreateManager::Get->m_CurrentSelectSlot,Gas->m_nSkillPoints,Gas->m_nTotalSkillPointSpents,
		Gas->m_AryBaseSkill,
		Gas->m_AryPowerSkill,
		Gas->m_AryDefensvieSkill,
		Gas->m_AryPowerSkill,
		Gas->m_AryMasterySkill,
		Gas->m_AryUltimateSkill);
}

void UDiabloCheatManager::LoadSkill()
{
	USaveLoadManager::Get->LoadSkill(UPlayerCreateManager::Get->m_CurrentSelectSlot);
	USaveLoadManager::Get->SetLoadedSkillDataToPlayer(UPlayerCreateManager::Get->m_CurrentSelectSlot);
}

void UDiabloCheatManager::SaveInven()
{
	TWeakObjectPtr<ADiabloPlayerController> DiaPC = ADiabloPlayerController::Get;
	
	USaveLoadManager::Get->SaveInventory(UPlayerCreateManager::Get->m_CurrentSelectSlot,DiaPC->GetInven()->GetItemAry());
}

void UDiabloCheatManager::LoadInven()
{
	USaveLoadManager::Get->LoadInventory(UPlayerCreateManager::Get->m_CurrentSelectSlot);
	USaveLoadManager::Get->SetLoadedInvenDataToPlayer(UPlayerCreateManager::Get->m_CurrentSelectSlot);
}

void UDiabloCheatManager::SaveEquip()
{
	TArray<FItemInstance> AryEquip;

	for(FEquipSlot* Slot : ADiabloPlayerController::Get->GetEquipment()->GetArySlotPtr())
	{
		AryEquip.Emplace(Slot->m_Item);
	}
	
	USaveLoadManager::Get->SaveEquipment(UPlayerCreateManager::Get->m_CurrentSelectSlot,AryEquip);
	
}

void UDiabloCheatManager::LoadEquip()
{
	USaveLoadManager::Get->LoadEquipment(UPlayerCreateManager::Get->m_CurrentSelectSlot);
	USaveLoadManager::Get->SetLoadedEquipDataToPlayer(UPlayerCreateManager::Get->m_CurrentSelectSlot);
}

void UDiabloCheatManager::SaveCharStat()
{
	TWeakObjectPtr<ADiabloPlayerController> DiaPC = ADiabloPlayerController::Get;
	TWeakObjectPtr<APlayerDiabloCharacter> DiaPl = DiaPC->GetPlayerPawn();
	FText Name = DiaPl->m_TextUnitName;
	int Lev= DiaPl->GetCharacterLevel();
	int Hair= DiaPl->m_HairIndex;
	int Face= DiaPl->m_FaceIndex;
	
	USaveLoadManager::Get->SaveCharacterStat(UPlayerCreateManager::Get->m_CurrentSelectSlot,Lev,Name,Face,Hair,USaveLoadManager::Get->GetCurrentPlayerClassName(),DiaPl->m_fCurrentExp);
}

void UDiabloCheatManager::LoadCharStat()
{
	USaveLoadManager::Get->LoadCharStat(UPlayerCreateManager::Get->m_CurrentSelectSlot);
	USaveLoadManager::Get->SetLoadedCharDataToPlayer(UPlayerCreateManager::Get->m_CurrentSelectSlot);
}

void UDiabloCheatManager::DeleteAllSlot()
{
	USaveLoadManager::Get->DeleteAllSlot();
}

void UDiabloCheatManager::RemoveAllEffect()
{
	TWeakObjectPtr<ADiabloPlayerController> DiaPC = ADiabloPlayerController::Get;
	TWeakObjectPtr<APlayerDiabloCharacter> DiaPl = DiaPC->GetPlayerPawn();
	DiaPl->RemoveAllEffect();
}

void UDiabloCheatManager::SetPlayerLevel(int levelWant)
{
	TWeakObjectPtr<ADiabloPlayerController> DiaPC = ADiabloPlayerController::Get;
	TWeakObjectPtr<APlayerDiabloCharacter> DiaPl = DiaPC->GetPlayerPawn();
	DiaPl->SetCharacterLevel(levelWant);
}

void UDiabloCheatManager::AddPlayerExp(float wantV)
{
	TWeakObjectPtr<ADiabloPlayerController> DiaPC = ADiabloPlayerController::Get;
	TWeakObjectPtr<APlayerDiabloCharacter> DiaPl = DiaPC->GetPlayerPawn();
	DiaPl->EarnExp(wantV);
}

void UDiabloCheatManager::CreateDungeon(int stageLevel)
{
	UDiabloGameInstance::Get->GetDungeonManager()->CreateDefaultInfinityDungeon(stageLevel);
}

void UDiabloCheatManager::PortalToDungeon()
{
	UDiabloGameInstance::Get->GetDungeonManager()->PortalToRecentDungeon();
}

void UDiabloCheatManager::PortalToVillage()
{
	UDiabloGameInstance::Get->GetDungeonManager()->PortalToVillage();
}

void UDiabloCheatManager::DamageToPlayer(float wantV)
{
	TWeakObjectPtr<ADiabloPlayerController> DiaPC = ADiabloPlayerController::Get;
	TWeakObjectPtr<APlayerDiabloCharacter> DiaPl = DiaPC->GetPlayerPawn();
	
	auto DamageEffectSpecHandle =DiaPl->GetDiaAbilitySystem()->MakeEffectContext();
	UGameplayEffect* GEBounty = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("SelfDmg")));
	GEBounty->DurationPolicy = EGameplayEffectDurationType::Instant;
	
	int32 Idx = GEBounty->Modifiers.Num();
	GEBounty->Modifiers.SetNum(Idx + 1);
	FGameplayModifierInfo& InfoXP = GEBounty->Modifiers[Idx];
	InfoXP.ModifierMagnitude = FScalableFloat(wantV);
	InfoXP.ModifierOp = EGameplayModOp::Additive;
	InfoXP.Attribute = UBaseDiabloAttribute::GetTookPhysDamageAttribute();
	

	DiaPl->GetDiaAbilitySystem()->ApplyGameplayEffectToSelf(GEBounty, 1.0f, DamageEffectSpecHandle);
}

void UDiabloCheatManager::KillPlayer()
{
	TWeakObjectPtr<ADiabloPlayerController> DiaPC = ADiabloPlayerController::Get;
	TWeakObjectPtr<APlayerDiabloCharacter> DiaPl = DiaPC->GetPlayerPawn();
	DiaPl->Die();
}

void UDiabloCheatManager::StunPlayer(float duration)
{
	PRINTF("Apply Stun");
	TWeakObjectPtr<ADiabloPlayerController> DiaPC = ADiabloPlayerController::Get;
	TWeakObjectPtr<APlayerDiabloCharacter> DiaPl = DiaPC->GetPlayerPawn();

	auto ASD=DiaPl->GetDiaAbilitySystem()->MakeOutgoingSpec(m_StunEffect,1,DiaPl->GetDiaAbilitySystem()->MakeEffectContext());

	ASD.Data->Duration=duration;//효과없음
	
	DiaPl->GetDiaAbilitySystem()->ApplyGameplayEffectSpecToSelf(*ASD.Data);
}

