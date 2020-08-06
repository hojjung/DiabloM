// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class DIABLOM_API SaveLoadManager
{
public:
	static SaveLoadManager* Get;
	
	void CreateSaveLoadInstance();
	~SaveLoadManager();

public:
	void SaveInventory() const;

	void LoadInventory() const;
	
};
