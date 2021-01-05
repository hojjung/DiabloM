// My First Hack n Slash


#include "AbilityProjectile.h"

#include "GameplayCueManager.h"
#include "AbilitySystemGlobals.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"

// Sets default values
AAbilityProjectile::AAbilityProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	
	m_Coll = CreateDefaultSubobject<USphereComponent>("Coll00");
	m_Coll->SetCollisionProfileName(UCollisionProfile::DefaultProjectile_ProfileName);
	m_Coll->SetSphereRadius(32.f);
	m_Coll->SetCanEverAffectNavigation(false);
	m_Coll->CanCharacterStepUpOn=ECB_No;
	m_Coll->bDynamicObstacle = false;
	RootComponent = m_Coll;

	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh01");
	m_Mesh->CastShadow=false;
	m_Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_Mesh->SetupAttachment(m_Coll);

	m_Particle = CreateDefaultSubobject<UParticleSystemComponent>("Particle02");
	m_Particle->SetupAttachment(m_Mesh);
	
	m_ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("PrjMove03");
	m_ProjectileMovement->SetUpdatedComponent(RootComponent);
	m_ProjectileMovement->InitialSpeed = 1000.f;
	m_ProjectileMovement->ProjectileGravityScale = 0.f;
	m_ProjectileMovement->bInitialVelocityInLocalSpace=true;

	m_Range=2200.f;
}


void AAbilityProjectile::BeginPlay()
{
	Super::BeginPlay();
	m_Coll->OnComponentBeginOverlap.AddDynamic(this,&AAbilityProjectile::OnOverlap);
	ShootEnd();

}

void AAbilityProjectile::SetProjectileVelocity()
{
	FVector NewVelocity=FVector(1.f,0.f,0.f);
	
	NewVelocity = NewVelocity.GetSafeNormal() * m_ProjectileMovement->InitialSpeed;

	m_ProjectileMovement->SetVelocityInLocalSpace(NewVelocity);
}

void AAbilityProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AUnitPawn* UnitPawn = Cast<AUnitPawn>(OtherActor);

	if(!UnitPawn)
	{
		return;
	}

	if(m_TargettTeamId != UnitPawn->GetTeamID())
	{
		return;
	}

	FGameplayEffectSpec* EffectData = m_DamageEffectSpecHandle.Data.Get();

	if(!EffectData)
	{
		ShootEnd();
		return;
	}

	FGameplayEffectContextHandle Context = EffectData->GetEffectContext();

	EffectData->SetContext(Context);
	
	UnitPawn->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*m_DamageEffectSpecHandle.Data.Get());

	ShootEnd();
}

float AAbilityProjectile::GetLifeTime() const
{
	return m_Range / m_ProjectileMovement->InitialSpeed;
}

void AAbilityProjectile::ShootStart(const FGameplayEffectSpecHandle& effect)
{
	m_EndHandle.Invalidate();
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	m_Mesh->SetComponentTickEnabled(true);
	m_Coll->SetComponentTickEnabled(true);
	//
	 
	SetProjectileVelocity();
	m_ProjectileMovement->SetComponentTickEnabled(true);
	//
	m_Particle->Activate();
	m_Particle->SetComponentTickEnabled(true);

	m_DamageEffectSpecHandle = effect;

	ShootEndSpen(GetLifeTime());

}

void AAbilityProjectile::ShootEnd()
{
	m_EndHandle.Invalidate();
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	m_Mesh->SetComponentTickEnabled(false);
	m_Coll->SetComponentTickEnabled(false);
	m_ProjectileMovement->SetComponentTickEnabled(false);
	m_Particle->Deactivate();
	m_Particle->SetComponentTickEnabled(false);
}

void AAbilityProjectile::ShootEndSpen(float timer)
{
	if ((GetLocalRole() == ROLE_Authority || GetTearOff()) && !IsPendingKill())
	{
		if( timer > 0.0f)
		{
			GetWorldTimerManager().SetTimer( m_EndHandle, this, &AAbilityProjectile::ShootEnd, timer );
		}
		else
		{
			GetWorldTimerManager().ClearTimer( m_EndHandle );		
		}
	}
}


