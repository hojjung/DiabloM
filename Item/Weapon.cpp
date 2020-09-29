#include "Weapon.h"
#include "Datas/ItemDataTable.h"
#include "Managers/DiabloGameInstance.h"
#include "Characters/PlayerDiabloCharacter.h"

// Sets default values
AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	m_RootSphere=CreateDefaultSubobject<USphereComponent>("RootSphere");
	RootComponent=m_RootSphere;
	m_RootSphere->SetCollisionProfileName("NoCollision");
	
	m_SkMeshWeapon = CreateDefaultSubobject<USkeletalMeshComponent>("SkMesh00");
	m_SkMeshWeapon->SetCollisionProfileName("NoCollision");
	m_SkMeshWeapon->SetupAttachment(RootComponent);
	m_SkMeshWeapon->SetCastShadow(false);

	m_StMeshWeapon = CreateDefaultSubobject<UStaticMeshComponent>("StMesh00");
	m_StMeshWeapon->SetCollisionProfileName("NoCollision");
	m_StMeshWeapon->SetupAttachment(RootComponent);
	m_StMeshWeapon->SetCastShadow(false);

	m_User = nullptr;
	m_bIsAttacking = false;
}


void AWeapon::InitWeapon(APlayerDiabloCharacter* pl, const FItemInstance* itemInst)
{
	m_User = pl;

	m_ItemInstPtr=itemInst;

	if(m_ItemInstPtr->m_ItemData->m_ItemMesh)
	{
		m_StMeshWeapon->SetStaticMesh(m_ItemInstPtr->m_ItemData->m_StEquipment);
	}
	else if(m_ItemInstPtr->m_ItemData->m_ItemMesh)
	{
		m_SkMeshWeapon->SetSkeletalMesh(m_ItemInstPtr->m_ItemData->m_SkEquipment);
	}

	//addition scale

	//pivot, rotation setting
};