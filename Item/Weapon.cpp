#include "Weapon.h"
#include "Datas/ItemDataTable.h"
#include "Managers/DiabloGameInstance.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
// Sets default values
AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	
	m_Coll = CreateDefaultSubobject<UCapsuleComponent>("Capsule00");
	m_Coll->SetupAttachment(RootComponent);
	m_Coll->SetRelativeLocation(FVector(0.f,0.f,50.f));
	m_Coll->SetCollisionProfileName("OverlapOnlyPawn");
	m_Coll->SetCapsuleSize(22.f,64.f);
	m_Coll->SetGenerateOverlapEvents(true);
	m_Coll->SetHiddenInGame(true);
	m_Coll->SetCastShadow(false);
	m_Coll->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkMesh00");
	m_WeaponMesh->SetCollisionProfileName("NoCollision");
	m_WeaponMesh->SetupAttachment(RootComponent);
	m_WeaponMesh->SetCastShadow(false);

	m_User = nullptr;
	m_bIsAttacking = false;
	m_AlreadyHittenForIgnore.Reserve(10);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	m_Coll->IgnoreActorWhenMoving(this,true);
	
	OnActorBeginOverlap.AddDynamic(this,&AWeapon::WeaponCollBegin);
}

void AWeapon::SetWeapon(FName weaponId, APlayerDiabloCharacter * usingPlayer)
{
	m_Id = weaponId;
	m_User = usingPlayer;
	m_Coll->IgnoreActorWhenMoving(usingPlayer, true);
}

void AWeapon::WeaponAttackBegin(FGameplayTag attackEventTag)
{
	
	m_CachedAttackEvent = attackEventTag;
	m_bIsAttacking = true;
	m_Coll->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeapon::WeaponAttackEnd()
{
	m_bIsAttacking = false;
	m_Coll->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_AlreadyHittenForIgnore.Reset();
}

void AWeapon::WeaponCollBegin(AActor* overlappedActor, AActor * otherActor)
{
	if (m_AlreadyHittenForIgnore.Contains(otherActor))
	{
		return;
	}
	m_AlreadyHittenForIgnore.Add(otherActor);

	FGameplayEventData EventData;
	EventData.Instigator = m_User;
	EventData.Target = otherActor;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(m_User,m_CachedAttackEvent,EventData);
}


