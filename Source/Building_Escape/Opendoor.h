// Copyright My Bridges 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Opendoor.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDING_ESCAPE_API UOpendoor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpendoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void FindAudioComponent();
	void FindPressurePlate() const;


	bool OpenAudioPlay=true;
private:
	UPROPERTY(EditAnywhere)
	float TargetYaw = 90.f;
	float CurrentYaw;

	UPROPERTY(EditAnywhere)
	ATriggerVolume *PressurePlate=nullptr;

	// UPROPERTY(EditAnywhere)
	// AActor *ActorThatOpen=nullptr;

	float InitialYaw;

	float DoorLastOpened=0.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay=.2f;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed=0.8f;

    UPROPERTY(EditAnywhere)
	float DoorCloseSpeed=2.f;

	UPROPERTY(EditAnywhere)
	float MassToOpenDoor=50.f;

	UPROPERTY()
	UAudioComponent* AudioComponent=nullptr;
};
