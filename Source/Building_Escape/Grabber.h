// Copyright My Bridges 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDING_ESCAPE_API UGrabber : public UActorComponent
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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:
	float Reach = 150.f;
	UPROPERTY()
	UPhysicsHandleComponent *PhysicsHandle = nullptr;
	UPROPERTY()
	UInputComponent *InputComponent = nullptr;

	void Grab();
	void Release();
	void FindPhysicsHandle();
	void SetupInputComponent();

	// return first actor whithin reach of physics body
	FHitResult GetPhysicsBodyInReach() const;

	// return line trace end
	FVector GetPlayerReach() const;

	// get players location in world
	FVector GetPlayerWorldPosition() const;
};
