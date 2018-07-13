// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

#include "BuildingEscape1.h"
#include "Gameframework/Actor.h"
#include "Public/DrawDebugHelpers.h"
#include "GameFramework/Controller.h"

#define OUT


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

    UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get the player viewpoint this tick
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld() -> GetFirstPlayerController() -> GetPlayerViewPoint(
        OUT PlayerViewPointLocation,
        OUT PlayerViewPointRotation
    );
    // Logout to test
    //    UE_LOG(LogTemp, Warning, TEXT("Player is at Location: %s -- Player is at rotation: %s"),
    //           *PlayerViewPointLocation.ToString(),
    //           *PlayerViewPointRotation.ToString()
    //    );
    
    FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
    
    // Draw a red trace to visualize
    DrawDebugLine(
        GetWorld(),
        PlayerViewPointLocation,
        LineTraceEnd,
        FColor(255,0,0),
        false,
        0.f,
        0.f,
        10.f
    );
    
    // Setup query parameters
    FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
    
    // Line-trace(AKA ray-cast) out to reach distance
    FHitResult Hit;
    
    
    GetWorld() -> LineTraceSingleByObjectType(
        OUT Hit,
        PlayerViewPointLocation,
        LineTraceEnd,
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        TraceParameters
    );
    // See what we hit
    
    AActor* ActorHit = Hit.GetActor();
    if (ActorHit)
    {
        UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit -> GetName()) );
    }
    
    
}

