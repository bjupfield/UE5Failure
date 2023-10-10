// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnThisFuckerBlue.h"

// Sets default values for this component's properties
UTurnThisFuckerBlue::UTurnThisFuckerBlue()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTurnThisFuckerBlue::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTurnThisFuckerBlue::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTurnThisFuckerBlue::turnItNow(){
	UStaticMeshComponent* rootComp = Cast<UStaticMeshComponent>(this->GetOwner()->GetRootComponent());
	UMaterialInstanceDynamic* blue = UMaterialInstanceDynamic::Create(rootComp->GetMaterial(0), this);
	// UTexture c;
	// blue.SetTextureParameterValue(TEXT("BLUEISH"), &c);
	// static ConstructorHelpers::FObjectFinder<UTexture2d>BarFillObj(TEXT("brol"));
	rootComp->SetMaterial(0, blue);//set material
	UnregisterComponent();
}
