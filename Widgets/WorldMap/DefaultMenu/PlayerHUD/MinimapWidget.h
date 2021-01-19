// My First Hack n Slash

#pragma once

#include "DiabloM.h"

#include "DungeonMiniMap.h"
#include "Image.h"
#include "Overlay.h"
#include "Blueprint/UserWidget.h"
#include "Managers/DungeonManager.h"


#include "MinimapWidget.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UMinimapWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMinimapWidget(const FObjectInitializer& objInit);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D m_MinimapWidgetSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_fPortalHoldTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_fPortalDelayTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_fCornerMapScale;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Transient)
	UMaterialInterface* m_MatMinimapInst;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_MinimapCenterFull;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UOverlay* m_OverlayMinimapWrap;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_MinimapCornerSmall;
	UPROPERTY(EditAnywhere)
	UTexture2D* m_CancelMinimapTexture;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UButton* m_PortalBtn;
	
	UPROPERTY()
	UImage* m_SelectedUpdateImage;
	UPROPERTY()
	UDungeonManager* m_DgM;
	
protected:
	FVector2D* m_PlayerScreenPos;

	FVector2D m_OverlaySize;

	bool m_bIsUsingCornerMap;

	float m_fCornerMapScaleRatio;

	FTimerHandle m_TimerHandle_OnTimer;

protected:
	FVector2D TransformScreenCoordToMyWidget(const FVector2D& sceenPos);

	void SetOverlaySize();

	void SetFullMinimap();

	void SetCornerMinimap();

	void UpdateMinimapWidget();
	
	void UpdateCornerMapTransform();
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetCornerScaleValue(float scaleV);
	
public:
	void Init();
	UFUNCTION()
	void OnBtnPressHold();
	UFUNCTION()
	void OnBtnReleaseHold();
	
	void SetMinimapMat(UMaterialInterface* material_interface);
	
	void HideCornerMinimap();

	UFUNCTION()
	FEventReply OnCornerMapClicked(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

	void HideMinimap();

	void ShowMinimap();
};


