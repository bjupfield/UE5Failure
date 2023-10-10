// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SecondAttempt.generated.h"

UCLASS()
class MINE_API ASecondAttempt : public APawn
{
	GENERATED_BODY()
private:

	TArray<FIntVector2> bresenhamCircle;

public:
	// Sets default values for this pawn's properties
	ASecondAttempt();


protected:
	// Called when the game starts or when spawned
	// virtual void BeginPlay() override;
	//the guy destroyed this, just commmenting it out so we remember it exist for later use

	// std::map<FString, UMaterialInterface*> strToMatMap;
	TMap<FString, UMaterialInterface*> strToMatMap;

	UPROPERTY()
	class UEnhancedInputComponent* EIcomp;

	UPROPERTY()
	class AToTurnBlue* Pcontr;

	void moveCameraAboveObject(AActor* targetObject);
	
	void moveCameraBack();

	bool rayCastCheckForActor(AActor*& actorRef, bool drawDebug = false, bool pos = false, FVector startPos = FVector(0,0,0), FVector endPos = FVector(0,0,0));

	FVector ogRelCamTransLoc;

	FQuat ogRelCamTransRot;

public:	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;
	// he also destroyed this which seems evenw weirder

	UPROPERTY(EditAnywhere)
	class UBoxComponent* box;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* body;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* camera;

	UPROPERTY(EditAnywhere)
	class UFloatingPawnMovement* movement;

	UPROPERTY(EditAnywhere)
	float moveAdjust;

	UPROPERTY(EditAnywhere)
	float rotateAdjust;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* material;

	UPROPERTY(EditAnywhere)
	int circleRadius;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void move(const struct FInputActionValue& actionValue);

	void rotate(const struct FInputActionValue& actionValue);

	void turnItBlue(const struct FInputActionValue& actionValue);

	void changeMapAndCamera(const struct FInputActionValue& actionValue);

	void mouseDownTrack(const struct FInputActionValue& actionValue);
};
