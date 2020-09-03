// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerVisual.h"

// Sets default values
APlayerVisual::APlayerVisual()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerVisual::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerVisual::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

