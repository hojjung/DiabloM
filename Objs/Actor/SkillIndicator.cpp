#include "SkillIndicator.h"
#include "Components/DecalComponent.h"

ASkillIndicator* ASkillIndicator::GetCurrent = nullptr;

// Sets default values
ASkillIndicator::ASkillIndicator()
{
	PrimaryActorTick.bCanEverTick = false;

	m_fRadius = 100.f;
	m_CollSphere=CreateDefaultSubobject<USphereComponent>("Sphere00");
	m_CollSphere->SetSphereRadius(m_fRadius);

	RootComponent = m_CollSphere;

	m_Indicator=CreateDefaultSubobject<UDecalComponent>("Decal00");
	m_Indicator->SetupAttachment(RootComponent);
	
}

void ASkillIndicator::SetRadiusScale(float radiusWant)
{
	float Rate = radiusWant / m_fRadius;
	
	//m_CollSphere->SetSphereRadius(radiusWant);

	SetActorRelativeScale3D(FVector(Rate,Rate,Rate));
}

