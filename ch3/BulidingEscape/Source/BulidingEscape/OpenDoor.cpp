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

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("PressurePlate is null"));
	}
}


void UOpenDoor::openDoor()
{
	OnOpenRequest.Broadcast();
}
void UOpenDoor::closeDoor()
{
	OnCloseRequest.Broadcast();

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
	for (const auto &a : overlappingActors)
	{
		auto primitive = a->FindComponentByClass<UPrimitiveComponent>();
		//UE_LOG(LogTemp, Warning, TEXT("Overlapped by %s"), *a->GetName());
		totalMass += primitive->GetMass();
	}

	//UE_LOG(LogTemp, Warning, TEXT("Total Mass %f"), totalMass);
	return totalMass;

}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (isPlateTiggered())
	{
		openDoor();
	}
	else
	{
		closeDoor();
	}
}
