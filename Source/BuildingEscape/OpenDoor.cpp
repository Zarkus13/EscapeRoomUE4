// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate->IsOverlappingActor(ActorThatOpens)) {
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	else if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime >= DoorCloseDelay) {
		CloseDoor();
	}
}

void UOpenDoor::OpenDoor()
{
	FRotator Rotation = Owner->GetActorRotation();

	Owner->SetActorRotation(FRotator(Rotation.Pitch, OpenAngle < Rotation.Yaw ? Rotation.Yaw - 1 : OpenAngle, Rotation.Roll));
}

void UOpenDoor::CloseDoor()
{
	FRotator Rotation = Owner->GetActorRotation();

	Owner->SetActorRotation(FRotator(Rotation.Pitch, Rotation.Yaw < 0 ? Rotation.Yaw + 3 : 0, Rotation.Roll));
}

