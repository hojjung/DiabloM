#include "NormalDungeonManager.h"
#include "DungeonManager.h"
#include "EngineUtils.h"
#include "JsonSerializer.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/StartMap/PlayerVisual.h"
#include "Engine/AssetManager.h"
#include "Managers/DiabloGameInstance.h"
#include "Widgets/HUD/MainCanvas.h"

UDataTable* UNormalDungeonManager::DungeonDataTable = nullptr;
UDataTable* UNormalDungeonManager::DropDataTable = nullptr;
UDataTable* UNormalDungeonManager::MonsterEntityTable = nullptr;
#define BOSSDURATION 12.f

UNormalDungeonManager::UNormalDungeonManager()
{
	//Blueprint'/Game/Blueprints/BP_StompShake.BP_StompShake'
	static ConstructorHelpers::FClassFinder<UCameraShake> FoundCamshake(
		TEXT("Blueprint'/Game/Blueprints/BP_StompShake.BP_StompShake_C'"));
	m_ClassShake = FoundCamshake.Class;
	m_SensingInterval = 5.f;
	m_CurrentWorld = nullptr;
	m_NavSys = nullptr;
	m_fSpawnRadius = 6200.f;
	m_IdEnemy = "enemy";
	m_IdBossEnemy = "boss";
	m_IdSpecialEnemy = "special";

	static ConstructorHelpers::FObjectFinder<UDataTable> FoundData(
		TEXT("DataTable'/Game/DataTables/Entities/MonsterTable.MonsterTable'"));
	m_MobEntityTable = FoundData.Object;
	//(DataTable=DataTable'"/Game/DataTables/Entities/MonsterTable.MonsterTable"',RowName="GoldGoblin")

	m_nGoldGoblinSpawnCount = 0;
	//
	m_CurrentDg = nullptr;
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundDungeon(
		TEXT("DataTable'/Game/DataTables/Dungeon/DungeonData.DungeonData'"));

	DungeonDataTable = FoundDungeon.Object;

} //m_MonsterManager->StartSpawn(world, m_CurrentDg);

void UNormalDungeonManager::Init()
{
	m_GoldGoblinEntity = m_MobEntityTable->FindRow<FMonsterEntity>("GoldGoblin", "");
}

void UNormalDungeonManager::OnLevelLoadComplete(UWorld* world)
{
	m_nGoldGoblinSpawnCount=0;
	m_nKillCount=0;

	StartSpawn(world, m_CurrentDg);
	SpawnVisualActor(world);
	SpawnOtherPVPActor(world);
	
	AGameLevelHUD* MyHud = Cast<AGameLevelHUD>(UDiabloGameInstance::Get->GetPlCon()->GetHUD());
	MyHud->m_Canvas->m_OnMenuVisibleChanged.AddUObject(this, &UNormalDungeonManager::OnMenuOpen);

	//

}

void UNormalDungeonManager::StartSpawn(UWorld* world, const FDungeonDataTableRow* dgData)
{
	m_nKillCount = 0;

	m_nGoldGoblinSpawnCount = FMath::RandRange(10, 25);

	m_nGoldGoblinSpawnCount += m_nKillCount;

	m_CurrentWorld = world;

	m_NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(m_CurrentWorld);

	m_DgDataTable = dgData;

	if (!m_DgDataTable)
	{
		return;
	}

	Reset();

	int i = 0;

	while (i++ < MonsterPoolCount)
	{
		AMonsterPawn* SpawnedMob = CreateMob(FVector::ZeroVector);

		m_AryMonsterSpawnedCurrently.Add(SpawnedMob);

		SpawnedMob->m_OnDeathAnimBefore.AddUObject(this, &UNormalDungeonManager::OnMonsterDead);

		SpawnMobToLoc(FVector::ZeroVector);
	}

	SetSpawnMonsterOnTick(true);
}

void UNormalDungeonManager::SetSpawnMonsterOnTick(const bool bEnabled)
{
	if (bEnabled && m_SensingInterval > 0.f)
	{
		const float InitialDelay = (m_SensingInterval * FMath::SRand()) + KINDA_SMALL_NUMBER;

		SetTimer(InitialDelay);
	}
	else
	{
		SetTimer(0.f);
	}
}


void UNormalDungeonManager::SetTimer(const float TimeInterval)
{
	if (m_CurrentWorld && GEngine->GetNetMode(GetWorld()) < NM_Client)
	{
		m_CurrentWorld->GetTimerManager().SetTimer(m_TimerHandle_OnTimer, this, &UNormalDungeonManager::OnTimer,
		                                           TimeInterval,
		                                           false);
	}
}

