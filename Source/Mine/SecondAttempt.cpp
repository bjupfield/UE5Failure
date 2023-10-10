// Fill out your copyright notice in the Description page of Project Settings.


#include "SecondAttempt.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ToTurnBlue.h"
#include "../../Engine/Source/Runtime/Engine/Public/Rendering/Texture2DResource.h"
#include "Mine.h"
// Sets default values
#define texture_Width 512
#define texture_Height 512
#define circle_Radius 5
ASecondAttempt::ASecondAttempt()//not on create function, instead something like on build game that applies to all classes before the instance somehow
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;
	// no ticking from the guy again

	box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(box);//this sets the root component of the object to the box, remember reootComponents
	// are the parent components of all others

	box->SetCollisionProfileName(TEXT("Player"));//this sets the collisionprofile, im attempting to set it too my custom profile of player

	body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	body->SetupAttachment(box);//this attaches a component to another

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(box);

	movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	moveAdjust = 1.0f;

	rotateAdjust = 50.0f;
	// LowLevelRaycast;


	//bresenham assignment

	int x = circle_Radius;
	int y = 0;
	int d = 1 - circle_Radius;
	TArray<FIntVector2> circleOctant;//one eighth of a circle, the +y axis adjacent +x piece

	FIntVector2 textVec = {x, y};
	// UE_LOG(MineLog, Log, TEXT("HERE IT IS: %d"), textVec.X);
	
	// circleOctant.Add(textVec);//error
	while(x > y){
		
		y++;

		if(d <= 0)
		{//no adjustment to y, so in the cricle its drawing the next point (x-1, y) from the previous one
			// y--;
			d += + 2 * y + 1;
		}
		else
		{//adjusting y so the circle is drawing the next point being (x-1, y+1) from the previous one
			x--;
			d += 2 * (y + x) + 1;
		}
		if(x < y){break;}
		textVec.X = x;
		textVec.Y = y;
		// UE_LOG(MineLog, Log, TEXT("Why does this neeed to be done: %d || %d"), x, y)
		circleOctant.AddUnique(textVec);//error
	}

	for(auto& thing : circleOctant)
	{
		// UE_LOG(MineLog, Warning, TEXT("Canned Ham: (%d, %d)"), thing.X, thing.Y);
	}


	for(auto& point : circleOctant){//x y not flipped
		textVec.X = point.X;
		textVec.Y = point.Y;
		bresenhamCircle.Add(textVec);
	}
	for(auto& point : circleOctant){//x y not flipped
		textVec.X = point.X * -1;
		textVec.Y = point.Y * -1;
		bresenhamCircle.Add(textVec);
	}
	for(auto& point : circleOctant){//x y not flipped
		textVec.X = point.X * -1;
		textVec.Y = point.Y * 1;
		bresenhamCircle.Add(textVec);
	}
	for(auto& point : circleOctant){//x y not flipped
		textVec.X = point.X * 1;
		textVec.Y = point.Y * -1;
		bresenhamCircle.Add(textVec);
	}
	for(auto& point : circleOctant){//x y flipped
		textVec.Y = point.X;
		textVec.X = point.Y;
		bresenhamCircle.Add(textVec);
	}
	for(auto& point : circleOctant){//x y flipped
		textVec.Y = point.X * -1;
		textVec.X = point.Y * -1;
		bresenhamCircle.Add(textVec);
	}
	for(auto& point : circleOctant){//x y flipped
		textVec.Y = point.X * -1;
		textVec.X = point.Y * 1;
		bresenhamCircle.Add(textVec);
	}
	for(auto& point : circleOctant){//x y flipped
		textVec.Y = point.X * 1;
		textVec.X = point.Y * -1;
		bresenhamCircle.Add(textVec);
	}
	textVec.Y = circle_Radius;
	textVec.X = 0;
	bresenhamCircle.Add(textVec);
	textVec.Y = 0;
	textVec.X = circle_Radius;
	bresenhamCircle.Add(textVec);
	textVec.Y = -circle_Radius;
	textVec.X = 0;
	bresenhamCircle.Add(textVec);
	textVec.Y = 0;
	textVec.X = -circle_Radius;
	bresenhamCircle.Add(textVec);
	

	
	bresenhamCircle.Sort([](const FIntVector2 point1, const FIntVector2 point2){//this is the basic syntax of the array sort, im sorting it by y layer and then by x in those layers
		int y1 = point2.Y;
		int y2 = point1.Y;
		if(y1 > y2){
			// UE_LOG(MineLog, Log, TEXT("This Y: %d || should be greater than this Y: %d"), y1, y2);
			return false;//point2 y greater than other y return true
		}
		else if(point2.Y == point1.Y){//ys are equal sort by x
			int x1 = point1.X;
			int x2 = point2.X;
			if(x2 < x1){//x starts at -radius and goes to + radius
				// UE_LOG(MineLog, Log, TEXT("This X: %d || should be less than this X: %d"), x2, x1);
				return false;//point 2s x is greater than point 1 return true
			}
		}
		return true;// point1 is ahead of point 2 like it should be
	});

	// y layers = diameter of circle
	for(int i = circle_Radius - 1; i > -circle_Radius; i--)
	{//minus one so it doesnt iterate over the first or last layer
		TArray<FIntVector2> yLayer = bresenhamCircle.FilterByPredicate([&](const FIntVector2 point){//the & means the filter will take in the variables in the given context, so I can use i as a variable inside the predicate
			return point.Y == i;
		});
		// UE_LOG(MineLog, Warning, TEXT("MY EYES ARE DRY:"), i);
		int indexToAddNumOg = yLayer.Num() / 2;
		int indexToAdd = indexToAddNumOg + (int)bresenhamCircle.Find(yLayer[0]);//error //add first point on that y layers index to the total point divided by two to give us the position of the first y value on the right side
		//of the gap
		int xOfRightSidePoint = yLayer[indexToAddNumOg].X;
		int xOfLeftSidePoint = yLayer[indexToAddNumOg - 1].X;
		TArray<FIntVector2> bufferT;//make buffer because it was misinterpretating my insert function
		while(xOfRightSidePoint > xOfLeftSidePoint){
			xOfRightSidePoint--;
			// UE_LOG(MineLog, Warning, TEXT("PLZ FILL MY SCREEN"));
			// int myNewInt[2] = {xOfRightSidePoint, i};
			textVec.X = xOfRightSidePoint;
			textVec.Y = i;
			bufferT.Add(textVec);//fills in the circle by adding the points inbetween the edges
		}
		bresenhamCircle.Insert(bufferT, indexToAdd);
	}
	for(auto& thing : bresenhamCircle)
	{
		// UE_LOG(MineLog, Warning, TEXT("OPEN UP FBI: (%d, %d)"), thing.X, thing.Y);
	}
}

