#pragma once

#include "DiabloM.h"
#include "GameFramework/PlayerController.h"
#include "Widgets/CommonElement/DamageTextWidgetComponent.h"

#include "DiabloPlayerController.generated.h"

/**
 * 
 */


UCLASS()
class DIABLOM_API ADiabloPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADiabloPlayerController();
	
protected:
	UPROPERTY()
	TSubclassOf<UDamageTextWidgetComponent> m_ClassDW;
	UPROPERTY()
	TArray<UDamageTextWidgetComponent*> m_AryDmgWC;
	
	FTextFormat m_FormatMiss;
	
	int m_DmgIndex;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Transient,ReplicatedUsing = OnRep_CurrentMsg,Category="Chat")
	FString m_CurrentMsg;
	UPROPERTY()
	TArray<AActor*> m_AryIgnoreActors;

	TArray<TEnumAsByte<EObjectTypeQuery>> m_AryQuery;
protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
	
	void InitWidget();

	void CreateDmgWC(int count);

	UDamageTextWidgetComponent* GetDmgWC(); 

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void SetupInputComponent() override;

	void ExitGame();

public:
	UFUNCTION()
	void OnDeviceBackKey();
	//
	void ShowDamageNumber(const BigInt& local_damage_done,AUnitPawn* unit_pawn,EDamagePopup dmgPopup); //target

	void ShowDamageText(const FString stringWant,AUnitPawn* unit_pawn,EDamagePopup dmgPopup); //target

	UFUNCTION()
    void BackToSelectMenu();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable,Category="Chat")
	void AttemptToSendChatMessage(const FString& msg);

private://server only
	void SendChatMsg(const FString& msg);

	void ClearChatmsg();

	UFUNCTION(Server,Reliable,WithValidation)
	void ServerSendChatMsg(const FString& msg);
	void ServerSendChatMsg_Implementation(const FString& msg);
	bool ServerSendChatMsg_Validate(const FString& msg);
	
	UFUNCTION()
    void OnRep_CurrentMsg();

	void UpdateChatText();

public:
	UFUNCTION()
	void ClickActor();
};
