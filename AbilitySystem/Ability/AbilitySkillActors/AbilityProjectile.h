// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "GameplayEffectTypes.h"
#include "Characters/UnitPawn.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "AbilityProjectile.generated.h"

UCLASS()
class DIABLOM_API AAbilityProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AAbilityProjectile();
	
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    UProjectileMovementComponent* m_ProjectileMovement;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UParticleSystemComponent* m_Particle;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UStaticMeshComponent* m_Mesh;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USphereComponent* m_Coll;

protected:	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true))
	float m_Range;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true))
	ETeamID m_TargettTeamId;
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle m_DamageEffectSpecHandle;

public:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	float GetLifeTime() const
	{
		return m_Range / m_ProjectileMovement->InitialSpeed;
	}

	void SetEffectSpec(const FGameplayEffectSpecHandle& effect);
};