// Called when the game starts or when spawned
// void ASecondAttempt::BeginPlay()
// {
// 	Super::BeginPlay();
	
// }

// Called every frame
// void ASecondAttempt::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);

// }

// Called to bind functionality to input
void ASecondAttempt::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	EIcomp = Cast<UEnhancedInputComponent>(PlayerInputComponent);//convert input to enhanced
	Pcontr = Cast<AToTurnBlue>(Controller);//i guess this converts the controller to the player controller, but i thought that was already done

	check(EIcomp && Pcontr);//this checks to seee if they exist
	EIcomp->BindAction(Pcontr->MoveInput, ETriggerEvent::Triggered, this, &ASecondAttempt::move);
	EIcomp->BindAction(Pcontr->RotateInput, ETriggerEvent::Triggered, this, &ASecondAttempt::rotate);
	EIcomp->BindAction(Pcontr->TurnBlueMotherFucker, ETriggerEvent::Triggered, this, &ASecondAttempt::turnItBlue);
	EIcomp->BindAction(Pcontr->ChangeInput, ETriggerEvent::Triggered, this, &ASecondAttempt::changeMapAndCamera);
	EIcomp->BindAction(Pcontr->MouseDown, ETriggerEvent::Triggered, this, &ASecondAttempt::mouseDownTrack);

	//below adds the mapping context
	ULocalPlayer* localPlayer = Pcontr->GetLocalPlayer();//this grabs the player, check up on this func later
	check(localPlayer);//also check on check to see if it does something like just break when it fails
	UEnhancedInputLocalPlayerSubsystem* subSystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();//check on the subsystem, but its grabbing the subsystem
	check(subSystem);
	subSystem->ClearAllMappings();//this clears previous mappingcontext, so you can set the ones created in the pawn controller
	subSystem->AddMappingContext(Pcontr->theMappingContext, 0);//sets mapping context to priority 0, which is first on the list

	ogRelCamTransLoc = this->camera->GetRelativeTransform().GetLocation();
	ogRelCamTransRot = this->camera->GetRelativeTransform().GetRotation();

	// UE_LOG(MineLog, Warning, TEXT("Camera Transform is: X%1f || Y%2f || Z%3f"), ogRelCamTransLoc.X, ogRelCamTransLoc.Y, ogRelCamTransLoc.Z);
	// UE_LOG(MineLog, Warning, TEXT("Camera Rotation is: X%1f || Y%2f || Z%3f || W%4f"), ogRelCamTransRot.X, ogRelCamTransRot.Y, ogRelCamTransRot.Z, ogRelCamTransRot.W);
}

