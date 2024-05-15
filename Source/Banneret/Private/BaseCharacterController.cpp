// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterController.h"

void ABaseCharacterController::BeginPlay()
{
	Super::BeginPlay();
	if(IsLocalPlayerController())
	{
		if(BaseWidget)
		{
			BaseWidgetInstance = CreateWidget<UUserWidget>(this,BaseWidget);
			if(BaseWidgetInstance)
			{
				BaseWidgetInstance->AddToViewport();
			}
		}
	}
	
}




