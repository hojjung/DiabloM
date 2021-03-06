// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Widgets/SetNickname.h"

#include "InitPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API AInitPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AInitPlayerController();

	UPROPERTY()
	TSubclassOf<USetNickname> m_ClassWidget;
	UPROPERTY()
	USetNickname* m_Canvas;

	virtual void BeginPlay() override;
	
};