void ASecondAttempt::move(const struct FInputActionValue& actionValue)
{
	FVector input = actionValue.Get<FInputActionValue::Axis3D>();
	AddMovementInput(GetActorRotation().RotateVector(input), moveAdjust);//sends the movement to the pawn, but first adjust it by rotation
}

void ASecondAttempt::rotate(const struct FInputActionValue& actionValue)
{
	FRotator Input(actionValue[0], actionValue[1], actionValue[2]);
	Input *= GetWorld()->GetDeltaSeconds() * rotateAdjust;
	Input += GetActorRotation();
	Input.Pitch = FMath::ClampAngle(Input.Pitch, -89.9f, 89.9f);
	Input.Roll = 0;
	SetActorRotation(Input);
}

void ASecondAttempt::turnItBlue(const struct FInputActionValue& actionValue)
{
	AActor* hitActor = nullptr;
	if(this->rayCastCheckForActor(hitActor, true))
	{
		UStaticMeshComponent* mesh;
		mesh = Cast<UStaticMeshComponent>(hitActor->GetRootComponent());
		// b.GetActor()->;
		if(mesh != nullptr){
			if(mesh->GetMaterial(0) == material){
				mesh->SetMaterial(0, strToMatMap[*(hitActor->GetName())]);
			}
			else{
				if(!strToMatMap.Contains(*(hitActor->GetName())))
				{
					UE_LOG(MineLog, Log, TEXT("HAPPENS ONCE"));
					strToMatMap.Add({*(hitActor->GetName()), mesh->GetMaterial(0)});
				}
				mesh->SetMaterial(0, material);
			}
		}
		// mesh.SetMaterial()
	}
	else
	{
		UE_LOG(MineLog, Log, TEXT("FAILED || HIT RECIEVED"));
	}
}

void ASecondAttempt::changeMapAndCamera(const struct FInputActionValue& actionValue)
{
	
	UE_LOG(MineLog, Log, TEXT("Inside Change Map and Camera"));

	ULocalPlayer* localPlayer = Pcontr->GetLocalPlayer();
	check(localPlayer);

	UEnhancedInputLocalPlayerSubsystem* subSystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(subSystem);

	if(subSystem->HasMappingContext(Pcontr->theMappingContext)){

		AActor* targetObject = nullptr;
		if(ASecondAttempt::rayCastCheckForActor(targetObject, true))
		{

			TArray<FName> tagNames = targetObject->Tags;
			for(int i = 0; i < tagNames.Num(); i++)
			{
				UE_LOG(MineLog, Log, TEXT("TAG IS CALLED: %s"), *(tagNames[i].ToString()))
			}
			if(targetObject->Tags.Contains(TEXT("RayHit.table")))
			{
				UE_LOG(MineLog, Log, TEXT("Has Move Map"));
				subSystem->RemoveMappingContext(Pcontr->theMappingContext);
				subSystem->AddMappingContext(Pcontr->drawingMapping, 0);
				// we change the mapping from move map to the draw map
				//this needs to check what the current mapping context is

				moveCameraAboveObject(targetObject);
			}
			else
			{
				UE_LOG(MineLog, Log, TEXT("Did not hit raycast.table"));
			}
		}
		else{
			UE_LOG(MineLog, Warning, TEXT("No Target Hit"));
		}
	}
	else if(subSystem->HasMappingContext(Pcontr->drawingMapping))
	{
		UE_LOG(MineLog, Log, TEXT("Has Draw Map"));
		subSystem->RemoveMappingContext(Pcontr->drawingMapping);
		subSystem->AddMappingContext(Pcontr->theMappingContext, 0);
		// we change the mapping from draw map to the move map
		moveCameraBack();
	}
	else
	{
		UE_LOG(MineLog, Warning, TEXT("HAS FAILED SOMEHOW"));
	}

}