void UNormalDungeonManager::OnTimer()
{
	if (!m_CurrentWorld)
	{
		return;
	}
	//Spawn
	for (int i = 0; i < 5; i++)
	{
		if (!SpawnMobToLoc(FVector::ZeroVector))
		{
			break;
		}
	}

	SetTimer(m_SensingInterval);
};


void UNormalDungeonManager::Reset()
{
	for (AMonsterPawn* Mob : m_AryMonsterSpawnedCurrently)
	{
		if (!Mob)
		{
			continue;
		}
		Mob->Destroy();
	}

	m_AryMonsterSpawnedCurrently.Reset();
}

AMonsterPawn* UNormalDungeonManager::GetReadyMonster()
{
	AMonsterPawn* SelectedPawn = nullptr;

	for (AMonsterPawn* MPawn : m_AryMonsterSpawnedCurrently)
	{
		if (MPawn->IsReadyToPool()) //죽은애만 데려옴
		{
			SelectedPawn = MPawn;
			break;
		}
	}

	return SelectedPawn;
}

AMonsterPawn* UNormalDungeonManager::SpawnMobToLoc(FVector loc)
{
	AMonsterPawn* Mob = GetReadyMonster();

	if (!Mob)
	{
		return nullptr;
	}

	float MinX = loc.X - 500.f;
	float MaxX = loc.X + 500.f;

	float MinY = loc.Y - 500.f;
	float MaxY = loc.Y + 500.f;

	loc.X = FMath::RandRange(MinX, MaxX);
	loc.Y = FMath::RandRange(MinY, MaxY);


	FVector NewLoc = GetRandomPointFromNav(loc, 4000.f);

	const FMonsterEntityHandle& MobHandle = m_DgDataTable->m_Monsters.GetRandom();

	const FMonsterEntity* MonData = MobHandle.GetRow<FMonsterEntity>("");

	NewLoc.Z += Mob->GetCapsule()->GetScaledCapsuleHalfHeight();

	Mob->SetActorLocation(NewLoc);

	float GoldScale = 1.f;

	float StatScale = 1.f;

	if (m_nKillCount >= m_nGoldGoblinSpawnCount)
	{
		m_nGoldGoblinSpawnCount = FMath::RandRange(25, 35);

		m_nGoldGoblinSpawnCount += m_nKillCount;

		MonData = m_GoldGoblinEntity;

		GoldScale = 15.f;

		StatScale = 5.f;
	}

	Mob->DataInject(MonData, m_DgDataTable->GetMobHp(), m_DgDataTable->GetMobGold(), EMonsterType::Normal, StatScale,
	                MonData->m_fScale, GoldScale);

	return Mob;
}

void UNormalDungeonManager::OnBossDead(AMonsterPawn* pawn)
{
	m_SpawnedBoss->m_OnDeathAnimAfter.Remove(m_BossDeleHandle);
	m_SpawnedBoss = nullptr;
	pawn->Destroy();
	UDiabloGameInstance::Get->m_PlayerUpgradeManager->ClearCooldownAllSkill();

	EndDungeon(true);
}

void UNormalDungeonManager::BeginDestroy()
{
	Super::BeginDestroy();

}

AUnitPawn* UNormalDungeonManager::GetNearestEnemy(const FVector& wantPos)
{
	if (m_SpawnedBoss)
	{
		return m_SpawnedBoss;
	}

	float Dist = FLT_MAX;

	AMonsterPawn* ResultMob = nullptr;

	for (AMonsterPawn* Mob : m_AryMonsterSpawnedCurrently)
	{
		if (!Mob || Mob->IsReadyToPool() || !Mob->IsAlive())
		{
			continue;
		}

		float DistNew = FVector::DistSquared2D(Mob->GetActorLocation(), wantPos);

		if (Dist >= DistNew)
		{
			Dist = DistNew;

			ResultMob = Mob;
		}
	}

	return ResultMob;
}

FVector UNormalDungeonManager::GetRandomPointFromNav(const FVector& loc, const float& radius)
{
	FNavLocation ResultLoc;

	if (!m_NavSys->GetRandomReachablePointInRadius(loc, radius, ResultLoc))
	{
		//FAIL
		return loc;
	}

	return ResultLoc;
}

AMonsterPawn* UNormalDungeonManager::CreateMob(FVector loc)
{
	FActorSpawnParameters Param;

	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	Param.bNoFail = true;

	//88
	FRotator Rot;
	Rot.Pitch = 0.f;
	Rot.Roll = 0.f;
	Rot.Yaw = FMath::RandRange(-360.f, 360.f);

	AMonsterPawn* Mob = m_CurrentWorld->SpawnActor<AMonsterPawn>(AMonsterPawn::StaticClass(), loc, Rot, Param);
	Mob->SetAcive(false);

	check(Mob);

	return Mob;
}


