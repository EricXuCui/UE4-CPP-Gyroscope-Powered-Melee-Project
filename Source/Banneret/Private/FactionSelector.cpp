// Fill out your copyright notice in the Description page of Project Settings.


#include "FactionSelector.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AFactionSelector::AFactionSelector()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RedPlayers = 0;
	SilverPlayers = 0;
}


// Called when the game starts or when spawned
void AFactionSelector::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(_DelaySync, this, &AFactionSelector::SyncUIIndex, 1.f, true, 0);
}

// Called every frame
void AFactionSelector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AFactionSelector::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AFactionSelector::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const 
{ 
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 
	DOREPLIFETIME(AFactionSelector, RedPlayers);
	DOREPLIFETIME(AFactionSelector, SilverPlayers);
}