void ASecondAttempt::moveCameraAboveObject(AActor* targetObject)
{
	UE_LOG(MineLog, Warning, TEXT("ACTORS NAME IS: %s"), *(targetObject->GetName()))

	FVector targetObjectPos = targetObject->GetActorLocation();
	
	FQuat playQuat = this->GetActorTransform().GetRotation();//quaternion

	camera->SetWorldRotation(playQuat.MakeFromEuler(FVector{0, -90, 0}));
	camera->SetWorldLocation(FVector{targetObjectPos.X, targetObjectPos.Y, targetObjectPos.Z + 300});
	//im dumb lets just do this it does it for us, probably with better math or code anyway
	// FVector cameraRot = camera->GetRelativeTransform().GetRotation().Euler();

	Pcontr->SetShowMouseCursor(true);

	// UE_LOG(MineLog, Warning, TEXT("Rotation after SETTING IS: X %1f || Y: %2f || Z: %3f"), playRot.X, playRot.Y, playRot.Z);
}

void ASecondAttempt::moveCameraBack(){
	// this->camera->GetRelativeTransform().SetRotation(ogRelCamTransRot);
	// this->camera->GetRelativeTransform().SetLocation(ogRelCamTransLoc);
	camera->SetRelativeLocation(ogRelCamTransLoc);
	camera->SetRelativeRotation(ogRelCamTransRot);

	Pcontr->SetShowMouseCursor(false);
	

	
}

