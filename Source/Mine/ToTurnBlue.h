// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "ToTurnBlue.generated.h"

/**
 * 
 */
UCLASS()
class MINE_API AToTurnBlue : public APlayerController
{
	GENERATED_BODY()


public:

	// UEnhancedInputComponent* inputComp;

	AToTurnBlue();

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
	
	virtual void Tick(float DeltaTime) override;

	virtual void turning(const FInputActionInstance& Instance);
	
	UPROPERTY()
	class UInputMappingContext* theMappingContext;

	UPROPERTY()
	class UInputMappingContext* drawingMapping;

	UPROPERTY()
	UInputAction* TurnBlueMotherFucker;

	UPROPERTY()
	UInputAction* MoveInput;

	UPROPERTY()
	UInputAction* RotateInput;

	UPROPERTY()
	UInputAction* ChangeInput;

	UPROPERTY()
	UInputAction* MouseDown;

	float mouseX;

	float mouseY;
};
