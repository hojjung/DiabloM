#include "ShopKeeper.h"
#include "Characters/DiabloPlayerController.h"
#include "Managers/DiabloGameInstance.h"

AShopKeeper::AShopKeeper(const FObjectInitializer& objInit):Super(objInit)
{
    m_MeshShopKeeper = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh00");
    m_MeshShopKeeper->SetupAttachment(m_Capsule);
}

void AShopKeeper::BeginPlay()
{
    Super::BeginPlay();

    m_AryShopItemContainer.Reset();
    
    int i=0;
    
    while (i++<2)
    {
        UShopItemContainer* ShopPanel = NewObject<UShopItemContainer>();
        ShopPanel->InitShopStorage(SHOP_X,SHOP_Y);
        m_AryShopItemContainer.Emplace(ShopPanel);
    }
    UShopItemContainer* ShopPanel = NewObject<UShopItemContainer>();
    ShopPanel->InitShopStorage(SHOPRESELL_X,SHOPRESELL_Y);
    m_AryShopItemContainer.Emplace(ShopPanel);

    SetItemFromDataTable1();
    SetItemFromDataTable2();
    //SetItemFromDataTable3();
}

void AShopKeeper::SetItemFromDataTable1()
{
    const FShopItemSellData* ShopItemData = m_ShopItemSellHandle1.GetRow<FShopItemSellData>("");

    for(const FShopItemSell& ItemSell : ShopItemData->m_AryShopItemSell)
    {
        FItemInstance ItemCreated = UDiabloGameInstance::Get->CreateItemManual(ItemSell);
        m_AryShopItemContainer[0]->AddItemAuto(ItemCreated);
    }
}

void AShopKeeper::SetItemFromDataTable2()
{
    const FShopItemSellData* ShopItemData = m_ShopItemSellHandle2.GetRow<FShopItemSellData>("");

    for(const FShopItemSell& ItemSell : ShopItemData->m_AryShopItemSell)
    {
        FItemInstance ItemCreated = UDiabloGameInstance::Get->CreateItemManual(ItemSell);
        m_AryShopItemContainer[1]->AddItemAuto(ItemCreated);
    }
}

void AShopKeeper::Interact(AActor* instigator)
{
    TWeakObjectPtr<ADiabloPlayerController> DiaPC = ADiabloPlayerController::Get;
    
    DiaPC->GetMainCanvas()->OpenBasicShopMenu(this);
}
