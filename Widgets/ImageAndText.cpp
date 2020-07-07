#include "ImageAndText.h"
#include "ConstructorHelpers.h"

UImageAndText::UImageAndText(const FObjectInitializer& objInit) : Super(objInit)
, m_StringWant("Test String")
, m_nFontSize(24)
, m_fIconSize(64.f)
, m_NameTypeFace("Regular")
{
	static ConstructorHelpers::FObjectFinder<UFont> FoundFont(
		TEXT("Font'/Game/Fonts/Roboto_2.Roboto_2'"));
	m_FontWant = FoundFont.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D> FoundTexture(
		TEXT("Texture2D'/Game/Sprite/fantasy_gui_4/textures/icons/items/fg4_icons_daggers.fg4_icons_daggers'"));

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
	m_TextStringShow->SetFont(FontInfo);
	//

}

void UImageAndText::SetString(FText stringWant)
{
	m_TextStringShow->SetText(stringWant);
}

void UImageAndText::SetStringColor(FLinearColor colorWant)
{
	m_TextStringShow->SetColorAndOpacity(colorWant);
}
