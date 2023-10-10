// Fill out your copyright notice in the Description page of Project Settings.


#include "TheTurnBlueGamemode.h"
#include "ToTurnBlue.h"
#include "Mine.h"

ATheTurnBlueGamemode::ATheTurnBlueGamemode(){
    // PlayerControllerClass = AToTurnBlue::StaticClass();
    // DefaultPawnClass = ;
}

void ATheTurnBlueGamemode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);
    UE_LOG(MineLog, Log, TEXT("GAME START || MAP: %s || Options: %s"), *MapName, *Options);

}