void ASecondAttempt::mouseDownTrack(const struct FInputActionValue& actionValue){

	UE_LOG(MineLog, Log, TEXT("This is the Mouse Pos: X: %1f || Y: %2f"), Pcontr->mouseX, Pcontr->mouseY);

	FVector mouseStart, mouseDirection;

	// mouseDirection *= 3000;

	Pcontr->DeprojectMousePositionToWorld(mouseStart, mouseDirection);

	mouseDirection *= 500;
	mouseDirection += mouseStart;

	UE_LOG(MineLog, Warning, TEXT("This is the Mouse Pos: MouseStart: X: %1f || Y: %2f || Z: %3f ||| MouseDirection: X: %4f || Y: %5f || Z: %6f"), mouseStart.X, mouseStart.Y, mouseStart.Z, mouseDirection.X, mouseDirection.Y, mouseDirection.Z);

	DrawDebugLine(GetWorld(), mouseStart, mouseDirection, FColor::Purple, false, 2.0f);

	FHitResult hitResult;

	FCollisionQueryParams traceParams(FName(TEXT("")), false, GetOwner());

	FGenericPhysicsInterface::RaycastSingle(GetWorld(), OUT hitResult, mouseStart, mouseDirection, ECollisionChannel::ECC_GameTraceChannel1, traceParams, FCollisionResponseParams::DefaultResponseParam);

	if (&hitResult != nullptr)
	{



		if(hitResult.GetActor() != nullptr)
		{
			//it hit something
			AActor* hitActor = hitResult.GetActor();

			UStaticMeshComponent* mesh;
			mesh = Cast<UStaticMeshComponent>(hitActor->GetRootComponent());
			
			FVector2D hitPoint = {hitResult.ImpactPoint.X, hitResult.ImpactPoint.Y};
			hitPoint.X = hitPoint.X - hitActor->GetActorLocation().X;
			hitPoint.Y = hitPoint.Y - hitActor->GetActorLocation().Y;
			
			if(mesh != nullptr){//it is a staticmesh
				UMaterialInstanceDynamic* mat2 = Cast<UMaterialInstanceDynamic>(mesh->GetMaterial(0));
				// mesh->SetMaterial(0, mat2);
				if(mat2 != nullptr){
					UE_LOG(MineLog, Log, TEXT("NAME OF MAT IS: %s"), *(mat2->GetName()));
					if((mat2->GetName()) == TEXT("PaintSmhAttempt1Inst_Dynamic"))
					{
						UE_LOG(MineLog, Log, TEXT("SUCCESS!!!"));


						UTexture* custyTexture; 

						// mat2->GetAllParametersOfType(EMaterialParameterType::Texture)
						mat2->GetTextureParameterValue(TEXT("TextureParam"), OUT custyTexture);
						UE_LOG(MineLog, Warning, TEXT("TexturesParamName is: %s"), *(custyTexture->GetName()));

						UTexture2D* castedTexture = Cast<UTexture2D>(custyTexture);
						if(castedTexture != nullptr)
						{
							UE_LOG(MineLog, Warning, TEXT("TexturesParamName is: %s"), *(custyTexture->GetName()));

							// TArray<ptr> mipData;

							// castedTexture->GetMipData(0, OUT mipData);

							FTexturePlatformData* castData = castedTexture->GetPlatformData();
							if(castData != nullptr)
							{
								int mipNums = castData->GetNumMipsInTail();
								int mipNum = castData->Mips.Num();
								UE_LOG(MineLog, Warning, TEXT("Number of Mip Maps: %d"), mipNum);
								if(mipNum == 1)
								{//single Mip
									UE_LOG(MineLog, Log, TEXT("TRIGGERS"));
									// castedTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
									// castedTexture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
									// castedTexture->SRGB = false;
									// castedTexture->UpdateResource();

									FTexture2DMipMap mipMap = castData->Mips[0];
									int mipSizeX = mipMap.SizeX;
									UE_LOG(MineLog, Log, TEXT("MipSizeX: %d"), mipSizeX);

									int dataSize = mipMap.BulkData.GetBulkDataSize();
									int elements = mipMap.BulkData.GetElementCount();
									UE_LOG(MineLog, Log, TEXT("Mip Data SIZE: %d"), dataSize);
									UE_LOG(MineLog, Log, TEXT("Mip ELEMENT SIZE: %d"), elements);

									if(mipMap.BulkData.IsUnlocked())
									{
										// this.bre

										uint8* mipColor = NULL;
										mipMap.BulkData.GetCopy((void**)&mipColor);
										
										// for(int i = 0; i < 16; i++){
										// 	int multiplied = i * 4;
										// 	UE_LOG(MineLog, Warning, TEXT("First 16 Colors, B: %d || G: %d || R: %d || A: %d"), mipColor[0 + multiplied], mipColor[1 + multiplied], mipColor[2 + multiplied], mipColor[3 + multiplied])
										// }

										UE_LOG(MineLog, Log, TEXT("HitPoint is: X: %d || Y: %d"), (int)hitPoint.X, (int)hitPoint.Y);

										int realY = ((hitPoint.Y + 50) / 100 * texture_Height);//+50 to make center equal to 50, divide by 100 to make the point equal to one, multiply by size to get location
										if(realY >= texture_Height)
										{
											realY = texture_Height - 1;
										}
										int realX = ((hitPoint.X + 50) / 100 * texture_Width);//+50 to make center equal to 50, divide by 100 to make the point equal to one, multiply by size to get location
										if(realX >= texture_Width)
										{
											realX = texture_Width - 1;
										}

										float floatX = ((float)hitPoint.X + 50.000000f) / 100.000000f;
										float floatY = ((float)hitPoint.Y + 50.000000f) / 100.000000f;

										UE_LOG(MineLog, Warning, TEXT("Hitpoint Interpreted Float Location is : X: %f || Y: %f"), floatX, floatY);

										UE_LOG(MineLog, Warning, TEXT("Hitpoint Interpreted Int Location is : X: %d || Y: %d"), realX, realY);

										// for(int i = 0; i < 4; i++)
										// {
										// 	int point = ((realY * texture_Width) + realX) * 4;//multiply twice because its stored in a single array where every layer is + x width for y increases
											
										// 	mipColor[point + i] = 255;
											
										// }
										for(auto& circlePoint : bresenhamCircle)
										{
											int pointY = realY + circlePoint.Y;
											int pointX = realX + circlePoint.X;
											if(pointX >= 0 && pointY >= 0 && pointY <= texture_Height && pointX <= texture_Width){
												UE_LOG(MineLog, Warning, TEXT("LOTS OF POINTS YAH RIGHT GIT: X: %d || Y: %d"), pointX, pointY);
												int point = ((pointY * texture_Width) + pointX) * 4;//multiply twice because its stored in a single array where every layer is + x width for y increases
												mipColor[point + 1] = 255;//one might mean green
											}
											
										}


										void* TextureData = castedTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);// this allows me to write to the map
										FMemory::Memcpy(TextureData, mipColor, dataSize);// this copie
										castedTexture->GetPlatformData()->Mips[0].BulkData.Unlock();//this closes the write
										castedTexture->UpdateResource();//this actually updates it based of the memcopy 
										

									}

									//for all this fancy nonsense refer to
									//https://www.orfeasel.com/generating-procedural-textures/
									// https://forums.unrealengine.com/t/setting-texture-mipgensettings-to-nomipmaps-at-runtime/143749
								}
								else if(mipNum == 8)
								{
									FTexture2DMipMap mipMap = castData->Mips[0];
									int mipSizeX = mipMap.SizeX;
									UE_LOG(MineLog, Log, TEXT("MipSizeX: %d"), mipSizeX);

									// mipMap.BulkData.Lock(LOCK_READ_WRITE);
									int dataSize = mipMap.BulkData.GetBulkDataSize();
									int elements = mipMap.BulkData.GetElementCount();
									UE_LOG(MineLog, Log, TEXT("Mip Data SIZE: %d"), dataSize);
									UE_LOG(MineLog, Log, TEXT("Mip ELEMENT SIZE: %d"), elements);
								}	


							}
							else
							{
								UE_LOG(MineLog, Warning, TEXT("ITS NULL"));
							}

						}

					}
				}
				else
				{//mat cant be converted into dynamic must not be dynamic
					if((mesh->GetMaterial(0)->GetName()) == TEXT("PaintSmhAttempt1"))
					{
						mat2 = UMaterialInstanceDynamic::Create(mesh->GetMaterial(0), this);
						mat2->Rename(TEXT("PaintSmhAttempt1Inst_Dynamic"));
						mesh->SetMaterial(0, mat2);

						UTexture* custyTexture; 

						// mat2->GetAllParametersOfType(EMaterialParameterType::Texture)
						mat2->GetTextureParameterValue(TEXT("TextureParam"), OUT custyTexture);
						
						UTexture2D* b = UTexture2D::CreateTransient(texture_Width, texture_Height, EPixelFormat::PF_B8G8R8A8, TEXT("My_Texture"));
						b->SRGB = 0;
						b->MipGenSettings = TMGS_NoMipmaps;//makes it writtable too, and starts with no mips
						//not only does this make it writtable I can now write to it in the future!!!

						TArray<FColor> Pixels;
						Pixels.Init(FColor(0, 0, 0, 255), texture_Width * texture_Height);
						//initiates the tarray with the 

						void* textureMipData = b->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
						FMemory::Memcpy(textureMipData, Pixels.GetData(), Pixels.Num() * sizeof(FColor));
						b->GetPlatformData()->Mips[0].BulkData.Unlock();
						b->UpdateResource();
						

						// b->Rename(TEXT("My_Texture"), this);
						mat2->SetTextureParameterValue(TEXT("TextureParam"), b);
					}
					else
					{
						UE_LOG(MineLog, Warning, TEXT("ERROR NOT RIGHT TEXTURE"));
					}



					// I WILL BE IGNORING THE FIRST INPUT FOR NOW I DO NOT CARE RIGHT NOW
				}
			}
		}
	}
	else{
		for(auto& points : bresenhamCircle){
			UE_LOG(MineLog, Warning, TEXT("X: %d || Y: %d"), points[0], points[1]);
		}
	}

}

