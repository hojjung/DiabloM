#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"

#include "SkillIndicator.generated.h"

class UDecalComponent;

UCLASS()
class DIABLOM_API ASkillIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	ASkillIndicator();

	static ASkillIndicator* GetCurrent;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Item")
	USphereComponent* m_CollSphere;
	UPROPERTY(VisibleAnywhere, Category = "Item")
	UDecalComponent* m_Indicator;

protected:
	float m_fRadius;

public:
	void SetRadiusScale(float radiusWant);
};
