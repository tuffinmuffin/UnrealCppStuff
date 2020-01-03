// Its a demo app.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenRequest);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseRequest);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BULIDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	bool rotateDoor(float deltaAngle);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void FlapDoor();
	void AnimateDoor(float DeltaTime);
	void openDoor();
	void closeDoor();
	bool isDoorClosed();
	bool isDoorOpened();
	bool isTargetClose(float target);
	bool isTargetClose();
	bool isPlateTiggered();
	float GetTotalMassOfActorsOnPlate();

private:
	//user config params
	UPROPERTY(EditAnywhere)
	float OpenedAngle = 90.0f;
	UPROPERTY(EditAnywhere)
	float ClosedAngle = 0.0f;
	UPROPERTY(EditAnywhere)
	float DoorOpenRateSec = 45.0f;
	UPROPERTY(EditAnywhere)
	float DoorCloseRateSec = 75.0f;
	UPROPERTY(EditAnywhere)
	bool OpenClockwise;
	UPROPERTY(EditAnywhere)
	bool BP_Enabled = false;
	
	//Trigger items
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;
	
	UPROPERTY(EditAnywhere)
	float TriggerMass = 60;

	UPROPERTY(EditAnywhere)
	float CloseDelaySeconds = 1.0;

	UPROPERTY(BlueprintAssignable)
	FOnOpenRequest OnOpenRequest;

	UPROPERTY(BlueprintAssignable)
	FOnCloseRequest OnCloseRequest;
	

	//door rotation
	bool doorOpening;
	bool doorClosing;
	float TargetAngle = ClosedAngle;
	FRotator RelativeRotation;
	float LastOpenTime;
	

	//house keeping values
	const float DoorDirection = 1.0;
	const float DoorMinErrorAngle = 2.0f;
	AActor* Owner;
	UWorld* World;


};
