// My First Hack n Slash


#include "MyInAppPurchase.h"

// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "InAppPurchaseCallbackProxy.h"
#include "Async/TaskGraphInterfaces.h"
#include "GameFramework/PlayerController.h"
#include "OnlineSubsystem.h"
#include "Engine/World.h"

//////////////////////////////////////////////////////////////////////////
// UInAppPurchaseCallbackProxy

UMyInAppPurchase::UMyInAppPurchase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PurchaseRequest = nullptr;
	WorldPtr = nullptr;
	SavedPurchaseState = EInAppPurchaseState::Unknown;
}

UMyInAppPurchase* UMyInAppPurchase::CreateProxyObjectForInAppPurchase(class APlayerController* PlayerController, const FInAppPurchaseProductRequest& ProductRequest)
{
	UMyInAppPurchase* Proxy = NewObject<UMyInAppPurchase>();
	Proxy->SetFlags(RF_StrongRefOnFrame);
	Proxy->Trigger(PlayerController, ProductRequest);
	return Proxy;
}



void UMyInAppPurchase::Trigger(APlayerController* PlayerController, const FInAppPurchaseProductRequest& ProductRequest)
{
	bFailedToEvenSubmit = true;
	EInAppPurchaseState::Type TempState = EInAppPurchaseState::Unknown;

	WorldPtr = (PlayerController != nullptr) ? PlayerController->GetWorld() : nullptr;
	if (APlayerState* PlayerState = (PlayerController != nullptr) ? PlayerController->PlayerState : nullptr)
	{
		if (IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::IsLoaded() ? IOnlineSubsystem::Get() : nullptr)
		{
			IOnlineStorePtr StoreInterface = OnlineSub->GetStoreInterface();
			if (StoreInterface.IsValid())
			{
				bFailedToEvenSubmit = false;

				// Register the completion callback
				InAppPurchaseCompleteDelegate = FOnInAppPurchaseCompleteDelegate::CreateUObject(this, &UMyInAppPurchase::OnInAppPurchaseComplete);
				InAppPurchaseCompleteDelegateHandle = StoreInterface->AddOnInAppPurchaseCompleteDelegate_Handle(InAppPurchaseCompleteDelegate);

				// Set-up, and trigger the transaction through the store interface
				PurchaseRequest = MakeShareable(new FOnlineInAppPurchaseTransaction());
				FOnlineInAppPurchaseTransactionRef PurchaseRequestRef = PurchaseRequest.ToSharedRef();
				StoreInterface->BeginPurchase(ProductRequest, PurchaseRequestRef);
			}
			else
			{
				TempState = EInAppPurchaseState::NotAllowed;
				FFrame::KismetExecutionMessage(TEXT("UMyInAppPurchase::Trigger - In-App Purchases are not supported by Online Subsystem"), ELogVerbosity::Warning);
			}
		}
		else
		{
			TempState = EInAppPurchaseState::Invalid;
			FFrame::KismetExecutionMessage(TEXT("UMyInAppPurchase::Trigger - Invalid or uninitialized OnlineSubsystem"), ELogVerbosity::Warning);
		}
	}
	else
	{
		TempState = EInAppPurchaseState::Invalid;
		FFrame::KismetExecutionMessage(TEXT("UMyInAppPurchase::Trigger - Invalid player state"), ELogVerbosity::Warning);
	}

	if (bFailedToEvenSubmit && (PlayerController != NULL))
	{
		OnInAppPurchaseComplete(TempState);
	}
}


void UMyInAppPurchase::OnInAppPurchaseComplete(EInAppPurchaseState::Type CompletionState)
{
	RemoveDelegate();
	SavedPurchaseState = CompletionState;
	if (UWorld* World = WorldPtr.Get())
	{
		DECLARE_CYCLE_STAT(TEXT("FSimpleDelegateGraphTask.DelayInAppPurchaseComplete"), STAT_FSimpleDelegateGraphTask_DelayInAppPurchaseComplete, STATGROUP_TaskGraphTasks);

		FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
			FSimpleDelegateGraphTask::FDelegate::CreateLambda([=](){

				OnInAppPurchaseComplete_Delayed();

			}),
			GET_STATID(STAT_FSimpleDelegateGraphTask_DelayInAppPurchaseComplete), 
			nullptr, 
			ENamedThreads::GameThread
		);
    }
    else
    {
        PurchaseRequest = nullptr;
    }
}


void UMyInAppPurchase::OnInAppPurchaseComplete_Delayed()
{
    /** Cached product details of the purchased product */
    FInAppPurchaseProductInfo ProductInformation; 

    if (SavedPurchaseState == EInAppPurchaseState::Success && PurchaseRequest.IsValid())
    {
        ProductInformation = PurchaseRequest->ProvidedProductInformation;
    }
    
	if (SavedPurchaseState == EInAppPurchaseState::Success)
	{
		OnSuccess.Broadcast(SavedPurchaseState, ProductInformation);
	}
	else
	{
		OnFailure.Broadcast(SavedPurchaseState, ProductInformation);
	}
    
    PurchaseRequest = nullptr;
}


void UMyInAppPurchase::RemoveDelegate()
{
	if (!bFailedToEvenSubmit)
	{
		if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::IsLoaded() ? IOnlineSubsystem::Get() : nullptr)
		{
			IOnlineStorePtr InAppPurchases = OnlineSub->GetStoreInterface();
			if (InAppPurchases.IsValid())
			{
				InAppPurchases->ClearOnInAppPurchaseCompleteDelegate_Handle(InAppPurchaseCompleteDelegateHandle);
			}
		}
	}
}


void UMyInAppPurchase::BeginDestroy()
{
	PurchaseRequest = nullptr;
	RemoveDelegate();

	Super::BeginDestroy();
}


