#include "CameraDissolve.h"


UCameraDissolve::UCameraDissolve()
{
    PrimaryComponentTick.bCanEverTick = true;
    bAutoActivate = false;
    bTickInEditor = true;
    PrimaryComponentTick.TickGroup = TG_PostPhysics;

    static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> FoundCollection(
        TEXT("MaterialParameterCollection'/Game/03_VisualEffect/PC_WallDissolveAndFog.PC_WallDissolveAndFog'"));

    m_MatParamAsset = FoundCollection.Object;

    m_TargetCam = nullptr;
    m_fDissloveAmount = 0.f;
    m_fDissolveMaxAmount = 0.35f;
    m_fDissolveHoleRadius = 0.f;
    m_fDissolveHoleMaxRadius = 150;
    m_fDissolvingTime = 0.5f;
    m_fTimer = 0.f;
    m_bWasBlocked = false;
    m_fCastSphereRadius=75;
    //
    m_CameraLagSpeed=10.f;

    TargetArmLength = 2200;

    m_RelativeSocketRotation = FQuat::Identity;

}

void UCameraDissolve::Init(USceneComponent* camWantFollow)
{
    m_TargetCam = camWantFollow;
    SetActive(true);
}

void UCameraDissolve::StartDissolve()
{
    m_fTimer = m_fDissolvingTime;
}

void UCameraDissolve::EndDissolve()
{
    m_fTimer = 0.35f;
}

void UCameraDissolve::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    UpdateDesiredArmLocation(DeltaTime);
    ExecuteDissolve(DeltaTime);
    SetValueParameter();
}


void UCameraDissolve::ExecuteDissolve(float DeltaTime)
{
    if (m_fTimer > 0.f)
    {
        m_fTimer -= DeltaTime;

        if (m_bWasBlocked)
        {
            m_fDissloveAmount = FMath::FInterpTo(m_fDissloveAmount, m_fDissolveMaxAmount, DeltaTime, 5.f);

            m_fDissolveHoleRadius = FMath::FInterpTo(m_fDissolveHoleRadius, m_fDissolveHoleMaxRadius, DeltaTime, 5.f);
        }
        else
        {
            m_fDissloveAmount = FMath::FInterpTo(m_fDissloveAmount, 0.f, DeltaTime, 5);

            m_fDissolveHoleRadius = FMath::FInterpTo(m_fDissolveHoleRadius, 0.f, DeltaTime, 5);
        }
    }
}


void UCameraDissolve::SetValueParameter()
{
    UKismetMaterialLibrary::SetScalarParameterValue(GetOwner(),m_MatParamAsset,"Amount", m_fDissloveAmount);
    UKismetMaterialLibrary::SetScalarParameterValue(GetOwner(),m_MatParamAsset,"Radius", m_fDissolveHoleRadius);
    //
    UKismetMaterialLibrary::SetVectorParameterValue(GetOwner(),m_MatParamAsset,"Position1", m_CompOrigin+m_CastOffset);
    UKismetMaterialLibrary::SetVectorParameterValue(GetOwner(),m_MatParamAsset,"Position2", m_TargetPos);
}

FTransform UCameraDissolve::GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace) const
{
    FTransform RelativeTransform(m_RelativeSocketRotation, m_RelativeSocketLocation);

    switch (TransformSpace)
    {
    case RTS_World:
        {
            return RelativeTransform * GetComponentTransform();
        }
    case RTS_Actor:
        {
            if (const AActor* Actor = GetOwner())
            {
                FTransform SocketTransform = RelativeTransform * GetComponentTransform();
                return SocketTransform.GetRelativeTransform(Actor->GetTransform());
            }
            break;
        }
    case RTS_Component:
        {
            return RelativeTransform;
        }
    }
    return RelativeTransform;
}

void UCameraDissolve::UpdateDesiredArmLocation(float DeltaTime)
{
    m_CompOrigin = GetComponentLocation() ;
    
    
    FRotator DesiredRot = GetRelativeRotation();
    m_TargetPos = m_CompOrigin;

    m_TargetPos -= DesiredRot.Vector() * TargetArmLength;

    

    FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(SpringArm), false, GetOwner());
    FHitResult Result;
    GetWorld()->SweepSingleByChannel(Result, m_CompOrigin, m_TargetPos, FQuat::Identity, ECC_Camera,
                                     FCollisionShape::MakeSphere(m_fCastSphereRadius), QueryParams);

    if (Result.bBlockingHit)
    {
        if (!m_bWasBlocked)
        {
            StartDissolve();
        }
    }
    else
    {
        if (m_bWasBlocked)
        {
            EndDissolve();
        }
    }

    m_bWasBlocked = Result.bBlockingHit;

    m_TargetPos = FMath::VInterpTo(m_PreviousPos, m_TargetPos, DeltaTime, m_CameraLagSpeed);

    FTransform WorldCamTM(DesiredRot, m_TargetPos);
    FTransform RelCamTM = WorldCamTM.GetRelativeTransform(GetComponentTransform());
    m_RelativeSocketLocation = RelCamTM.GetLocation();
    m_RelativeSocketRotation = RelCamTM.GetRotation();

    m_PreviousPos = m_TargetPos;
    UpdateChildTransforms();
}