void UNormalDungeonManager::AddKillCount()
{
	m_nKillCount++;
}

void UNormalDungeonManager::SpawnBossMob()
{
	//
	UGameplayStatics::GetPlayerController(UDiabloGameInstance::Get->GetWorld(), 0)->ClientPlayCameraShake(m_ClassShake);
	//
	FVector PlayerLoc = UGameplayStatics::GetPlayerPawn(UDiabloGameInstance::Get->GetWorld(), 0)->GetActorLocation();

	FVector NewLoc = GetRandomPointFromNav(PlayerLoc, 1000.f);

	auto* Mob = CreateMob(PlayerLoc);

	const FMonsterEntityHandle& MobHandle = m_DgDataTable->m_Monsters[0];

	const FMonsterEntity* MonData = MobHandle.GetRow<FMonsterEntity>("");

	//m_fBossMonsterStatFactor

	NewLoc.Z += Mob->GetCapsule()->GetScaledCapsuleHalfHeight();

	Mob->SetActorLocation(NewLoc);

	Mob->DataInject(MonData, m_DgDataTable->GetMobHp(), m_DgDataTable->GetMobGold(), EMonsterType::Boss, 10,
	                MonData->m_fBossMonsterRenderScale, 10.f);

	m_SpawnedBoss = Mob;

	m_BossDeleHandle = m_SpawnedBoss->m_OnDeathAnimAfter.AddUObject(this, &UNormalDungeonManager::OnBossDead);

	APlayerDiabloCharacter* Pl = UDiabloGameInstance::Get->GetPlChar();

	Pl->FocusTarget(m_SpawnedBoss);

	m_fBossTimer=0.f;
}

void UNormalDungeonManager::FailBossKill()
{
	//쿨타임 생기고
	//보스 없애기
	m_SpawnedBoss->Destroy();
	m_SpawnedBoss = nullptr;

	EndDungeon(false);
}


void UNormalDungeonManager::SetDungeonData(const FString& dgJsonStr)
{
	DungeonDataTable->GetAllRows("", m_AryDgDataTable);

	TSharedPtr<FJsonObject> JsonObject;

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(dgJsonStr);

	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		return;
	}

	int StageCurrentLevel = JsonObject->GetIntegerField(TEXT("CurrentStageLevel"));

	m_nMyMaxStageLevel = JsonObject->GetIntegerField(TEXT("MaxStageLevel"));

	if (StageCurrentLevel < 0 || StageCurrentLevel >= m_AryDgDataTable.Num())
	{
		StageCurrentLevel = 0;
	}

	SelectNormalDungeon(StageCurrentLevel);
	//
}

void UNormalDungeonManager::SelectNormalDungeon(int index)
{
	m_nCurrentStageLevel = index;
	m_CurrentDg = m_AryDgDataTable[m_nCurrentStageLevel.GetValue()];
}

void UNormalDungeonManager::LevelUpDungeon()
{
	m_nCurrentStageLevel = m_nCurrentStageLevel.GetValue();

	int NextLevel = (m_nCurrentStageLevel + 1).GetValue();

	if (NextLevel >= m_AryDgDataTable.Num())
	{
		return; //MAXStage
	}

	++m_nCurrentStageLevel;

	m_nCurrentStageLevel = m_nCurrentStageLevel.GetValue();

	SelectNormalDungeon(m_nCurrentStageLevel.GetValue());

	if (m_nCurrentStageLevel > m_nMyMaxStageLevel.GetValue())
	{
		PRINTF("DgManager-LevelUpDungeon HighScore");

		m_nMyMaxStageLevel = m_nCurrentStageLevel.GetValue();
		UDiabloGameInstance::Get->m_QuestManager->AddQuestCount(EQuestType::StageLv);
		UDiabloGameInstance::Get->m_PlayfabManager->UploadNormalDungeon();
		m_OnDungeonMaxUpdate.Broadcast();
	}

	m_OnDgOpen.Broadcast(m_nCurrentStageLevel.GetValue());

	//UDiabloGameInstance::Get->m_PlayfabManager->UploadMainData();
	UDiabloGameInstance::Get->m_DungeonManager->OpenLevel(this);
	//OpenLevel();
}

BigInt UNormalDungeonManager::GetCurrentDungeonBounty()
{
	return m_CurrentDg->GetMobGold();
}

BigInt UNormalDungeonManager::GetMaxDungeonBounty()
{
	return m_AryDgDataTable[m_nMyMaxStageLevel.GetValue()]->GetMobGold();
}

