// Its a demo app.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "Logging/LogMacros.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "Grabber.generated.h"

//General Log
DECLARE_LOG_CATEGORY_EXTERN(GrabberLog, Log, All);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BULIDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere)
	float Reach = 100.0f;
	UPROPERTY(EditAnywhere)
	bool EnableDebug = false;
	
	UInputComponent* InputHandle = nullptr;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	//Ray-cast and grab what is in reach
	void Grab();

	//Release a grabbed obj
	void Release();

	//Get ray-cast vector for grabbing
	void GetGrabberVector(FVector& start, FVector& end);

	//Draw debug line when debugging enabled
	void DrawDebugGrabber();

	void BindInputActions();
	void FindPhysicsHandleComponent();

};
