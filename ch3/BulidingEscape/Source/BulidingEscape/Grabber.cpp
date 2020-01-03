// Grabber class to manulpate world objects
#include "Grabber.h"

#include <Engine/World.h>
#include <GameFramework/PlayerController.h>
#include "Components/PrimitiveComponent.h"

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

	FindPhysicsHandleComponent();
	BindInputActions();
}

void UGrabber::BindInputActions()
{
	InputHandle = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!InputHandle)
	{
		UE_LOG(GrabberLog, Error, TEXT("UInputComponent not found on: %s"), *GetOwner()->GetName());
	}
	else
	{
		//Bind the input access
		InputHandle->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
		InputHandle->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(GrabberLog, Error, TEXT("UPhysicsHandleComponent not found on: %s"), *GetOwner()->GetName());
	}
}

void UGrabber::Release()
{
	UE_LOG(GrabberLog, Display, TEXT("Releasing..."));
	if (PhysicsHandle->GetGrabbedComponent() == nullptr)
	{
		UE_LOG(GrabberLog, Display, TEXT("Releasing...Nothing grabbed"));
		return;
	}

	UE_LOG(GrabberLog, Display, TEXT("Releasing: %s"), *PhysicsHandle->GetGrabbedComponent()->GetName());

	//release physics handle
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::Grab()
{
	if (PhysicsHandle->GetGrabbedComponent())
	{
		UE_LOG(GrabberLog, Warning, TEXT("%s is already grabbing. Not grabbing"), *PhysicsHandle->GetGrabbedComponent()->GetName());
	}
	FVector start, end;

	GetGrabberVector(start, end);
	
	FHitResult hitResult;
	FCollisionQueryParams traceParams(FName("Grabber"), false, GetOwner());
	//

	bool hitFound = GetWorld()->LineTraceSingleByObjectType(
		hitResult,
		start,
		end,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		traceParams
	);

	//nothing to grab
	if(!hitFound)
	{
		return;
	}

	//attach physics handle to grabbed object
	

	UE_LOG(GrabberLog, Display, TEXT("Grabbing: %s"), *hitResult.GetComponent()->GetName());
	
	//PhysicsHandle->GrabComponentAtLocation(hitResult.GetComponent(), NAME_None, hitResult.GetComponent()->GetOwner()->GetActorLocation());
	PhysicsHandle->GrabComponentAtLocation(hitResult.GetComponent(), NAME_None, hitResult.GetComponent()->GetCenterOfMass());
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (EnableDebug)
	{
		DrawDebugGrabber();
	}

	//If we are attached to an object, move it
	if (PhysicsHandle->GetGrabbedComponent())
	{
		FVector start, end;
		GetGrabberVector(start, end);
		PhysicsHandle->SetTargetLocation(end);
	}

}

void UGrabber::GetGrabberVector(FVector& start, FVector& end)
{
	FRotator playerRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(start, playerRotation);

	//Draw a red trace
	end = start + playerRotation.Vector() * Reach;
}

void UGrabber::DrawDebugGrabber()
{
	FVector start, end;
	GetGrabberVector(start, end);
	//Get player view point
	DrawDebugLine(
		GetWorld(),
		start,
		end,
		FColor(0xFF0000),
		false,
		0.0f,
		0,
		5.0f
	);
}