FString UNormalDungeonManager::GetDgDataStr()
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetNumberField(TEXT("CurrentStageLevel"), m_nCurrentStageLevel.GetValue());

	JsonObject->SetNumberField(TEXT("MaxStageLevel"), m_nMyMaxStageLevel.GetValue());

	return PlayFab::FJsonKeeper(JsonObject).toJSONString();
}


bool UNormalDungeonManager::IsBattleStarted()
{
	return m_SpawnedBoss;
}

FString UNormalDungeonManager::GetOpenLevelAssetName()
{
	return m_AryDgDataTable[m_nCurrentStageLevel.GetValue()]->m_DgId.ToString();
}

void UNormalDungeonManager::OnMonsterDead(AMonsterPawn* self)
{
	AddKillCount();

	UDiabloGameInstance::Get->m_QuestManager->AddQuestCount(EQuestType::MonsterKill);

	UDiabloGameInstance::Get->m_GoldManager->AddGold(self->GetGoldBounty());
}

void UNormalDungeonManager::StartDungeon()
{
	Super::StartDungeon();

	SpawnBossMob();
}

void UNormalDungeonManager::EndDungeon(bool b)
{
	Super::EndDungeon(b);

	if (b)
	{
		UDiabloGameInstance::Get->m_PVPManager->m_OnMatchSuccessed.Unbind();
		LevelUpDungeon();
	}
	else
	{
//		FailBossKill();
	}
}

void UNormalDungeonManager::SpawnVisualActor(UWorld* world)
{
	FRotator Rot = FRotator(0);
	Rot.Yaw = 310.f;
	FVector Loc = FVector(7777.f);
	FActorSpawnParameters Param;
	Param.bNoFail = true;
	m_VisualActor = world->SpawnActor<APlayerVisual>(APlayerVisual::StaticClass(), Loc, Rot, Param);
	m_VisualActor->HideMeshWithTick();

	m_InitVisualRot = m_VisualActor->GetSkMesh()->GetComponentRotation();

	m_VisualActor->OnMeshChanged(UDiabloGameInstance::Get->GetPlChar());
}

void UNormalDungeonManager::SpawnOtherPVPActor(UWorld* world)
{
	FRotator Rot = FRotator(0);

	Rot.Yaw = 310.f;

	FVector Loc = FVector(77777.f);

	FActorSpawnParameters Param;

	Param.bNoFail = true;

	m_OtherPlayer = world->SpawnActor<AOtherPlayerPawn>(AOtherPlayerPawn::StaticClass(), Loc, Rot, Param);

	m_OtherPlayer->HideMeshWithTick();

	m_OtherPlayer->SetPetPositionForVisual();
	//
	UDiabloGameInstance::Get->m_PVPManager->m_OnMatchSuccessed.BindUObject(
		m_OtherPlayer, &AOtherPlayerPawn::SetPVPPlayerPawn);
}

void UNormalDungeonManager::OnMenuOpen(bool b)
{
	if (b)
	{
		m_VisualActor->ShowMeshWithTick();
	}
	else
	{
		m_VisualActor->HideMeshWithTick();
		m_OtherPlayer->HideMeshWithTick();
	}
}

void UNormalDungeonManager::RotatePawn(float x)
{
	x = -1.f * x;
	FRotator Rot(0.f);
	Rot.Yaw = x * 2.f;
	m_VisualActor->GetSkMesh()->AddLocalRotation(Rot);
}

void UNormalDungeonManager::OnTouchStart()
{
	m_bTouched = true;
}

void UNormalDungeonManager::OnTouchEnd()
{
	m_bTouched = false;
}

void UNormalDungeonManager::Tick(float delta)
{
	Super::Tick(delta);

	CalculateVisualActorRot(delta);

	if(IsBattleStarted())
	{
		m_fBossTimer+=delta;

		m_OnBossBattleTick.Broadcast( 1.f - (m_fBossTimer/BOSSDURATION),BOSSDURATION - m_fBossTimer);
		
		if(m_fBossTimer>BOSSDURATION)
		{
			FailBossKill();			
		}
	}
}

void UNormalDungeonManager::CalculateVisualActorRot(float delta)
{
	if (m_bTouched)
	{
		return;
	}

	FRotator NewRot = m_VisualActor->GetSkMesh()->GetComponentRotation();

	NewRot.Yaw = UKismetMathLibrary::RInterpTo(NewRot, m_InitVisualRot, delta, 5.5f).Yaw;

	m_VisualActor->GetSkMesh()->SetWorldRotation(NewRot);

	FRotator NewRot2 = m_VisualActor->GetSkMesh()->GetComponentRotation();
}



