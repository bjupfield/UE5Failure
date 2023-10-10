// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TheTurnBlueGamemode.generated.h"

/**
 * 
 */
UCLASS()
class MINE_API ATheTurnBlueGamemode : public AGameModeBase
{
	GENERATED_BODY()
	
	public:
	
	ATheTurnBlueGamemode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
};
