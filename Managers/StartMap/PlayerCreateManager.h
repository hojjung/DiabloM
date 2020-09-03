// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 월드맵에서 쓰일 캐릭터를 미리
 * 렌더링 하고 데이터를 취합해줌
 *
 * 위젯 세팅이 중요한데 결국 플레이어 컨트롤러가 시작하는것임
 * 이 클래스를 플레이어 컨트롤러가 사용하게 끔 학것
 */
class DIABLOM_API PlayerCreateManager
{
public:
	PlayerCreateManager();
	~PlayerCreateManager();
};
