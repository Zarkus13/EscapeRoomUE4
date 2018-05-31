// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Public/DrawDebugHelpers.h"
#include "Engine/EngineTypes.h"
#include "Components/InputComponent.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"

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

	AttachPhysicsHandler();
	AttachInputComponent();
}

void UGrabber::AttachPhysicsHandler()
{
	PhysicsHandler = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandler) {
		UE_LOG(LogTemp, Warning, TEXT("%s has a PhysicsHandler !"), *GetOwner()->GetName());
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Hoho, no PhysicsHandler in %s"), *GetOwner()->GetName());
	}
}

void UGrabber::AttachInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("%s has an InputComponent !"), *GetOwner()->GetName());

		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Hoho, no InputComponent in %s"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	if (!PhysicsHandler) return;

	UE_LOG(LogTemp, Warning, TEXT("Grab !"));

	auto Hit = GetFirstPhysicsBodyInReach();

	if (Hit.GetActor()) {
		auto ComponentToGrab = Hit.GetComponent();

		PhysicsHandler->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			Hit.ImpactPoint,
			ComponentToGrab->GetOwner()->GetActorRotation()
		);
	}
}

FVector UGrabber::GetLineTraceEnd(FPlayerPosition PlayerPosition)
{
	return PlayerPosition.Location + (PlayerPosition.Rotation.Vector() * ReachInMeters * 100);
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());
	FPlayerPosition PlayerPosition = GetPlayerPosition();
	FVector LineTraceEnd = GetLineTraceEnd(PlayerPosition);

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerPosition.Location,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	return Hit;
}

FPlayerPosition UGrabber::GetPlayerPosition()
{
	FPlayerPosition result;

	GetWorld()
		->GetFirstPlayerController()
		->GetPlayerViewPoint(
			OUT result.Location,
			OUT result.Rotation
		);

	return result;
}

void UGrabber::Release()
{
	if (!PhysicsHandler) return;

	UE_LOG(LogTemp, Warning, TEXT("Release !"));
	PhysicsHandler->ReleaseComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandler && PhysicsHandler->GrabbedComponent) {
		PhysicsHandler->SetTargetLocation(
			GetLineTraceEnd(
				GetPlayerPosition()
			)
		);
	}

}

