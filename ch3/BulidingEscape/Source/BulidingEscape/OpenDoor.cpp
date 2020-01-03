// Its a demo app.


#include "OpenDoor.h"
#include <Engine/World.h>
#include <GameFramework/PlayerController.h>
#include "Components/PrimitiveComponent.h"

#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	World = GetWorld();

	check(Owner);
	check(World);

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("PressurePlate is null"));
	}
	//ActorThatOpens = World->GetFirstPlayerController()->GetPawn();
}


void UOpenDoor::openDoor()
{
	if (isDoorClosed())
	{
		doorOpening = true;
		doorClosing = false;
		TargetAngle = OpenedAngle;
	}
	if (BP_Enabled)
	{
		OnOpenRequest.Broadcast();
	}

	LastOpenTime = World->GetTimeSeconds();

}
void UOpenDoor::closeDoor()
{
	if (!isDoorClosed())
	{
		doorOpening = false;
		doorClosing = true;
		TargetAngle = ClosedAngle;
	}

	if (BP_Enabled)
	{
		OnCloseRequest.Broadcast();
	}
}

bool UOpenDoor::isDoorClosed()
{
	return isTargetClose(ClosedAngle);
}

bool UOpenDoor::isDoorOpened()
{
	return isTargetClose(OpenedAngle);
}

bool UOpenDoor::isTargetClose()
{
	return isTargetClose(TargetAngle);
}

bool UOpenDoor::isPlateTiggered()
{
	//Get total mass of actors on plate
	//
	return GetTotalMassOfActorsOnPlate() > TriggerMass;
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{

	//Find all overlapping actors
	//iterate through them adding their masses
	TArray<AActor*> overlappingActors;
	PressurePlate->GetOverlappingActors(overlappingActors);
	float totalMass = 0;
	for (const auto a : overlappingActors)
	{
		auto primitive = a->FindComponentByClass<UPrimitiveComponent>();
		//UE_LOG(LogTemp, Warning, TEXT("Overlapped by %s"), *a->GetName());
		totalMass += primitive->GetMass();
	}

	//UE_LOG(LogTemp, Warning, TEXT("Total Mass %f"), totalMass);
	return totalMass;

}

bool UOpenDoor::isTargetClose(float target)
{	
	return FMath::IsNearlyEqual(RelativeRotation.Yaw, target, DoorMinErrorAngle);
}

bool UOpenDoor::rotateDoor(float deltaAngle)
{

	//if we are close just return
	if (isTargetClose())
	{
		return false;
	}

	RelativeRotation.Add(0, deltaAngle, 0);
	RelativeRotation.Normalize();

	Owner->SetActorRelativeRotation(RelativeRotation);

	//if this move finished the rotation, mark as done
	if (isTargetClose())
	{
		doorOpening = false;
		doorClosing = false;
		return false;
	}

	return true;
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!PressurePlate)
	{
		return;
	}

	if (BP_Enabled)
	{
		if (isPlateTiggered())
		{
			openDoor();
		}
		else
		{
			closeDoor();
		}
	}
	else
	{
		//Poll trigger
		//if the actor is in the volume open door.
		//UE_LOG(LogTemp, Warning, TEXT("Door trigger is %d by %s"), isPlateTiggered(), *ActorThatOpens->GetName());
		if (isPlateTiggered())
		{
			openDoor();
		}
		else if (CloseDelaySeconds + LastOpenTime < World->GetTimeSeconds())
		{
			closeDoor();
		}

		AnimateDoor(DeltaTime);
	}
	

	//FlapDoor();
	
}

void UOpenDoor::FlapDoor()
{
	if (isDoorClosed())
	{
		UE_LOG(LogTemp, Display, TEXT("Setting Door Opening"));
		openDoor();
	}

	if (isDoorOpened())
	{
		UE_LOG(LogTemp, Display, TEXT("Setting Door Closing"));
		closeDoor();
	}
}

void UOpenDoor::AnimateDoor(float DeltaTime)
{
	float deltaAngle = 0;
	float direction = OpenClockwise ? 1.0 : -1.0;

	if (doorOpening)
	{
		deltaAngle = DoorOpenRateSec * DeltaTime * direction;
		UE_LOG(LogTemp, Verbose, TEXT("Opening...%f"), deltaAngle);
	}
	else if (doorClosing)
	{
		deltaAngle = DoorCloseRateSec * DeltaTime * -direction;
		UE_LOG(LogTemp, Verbose, TEXT("Closing...%f"), deltaAngle);
	}

	if (doorClosing || doorOpening)
	{
		rotateDoor(deltaAngle);
	}
}

