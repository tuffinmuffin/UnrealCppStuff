// Grabber class to manulpate world objects
#include "Grabber.h"

#include <Engine/World.h>
#include <GameFramework/PlayerController.h>

#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"

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
	//ray cast (line trace) to reach distance

	FHitResult hitResult;
	FCollisionQueryParams traceParams(FName("Grabber"), false, GetOwner());
	FCollisionObjectQueryParams
	//

	bool hitFound = GetWorld()->LineTraceSingleByObjectType(
		hitResult,
		playerLocation,
		lineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		traceParams
		);

	if (hitFound)
	{
		UE_LOG(GrabberLog, Warning, TEXT("Found Hit: %s"), *hitResult.GetActor()->GetName());
	}

	//determine what is in range
}

