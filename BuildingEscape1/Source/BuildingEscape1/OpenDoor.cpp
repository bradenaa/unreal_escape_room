// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"

#include "BuildingEscape1.h"
#include "Gameframework/Actor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UOpenDoor::OpenDoor() {
    // Find the owning Actor
    AActor* Owner = GetOwner();
    
    // Create a rotator
    FRotator NewRotation = FRotator(0.0f, 90.0f, 0.0f);
    
    // Set the door rotation
    Owner -> SetActorRotation(NewRotation);
    
    // TODO: make the door rotations relative, so that they swing 90 degrees relative to their current rotational position.
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
    
    ActorThatOpens = GetWorld() -> GetFirstPlayerController() -> GetPawn();
    

}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    // Poll the Trigger Volume
    // If the ActorThatOpens is in the volume
    if (PressurePlate != nullptr && ActorThatOpens != nullptr && (PressurePlate -> IsOverlappingActor(ActorThatOpens)))
    {
        OpenDoor();
    }
    
}

