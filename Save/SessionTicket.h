// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SessionTicket.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API USessionTicket : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString m_SessionTicket;
};
