// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	AActor* Owner = GetOwner();
	
	/*float Z = Owner->GetTransform().GetRotation().Z;
	FString Name = Owner->GetName();

	UE_LOG(LogTemp, Warning, TEXT("Z Rotation of %s is %f"), *Name, Z);

	FRotator NewRotation = FRotator(0, -10, 0);
	Owner->SetActorRotation(NewRotation);*/
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FRotator Rotation = GetOwner()->GetActorRotation();

	GetOwner()->SetActorRotation(FRotator(Rotation.Pitch, Rotation.Yaw - 1, Rotation.Roll));
}

