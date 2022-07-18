// Copyright My Bridges 2021
#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts

void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	// check for physics handle component
	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Component Found for %s"), *GetOwner()->GetName());
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle Found on %s"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{

	// TODO only raycast when key is pressed and see if we reach any actor which has physics body collision channel set
	//  if we hit something attach physics handle
	//  todo attach physics handle
	FHitResult HitResult = GetPhysicsBodyInReach();
	UPrimitiveComponent *ComponentToGrab = HitResult.GetComponent();
    AActor* ActorHit=HitResult.GetActor();
	if (ActorHit)
	{
		 if(!PhysicsHandle){return;}
			PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			GetPlayerReach());
	}
}


void UGrabber::Release()
{
	// todo remove release physics handle
	if(!PhysicsHandle){return;}
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		// move the object we are holding
		if(!PhysicsHandle){return;}
		PhysicsHandle->SetTargetLocation(GetPlayerReach());
	}

	// UE_LOG(LogTemp, Warning, TEXT("Location:%s  Rotation:%s"), *PlayerViewpointLocation.ToString(), *PlayerViewpointRotation.ToString());
	// Draw a line from player to show direction

	// see what it hits
}

FHitResult UGrabber::GetPhysicsBodyInReach() const
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	// ray cast out to a distance (reach)
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerWorldPosition(),
		GetPlayerReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams);
	return Hit;	
}

FVector UGrabber::GetPlayerWorldPosition() const
{
	//  Take the players viewpoint
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation,
		OUT PlayerViewpointRotation);
	return PlayerViewpointLocation;
}

FVector UGrabber::GetPlayerReach() const
{
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointLocation,
		OUT PlayerViewpointRotation);
	return PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach;
}
// draws debug line
//  DrawDebugLine(
//  	GetWorld(),
//  	PlayerViewpointLocation,
//  	LineTraceEnd,
//  	FColor(0, 255, 0), // rgb
//  	false,
//  	0.f,
//  	0,
//  	5.f // bold
//  );
