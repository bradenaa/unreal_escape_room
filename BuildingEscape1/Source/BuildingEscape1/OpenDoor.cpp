// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"

#include "BuildingEscape1.h"
#include "Gameframework/Actor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UOpenDoor::OpenDoor()
{
    // Set the door rotation
    Owner -> SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
    
    // TODO: make the door rotations relative, so that they swing 90 degrees relative to their current rotational position.
}

void UOpenDoor::CloseDoor()
{
    // Set the door rotation
    Owner -> SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
    
    // TODO: make the door rotations relative, so that they swing 90 degrees relative to their current rotational position.
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
    Owner = GetOwner();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    // Poll the Trigger Volume
    // If the ActorThatOpens is in the volume
    if (GetTotalMassOfActorsOnPlate() > MassNeededToOpenDoor)
    {
        // Open door and store time when it was opened
        OpenDoor();
        LastDoorOpenTime = GetWorld() -> GetTimeSeconds();
    }
    
    // Check if it's time to close the door
    if (GetWorld() -> GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
    {
        CloseDoor();
    }
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
    float TotalMass = 0.f;
    
    // Find all the overlapping actors
    TArray<AActor*> OverlappingActors;
    PressurePlate -> GetOverlappingActors(OUT OverlappingActors);
    
    // iterate through them adding their masses
    for (const auto* Actor : OverlappingActors)
    {
        TotalMass += Actor -> FindComponentByClass<UPrimitiveComponent>() -> GetMass();
        UE_LOG(LogTemp, Warning, TEXT("%s is on the pressure plate"), *Actor -> GetName());
        
    }
    
    return TotalMass;
}

