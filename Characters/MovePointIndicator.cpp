#include "MovePointIndicator.h"
#include "ConstructorHelpers.h"
#include "Components/DecalComponent.h"
#include "Particles/ParticleSystem.h"

AMovePointIndicator::AMovePointIndicator()
{
	PrimaryActorTick.bCanEverTick = false;
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
	FoundW(TEXT("Material'/Game/03_VisualEffect/Decals/M_SkillIndicator.M_SkillIndicator'"));

	m_Decal = CreateDefaultSubobject<UDecalComponent>("Decal01");
	m_Decal->SetupAttachment(RootComponent);
	m_Decal->SetRelativeRotation(FRotator(-90,0,0));
	m_Decal->SetRelativeScale3D(FVector(0.2f));
	m_Decal->SetDecalMaterial(FoundW.Object);
}

