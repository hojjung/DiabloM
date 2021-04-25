// My First Hack n Slash


#include "OtherPlayerPawn.h"

void AOtherPlayerPawn::SetPVPPlayerPawn(UPlayFabJsonObject* statObj, UPlayFabJsonObject* equipObj)
{

	UPlayFabJsonObject* StatObj = statObj->GetObjectField(TEXT("Stat"));

	int BaseAttack = StatObj->GetNumberField(TEXT("BaseAttack"));

	int Critical = StatObj->GetNumberField(TEXT("Critical"));

	int CriticalDmg = StatObj->GetNumberField(TEXT("CriticalDmg"));

	int SuperCritical = StatObj->GetNumberField(TEXT("SuperCritical"));

	int SuperCriticalDmg = StatObj->GetNumberField(TEXT("SuperCriticalDmg"));

	int MagicBomb = StatObj->GetNumberField(TEXT("MagicBomb"));

	int MagicBombDmg = StatObj->GetNumberField(TEXT("MagicBombDmg"));

	int SuperMagicBomb = StatObj->GetNumberField(TEXT("SuperMagicBomb"));

	int SuperMagicBombDmg = StatObj->GetNumberField(TEXT("SuperMagicBombDmg"));

	UPlayFabJsonObject* EquippedObj = equipObj->GetObjectField(TEXT("CurrentEquipped"));

	int SkinIndex = EquippedObj->GetNumberField(TEXT("EquippedSkin"));

	int WeaponIndex = EquippedObj->GetNumberField(TEXT("EquippedWeapon"));

	int WeaponLevel = EquippedObj->GetNumberField(TEXT("EquippedWeaponLevel"));

	int PetIndex = EquippedObj->GetNumberField(TEXT("EquippedPet"));

	int EquippedPetLevel = EquippedObj->GetNumberField(TEXT("EquippedPetLevel"));

	int EquippedSkill01 = EquippedObj->GetNumberField(TEXT("EquippedSkill01"));
	
	int EquippedSkill01Level = EquippedObj->GetNumberField(TEXT("EquippedSkill01Level"));
	
	int EquippedSkill02 = EquippedObj->GetNumberField(TEXT("EquippedSkill02"));
	
	int EquippedSkill02Level = EquippedObj->GetNumberField(TEXT("EquippedSkill02Level"));
	
	int EquippedSkill03 = EquippedObj->GetNumberField(TEXT("EquippedSkill03"));
	
	int EquippedSkill03Level = EquippedObj->GetNumberField(TEXT("EquippedSkill03Level"));
	
	int EquippedSkill04 = EquippedObj->GetNumberField(TEXT("EquippedSkill04"));
	
	int EquippedSkill04Level = EquippedObj->GetNumberField(TEXT("EquippedSkill04Level"));
}
