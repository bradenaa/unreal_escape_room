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
    FindPhysicsHandleComponent();
    SetupInputComponent();
}

/// Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{
    PhysicsHandle = GetOwner() -> FindComponentByClass<UPhysicsHandleComponent>();
    if (PhysicsHandle == nullptr)
    {
        // Physics handle is found
        UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner() -> GetName());
    }
}

/// Look for attached input component (only appears at runtime)
void UGrabber::SetupInputComponent()
{
    InputComponent = GetOwner() -> FindComponentByClass<UInputComponent>();
    if (InputComponent)
    {
        InputComponent -> BindAction("Grab", IE_Pressed, this, &UGrabber::Grab); // input keys bound in unreal
        InputComponent -> BindAction("Grab", IE_Released, this, &UGrabber::Release); // input keys bound in unreal
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner() -> GetName());
    };
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    // if the physics handle is attached
    if (PhysicsHandle -> GrabbedComponent)
    {
        // move the object that we're holding
        PhysicsHandle -> SetTargetLocation(GetReachLineEnd());
    }
}

void UGrabber::Grab()
{
    // LINE TRACE and see if we reach any actors with physics body collision channal set
    auto HitResult = GetFirstPhysicsBodyInReach();
    auto ComponentToGrab = HitResult.GetComponent(); // gets the mesh in our case
    auto ActorHit = HitResult.GetActor();
    
    // If we hit something then attach a physics handle
    if (ActorHit)
    {
        PhysicsHandle -> GrabComponentAtLocationWithRotation(
           ComponentToGrab,
           NAME_None, // no bones needed
           ComponentToGrab -> GetOwner() -> GetActorLocation(),
           ComponentToGrab -> GetOwner() -> GetActorRotation()
        );
    }
}

void UGrabber::Release()
{
    // release physics handle
    PhysicsHandle -> ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
    // Setup query parameters
    FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
    
    // Line-trace(AKA ray-cast) out to reach distance
    FHitResult HitResult;
    
    GetWorld() -> LineTraceSingleByObjectType(
      OUT HitResult,
      GetReachLineStart(),
      GetReachLineEnd(),
      FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
      TraceParameters
    );
    return HitResult;
}

FVector UGrabber::GetReachLineStart()
{
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld() -> GetFirstPlayerController() -> GetPlayerViewPoint(
       OUT PlayerViewPointLocation,
       OUT PlayerViewPointRotation
    );
    
    // Adds the two components as vectors
    return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld() -> GetFirstPlayerController() -> GetPlayerViewPoint(
       OUT PlayerViewPointLocation,
       OUT PlayerViewPointRotation
    );
    
    // Adds the two components as vectors
    return PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
}





