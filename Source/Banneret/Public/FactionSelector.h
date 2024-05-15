// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TimerManager.h"
#include "CharacterInfo.h"
#include "FactionSelector.generated.h"


UCLASS()
class BANNERET_API AFactionSelector : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFactionSelector();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction", Replicated)
		int RedPlayers;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction", Replicated)
		int SilverPlayers;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateUIIndex();
	UFUNCTION(BlueprintImplementableEvent)
		void SyncUIIndex();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
		FString PlayerName;
	FTimerHandle _DelaySync;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
