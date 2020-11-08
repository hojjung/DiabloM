// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "Village/VillageActor.h"
#include "MapSelector.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API AMapSelector : public AVillageActor
{
    GENERATED_BODY()

public:
    AMapSelector();
    
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* m_MeshMapSelector;
    
public:
    virtual void Interact(AActor* instigator) override;
};
