// My First Hack n Slash


#include "AbilityProjectile.h"

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
}

void AAbilityProjectile::BeginPlay()
{
	Super::BeginPlay();

	m_Coll->OnComponentBeginOverlap.AddDynamic(this,&AAbilityProjectile::OnOverlap);
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

	FGameplayEffectContextHandle Context = EffectData->GetEffectContext();
	
	Context.AddHitResult(SweepResult);
	
	EffectData->SetContext(Context);
	
	UnitPawn->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*m_DamageEffectSpecHandle.Data.Get());
}

void AAbilityProjectile::SetEffectSpec(const FGameplayEffectSpecHandle& effect)
{
	m_DamageEffectSpecHandle = effect;
	
}


