// Its a demo app.

#include <Engine/World.h>
#include <GameFramework/PlayerController.h>

#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"

#include "Grabber.h"

DEFINE_LOG_CATEGORY(GrabberLog);

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Get player view point
	FVector playerLocation;
	FRotator playerRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerLocation, playerRotation);

	//Draw a red trace
	FVector lineTraceEnd = playerLocation + playerRotation.Vector() * Reach;
	DrawDebugLine(
		GetWorld(),
		playerLocation,
		lineTraceEnd,
		FColor(0xFF0000),
		false,
		0.0f,
		0,
		5.0f
	);

	//UE_LOG(GrabberLog, Verbose, TEXT("Player Vector: %s"), *playerLocation.ToString());
	//UE_LOG(GrabberLog, Verbose, TEXT("Player Rotation: %s"), *playerRotation.ToString());
	//ray cast to reach distance


	//determine what is in range
}

