// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"

#include "DungeonMiniMap.h"
#include "Image.h"
#include "Overlay.h"
#include "Blueprint/UserWidget.h"
#include "MinimapWidget.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UMinimapWidget : public UUserWidget
{
	GENERATED_BODY()
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	//UDungeonMiniMap

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Transient)
	UMaterialInterface* m_MatMinimapInst;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_MinimapCenterFull;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UOverlay* m_OverlayMinimapWrap;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UImage* m_MinimapCornerSmall;

	FVector2D* m_PlayerScreenPos;
protected:
	float m_fMaxX;
	float m_fMinX;
	float m_fMaxY;
	float m_fMinY;
public:

	void SetMaxMinCornerMap();
	void SetMinimapMat(UMaterialInterface* material_interface);

	void UpdateMinimapWidget();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};
