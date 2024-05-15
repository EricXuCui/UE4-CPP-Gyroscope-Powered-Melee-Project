// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "BaseCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class BANNERET_API ABaseCharacterController : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UUserWidget> BaseWidget;
	UUserWidget * BaseWidgetInstance;
	virtual void BeginPlay() override;
	
};
