// Copyright My Bridges 2021
#include "Engine/World.h"
#include "Components/AudioComponent.h"
#include "GameFramework/PlayerController.h"
#include "Opendoor.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UOpendoor::UOpendoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UOpendoor::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw; // now after adding uprop(editany) we can put in value in editor to add in initial value of target yaw

	FindPressurePlate();
	// ActorThatOpen = GetWorld()->GetFirstPlayerController()->GetPawn();
	FindAudioComponent();
}

void UOpendoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Audio component not found"), *GetOwner()->GetName());
	}
}

void UOpendoor::FindPressurePlate() const
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Character doesnt have pressure plate component on it"), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpendoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TotalMassOfActors() > MassToOpenDoor)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		// if door is open for X last second
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}
// Fratator
// Change yaw value of opendoor
// starting yaw target yaw and value between 0-1

// // float MyFloat = 90.f;
// FRotator Rotate(0.f,90.f,0.f);// reset
// // OpenDoor.Yaw = FMath::Lerp(CurrentYaw, TargetYaw, 0.06f);
// OpenDoor.Yaw = FMath::FInterpConstantTo(CurrentYaw, TargetYaw,DeltaTime,47);//means 47 deg per sec

// // FRotator CurrentRotation = GetOwner()->GetActorRotation();
// float CurrentYaw = GetOwner()->GetActorRotation().Yaw;
void UOpendoor::OpenDoor(float DeltaTime)
{

	// UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	// UE_LOG(LogTemp, Warning, TEXT("Yaw is %f"), GetOwner()->GetActorRotation().Yaw);

	CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, DeltaTime * DoorOpenSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;

	GetOwner()->SetActorRotation(DoorRotation);
	if (AudioComponent && OpenAudioPlay)
	{
		AudioComponent->Play();
		OpenAudioPlay = false;
	}
}

void UOpendoor::CloseDoor(float DeltaTime)
{

	// UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	// UE_LOG(LogTemp, Warning, TEXT("Yaw is %f"), GetOwner()->GetActorRotation().Yaw);

	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;

	GetOwner()->SetActorRotation(DoorRotation);

	if (AudioComponent && !OpenAudioPlay)
	{
		AudioComponent->Play(DoorCloseDelay);
		OpenAudioPlay = true;
	}
}

float UOpendoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;
	// find all overlapping actors
	TArray<AActor *> OverLappingActors;
	if (!PressurePlate)
	{
		return TotalMass;
	}
	PressurePlate->GetOverlappingActors(OUT OverLappingActors);
	// add masses
	for (AActor *Actor : OverLappingActors)
	{
		TotalMass = TotalMass + Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Actor->GetName());
	}
	return TotalMass;
}