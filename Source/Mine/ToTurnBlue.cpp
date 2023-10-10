// Fill out your copyright notice in the Description page of Project Settings.


#include "ToTurnBlue.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "InputMappingContext.h"

static void mapKey(UInputMappingContext* inputMap, UInputAction* inputAction, FKey key, bool negate = false, bool swizzle = false, EInputAxisSwizzle swizzleOrder = EInputAxisSwizzle::YXZ);

AToTurnBlue::AToTurnBlue(){
    // UInputComponent newComp = AToTurnBlue::SetupInputComponent();
}

void AToTurnBlue::BeginPlay(){
    Super::BeginPlay();
    
    FString hope = AToTurnBlue::GetPawnOrSpectator()->GetName();
    UE_LOG(LogTemp, Warning, TEXT("This is the Pawn NAME: %s"), *hope);
    // TArray<UInputComponent> b = AToTurnBlue::CurrentInputStack;
    // UInputComponent b = AToTurnBlue::CurrentInputStack[0].Get();
    int32 num = AToTurnBlue::CurrentInputStack.Num();
    UE_LOG(LogTemp, Warning, TEXT("NUM = %d"), num);
    UE_LOG(LogTemp, Log, TEXT("Inside BeginPLay InputComponentName: %s"), *this->InputComponent.GetName())
    for(auto& Input : AToTurnBlue::CurrentInputStack){
        FString b = Input.Get()->GetName();
        UEnhancedInputComponent* c = Cast<UEnhancedInputComponent>(Input.Get());
        // int d = c->GetActionValueBindings().Num();
        int l = Input.Get()->GetNumActionBindings();
        UE_LOG(LogTemp, Warning, TEXT("Number of Action Bindings: %d"), l);
        //  AToTurnBlue::SetupInputComponent();
        // char help = d;
        // printf("The Number should be: %c",d);
        // int abc = *d;
        // for(int i = 0; i < d; i++){
        //     UE_LOG(LogTemp, Warning, TEXT("Number of ActionBindings: %d"), i);
            // FInputActionBinding e = c->GetActionBinding(i);
            // FString actionName = e.GetActionName().ToString();
            // UE_LOG(LogTemp, Warning, TEXT("This is the ActionName: %s"), *actionName)
        // }
        UE_LOG(LogTemp, Warning, TEXT("This is the collection of Input Names: %s"), *b);
        Input.Get()->DestroyComponent();
    }
    // for(auto& Input : AToTurnBlue::CurrentInputStack){
    //     FString b = Input.Get()->GetName();
    //     // UE_LOG(LogTemp, Warning, TEXT("IS IT DELETED: %s"), *b);
    // }
    // AToTurnBlue::action
    FString enabler = AToTurnBlue::InputEnabled() ? "Enabled" : "NotEnabled";
    UE_LOG(LogTemp, Warning, TEXT("%s"), *enabler);
}

void AToTurnBlue::SetupInputComponent(){
    // UE_LOG(LogTemp, Log, TEXT("MoveUp"));
    // UE_LOG(LogTemp,Warning, TEXT("Component Name is: %s"), *this->InputComponent.GetName());
    Super::SetupInputComponent();
    for(auto& Input : AToTurnBlue::CurrentInputStack){
        FString b = Input.Get()->GetName();
        UE_LOG(LogTemp, Warning, TEXT("InputComponenentName is: %s"), *b);

    }


    //normal mapping context
    theMappingContext = NewObject<UInputMappingContext>(this);

    MoveInput = NewObject<UInputAction>(this);
    MoveInput->ValueType = EInputActionValueType::Axis3D;//these set what type of action it is, a 3d, 2d, 1d, or boolean type
    mapKey(theMappingContext, MoveInput, EKeys::W);
    mapKey(theMappingContext, MoveInput, EKeys::S, true);
    mapKey(theMappingContext, MoveInput, EKeys::A, true, true);
    mapKey(theMappingContext, MoveInput, EKeys::D, false, true);
    mapKey(theMappingContext, MoveInput, EKeys::SpaceBar, false, true, EInputAxisSwizzle::ZYX);
    mapKey(theMappingContext, MoveInput, EKeys::LeftShift, true, true, EInputAxisSwizzle::ZYX);

    RotateInput = NewObject<UInputAction>(this);
    RotateInput->ValueType = EInputActionValueType::Axis3D;
    mapKey(theMappingContext, RotateInput, EKeys::MouseY);
    mapKey(theMappingContext, RotateInput, EKeys::MouseX, false, true);

    TurnBlueMotherFucker = NewObject<UInputAction>(this);
    // TurnBlueMotherFucker->Triggers
    TurnBlueMotherFucker->Triggers.Add(NewObject<UInputTriggerPressed>(this));//this makes it where it only activates when pressed down, not when held or anything, you can add other triggers like this,
    //to look at the base triggers either go to definition of trigger above or create a blueprint input class and hover over the triggers in the trigger array selection and it will tell you the class names and what they do
    TurnBlueMotherFucker->ValueType = EInputActionValueType::Boolean;
    mapKey(theMappingContext, TurnBlueMotherFucker, EKeys::F);

    ChangeInput = NewObject<UInputAction>(this);
    ChangeInput->Triggers.Add(NewObject<UInputTriggerPressed>(this));
    ChangeInput->ValueType = EInputActionValueType::Boolean;
    mapKey(theMappingContext, ChangeInput, EKeys::E);

    //when drawing
    drawingMapping = NewObject<UInputMappingContext>(this);

    MouseDown = NewObject<UInputAction>(this);
    MouseDown->ValueType = EInputActionValueType::Boolean;
    mapKey(drawingMapping, MouseDown, EKeys::LeftMouseButton);

    mapKey(drawingMapping, ChangeInput, EKeys::E);
}
void AToTurnBlue::Tick(float DeltaTime){

    this->GetMousePosition(mouseX, mouseY);

}
void AToTurnBlue::turning(const FInputActionInstance& Instance){
    UE_LOG(LogTemp, Log, TEXT("MoveUp"));
    //logic to turn box blue
}

static void mapKey(UInputMappingContext* inputMap, UInputAction* inputAction, FKey key, 
    bool negate, 
    bool swizzle, 
    EInputAxisSwizzle swizzleOrder){
    
    FEnhancedActionKeyMapping& currentMap = inputMap->MapKey(inputAction, key);//assigns an action a key
    UObject* outer = inputMap->GetOuter();//this retrieves the outer object... neat

    if(negate){
        UInputModifierNegate* Negater = NewObject<UInputModifierNegate>(outer);//obviously retrieves the negate object
        currentMap.Modifiers.Add(Negater);//adds a modifier, in this case the negate modifier
    }
    if(swizzle){
        UInputModifierSwizzleAxis* theSwizzler = NewObject<UInputModifierSwizzleAxis>(outer);
        theSwizzler->Order = swizzleOrder;//hmmm sets order
        currentMap.Modifiers.Add(theSwizzler);
    }

}