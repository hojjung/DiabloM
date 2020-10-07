#include "ImageAndText.h"

UImageAndText::UImageAndText(const FObjectInitializer& objInit) : Super(objInit)
, m_StringWant("Test String")
, m_nFontSize(24)
, m_nOutlineSize(1)
, m_fIconSize(64.f)
, m_NameTypeFace("Regular")
{
	//Font'/Game/Fonts/FredokaOne-Regular_Font.FredokaOne-Regular_Font'
	static ConstructorHelpers::FObjectFinder<UFont> FoundFont(
		TEXT("Font'/Game/Fonts/FredokaOne-Regular_Font.FredokaOne-Regular_Font'"));
	m_FontWant = FoundFont.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D> FoundTexture(
		TEXT("Texture2D'/Game/Sprite/UI/fg4_icons_daggers.fg4_icons_daggers'"));

	m_IconWant = FoundTexture.Object;



}

void UImageAndText::NativePreConstruct()
{
	Super::NativePreConstruct();
	//
	SetString(FText::FromString( m_StringWant));
	//
	FSlateBrush BrushWant;
	BrushWant.SetImageSize(FVector2D(64.f,64.f));
	BrushWant.SetResourceObject(m_IconWant);
	m_ImageIcon->SetBrush(BrushWant);
	//
	m_SizeBoxWrap->SetWidthOverride(m_fIconSize);
	m_SizeBoxWrap->SetHeightOverride(m_fIconSize);
	//
	FSlateFontInfo FontInfo;
	FontInfo.FontObject = m_FontWant;
	FontInfo.Size = m_nFontSize;
	FontInfo.TypefaceFontName = m_NameTypeFace;
	FontInfo.OutlineSettings.OutlineSize = m_nOutlineSize;
	m_TextStringShow->SetFont(FontInfo);
	//

}

void UImageAndText::SetIcon(UTexture* texture)
{
	FSlateBrush BrushWant;
	BrushWant.SetImageSize(FVector2D(64.f,64.f));
	BrushWant.SetResourceObject(texture);
	m_ImageIcon->SetBrush(BrushWant);
}

void UImageAndText::SetString(FText stringWant)
{
	m_TextStringShow->SetText(stringWant);
}

void UImageAndText::SetStringColor(FLinearColor colorWant)
{
	m_TextStringShow->SetColorAndOpacity(colorWant);
}
