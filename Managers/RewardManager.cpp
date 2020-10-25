#include "RewardManager.h"
#include "CBezierCurve.h"
#include "DiabloGameMode.h"

void URewardManager::RequestMonsterDropItem(int level)
{
}

void URewardManager::BezierCurveMove(AActor* target, float height, FVector destination, FOnEnd* endCallback)
{
    FVector StartPoint = target->GetActorLocation();
    UCBezierCurve* Curve = UCActionFactory::MakeCurve();
    FVector ControlPoint = GetQuadControlPoint(StartPoint, destination, height);
    Curve->InitializeQuad(StartPoint, ControlPoint, destination);

    UCFollowBezierCurvePathAction* PathAction = UCActionFactory::MakeFollowBezierPathAction(target, Curve, 3.f);

    if (endCallback)
    {
        PathAction->m_OnComplete = *endCallback;
    }

    ADiabloGameMode::Get->GetActionManager()->RunAction(PathAction);
}

FVector URewardManager::GetQuadControlPoint(FVector start, FVector end, float height)
{
    FVector Result;

    Result.X = start.X + ((end.X - start.X) / 2.f);
    Result.Y = start.Y + ((end.Y - start.Y) / 2.f);
    Result.Z = start.Z + ((end.Z - start.Z) / 2.f);
    Result.Z+=height;
    return Result;
}
