#pragma once

#include "CircularThrobber.h"
#include "DiabloM.h"
#include "Blueprint/UserWidget.h"
#include "GameStartCanvas.generated.h"

class UPlayfabManager;
/**
 * 
 */
UCLASS()
class DIABLOM_API UGameStartCanvas : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnPrivacyPolicy;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnGameUsePolicy;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnRefund;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnStore;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCircularThrobber* m_ShowLoadingThrobber;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextClientVersion;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextAPIInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextAPIInfoInPopup;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* m_CanvasPopupConfirm;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnConfirm;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* m_CanvasNicknameSet;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UEditableText* m_Nickname;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnConfirmNickname;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnNews01;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnNews02;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnNews03;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnNews04;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnNews05;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* m_TextNewsBody;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_BtnEnterLevel;
	
protected:
	UPROPERTY()
	TArray<UButton*> m_AryNewsButtons;
	
	TWeakObjectPtr<UPlayfabManager> m_PlayfabManager;
	
	TArray<FString> m_AryNewsBody;
	//Login Flag
	bool m_bTitleNewsLock;

	bool m_bSetNicknameLock;

	bool m_bProcceedLoginLock;

	bool m_bNicknameDone;
	
	bool m_bRequestLock;

	bool m_bGameStartLock;

	bool m_bIsServerClosedLock;

	bool m_bIsVersionWrongLock;

	FDelegateHandle m_Handle;
public:
	virtual void NativeOnInitialized() override;

	virtual void NativeDestruct() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	void ReqeustPopupText(const FText& str);

	void OpenErrorPopupPanel(const FString& str);

public:
	UFUNCTION()
	void OpenPrivacyPolicy();
	UFUNCTION()
    void OpenGameUsePolicy();
	UFUNCTION()
    void OpenRefund();
	UFUNCTION()
    void OpenStore();
	UFUNCTION()
	void ConfirmNickname();
	UFUNCTION()
	void OnTextChanged(const FText& text);
	UFUNCTION()
	void ShowNews01();
	UFUNCTION()
    void ShowNews02();
	UFUNCTION()
    void ShowNews03();
	UFUNCTION()
    void ShowNews04();
	UFUNCTION()
    void ShowNews05();
	UFUNCTION()
	void ConfirmPopup();
	UFUNCTION()
	void EnterLevel();

	
};