bool ASecondAttempt::rayCastCheckForActor(AActor*& actorRef, bool drawDebug,//passing reference to a pointer... lol... 
bool pos, FVector startPos, FVector endPos 
)
{//okay just to reiterate & means the value is a reference, so it is modifiable and is the actual value not just a copy, while * meams it is a pointer which is the location of the value
	if(!pos){
		startPos = box->GetOwner()->GetActorLocation();
		FVector charForV = box->GetOwner()->GetActorForwardVector();
		endPos = startPos + charForV * 500;
	}

	FHitResult hitResult;
	
	FCollisionQueryParams traceParams(FName(TEXT("")), false, GetOwner());

	FGenericPhysicsInterface::RaycastSingle(GetWorld(), OUT hitResult, startPos, endPos, ECollisionChannel::ECC_GameTraceChannel1, traceParams, FCollisionResponseParams::DefaultResponseParam);

	if(drawDebug){
		DrawDebugLine(GetWorld(), startPos, endPos, FColor::Green, false, 2.0f);
	}

	if (&hitResult != nullptr)
	{
		if(hitResult.GetActor() != nullptr)
		{
			actorRef = hitResult.GetActor();
			if(drawDebug){
				UE_LOG(MineLog, Warning, TEXT("THIS IS THE NAME OF THE COMPONENT THE OBJECT SEES: %s"), *(actorRef->GetName()));//to use a minelog or any custom log you need to include the game.h file, in this case the mine.h file
			}
			return true;
		}
	}
	return false;
}