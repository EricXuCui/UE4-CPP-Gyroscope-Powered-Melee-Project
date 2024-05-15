// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerController.h"
#include "Sound/SoundCue.h"
#include "CharacterInfo.h"
#include "BaseCharacterController.h"
#include "BaseCharacter.generated.h"


UCLASS()
class BANNERET_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();
	//Functions
	virtual void MoveForward(float Val);
	virtual void MoveRight(float Val);
	virtual void MouseX(float Val);
	virtual void MouseY(float Val);
	/*virtual void StartRunning();
	virtual void StopRunning(); */
	virtual void LeftAttack();
	virtual void RightAttack();
	virtual void MidAttack();
	virtual void Defending();
	virtual void StopDefending();		
	//virtual void Dodge();
	virtual void HitEffect(bool Shield,FVector HitLocation);
	virtual void ReceiveDamage(float f_Damage, bool ShieldBroke,ABaseCharacter * Attacker,FVector HitLocation, bool bKick);
	
	//UFunctions
	UFUNCTION(BlueprintImplementableEvent)
		void CameraArmExtend();
	UFUNCTION(BlueprintImplementableEvent)
		void DisplayLowStaminaWidget();
	UFUNCTION(BlueprintImplementableEvent)
		void PlayCameraFeedback();
	UFUNCTION(BlueprintImplementableEvent)
		void PlayVibrationFeedback(bool hit);
	UFUNCTION(BlueprintImplementableEvent)
		void PlayHitChromaticAbberration();
	UFUNCTION(BlueprintImplementableEvent)
		void CreateCharacterWidget();
	UFUNCTION(BlueprintImplementableEvent)
		void DisplayHitFeedbackWidget();
	UFUNCTION(BlueprintImplementableEvent)
		void DisplayTeammateAttackWidget();
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
		void ShowWaitingPlayersWidget();
	UFUNCTION(BlueprintCallable)		
		void ResetAttackBoolean();
	UFUNCTION(BlueprintCallable)
		void ResetInjuryBool();
	UFUNCTION(BlueprintCallable)
		void KickSphereTrace();
	UFUNCTION(BlueprintCallable)
		void WeaponSphereTrace();
	UFUNCTION(BlueprintCallable)
		void RunningSoundPlay();
	UFUNCTION()
		void DelayEnableDeath();
	UFUNCTION()
		 void ResetMultipleInjury();
	UFUNCTION(BlueprintImplementableEvent)
		void Respawn();
	UFUNCTION(BlueprintCallable)
		virtual void Kick();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		virtual FTransform ReturnSpawnPoint();
	//NetworkReplicationFunctions
	UFUNCTION(Server,Reliable)
		void MontagePlay_Server(UAnimMontage * CurrentMontageToPlay, float PlaySpeed);
	UFUNCTION(NetMulticast,Reliable)
		void MontagePlay_Multicast(UAnimMontage * CurrentMontageToPlay, float PlaySpeed);
	UFUNCTION(NetMulticast,Reliable)
		void RunningNet_Multicast(float WalkSpeed);
	UFUNCTION(Server,Reliable)
		void Running_Server(float WalkSpeed);
	UFUNCTION(Server,Reliable,BlueprintCallable)
		void Shield_Server(bool RShield);
	UFUNCTION(NetMulticast,Reliable)
		void Shield_Multicast(bool RShield);
	UFUNCTION(Server,Reliable)
		void UpdateAttackDirection_Server(int f_AttackDirection);
	UFUNCTION(NetMulticast,Reliable)
		void UpdateAttackDirection_Multicast(int f_AttackDirection);
	UFUNCTION(Server,Reliable)
		void StopPlayAllMontage_Server();
	UFUNCTION(NetMulticast,Reliable)
		void StopPlayAllMontage_Multicast();
	UFUNCTION(Server,Reliable)
		void EnableDeath_Server();
	UFUNCTION(NetMulticast,Reliable)
		void EnableDeath_Multicast();
	UFUNCTION(Server,Reliable)
		void SpawnEmitterAtLocation_Server(UParticleSystem * Particle,FVector SpawnLocation,FRotator SpawnRotation,
		FVector Scale);
	UFUNCTION(NetMulticast,Reliable)
		void SpawnEmitterAtLocationNet_Muticast(UParticleSystem * Particle,FVector SpawnLocation,FRotator SpawnRotation,
	FVector Scale);
	UFUNCTION(Server,Reliable)
		void SoundPlay_Server(USoundCue * Sound, FVector PlayLocation, float Time, float volume = 1.f);
	UFUNCTION(NetMulticast,Reliable)
		void SoundPlayNe_Muticast(USoundCue * Sound, FVector PlayLocation, float Time, float volume);
	UFUNCTION(Server,Reliable,BlueprintCallable)
		void InitalizeCharactersInfo_Server(ESoliderTypes SoliderTypes,EFaction FactionTypes);
	UFUNCTION(NetMulticast,Reliable)
		void InitalizeCharactersInfo_Muticast(ESoliderTypes SoliderTypes,EFaction FactionTypes);
	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP")
		float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP")
		bool bDead;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP")
		float HealthPercentage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP")
		float StaminaPercentage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP")
		bool bRunning;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP")
		bool bAttacking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP")
		float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP")	
		float AttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP")
		TEnumAsByte<ESoliderTypes> EST_Types = EST_Knight;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP")
		bool bDefending;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
		UAnimMontage * DodgeAnimMontage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= "Montage")
		UAnimMontage * LeftAttackMontageWithoutRootMotion;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= "Montage")
		UAnimMontage * RightAttackMontageWithoutRootMotion;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= "Montage")
		UAnimMontage * MidAttackMontageWithoutRootMotion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP")
		TArray<TEnumAsByte<EObjectTypeQuery>> HitObjectTypes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
		TArray<UAnimMontage*> HitMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
		UAnimMontage * ShieldHitMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
		UAnimMontage * ShieldBreakMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
		UAnimMontage * KickMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP")
		bool bInjury;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP")
		float MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP")
		float TiltX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP")
		float TiltY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP")
		float TimeToRespawn;
	//UserSetting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserSetting")
		bool bEnableCameraShake;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserSetting")
		bool bEnablePlayerVibration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserSetting")
		bool bEnableChromaticAbberration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserSetting")
	    FString PlayerName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserSetting")
		float VibrationRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserSetting")
		float Volume;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP")
		TEnumAsByte<EFaction> EF_Types = EF_Silver;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP")
		UParticleSystem * HitParticleEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP")
		ABaseCharacterController * OwnController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPP")
		FVector Tilt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		TArray<USoundCue*> ShieldHitSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		TArray<USoundCue*> ArmourHitSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		TArray<USoundCue*>	RunningSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		TArray<USoundCue*> KickHitSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserInput")
		FVector TouchLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserInput")
		float TouchSensitive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserInput")
		int DoubleClick;
	float CurrentHP;
	float CurrentStamina;
	int AttackDirection;
	bool bTiltAttackMid;
	bool bTiltAttackRight;
	bool bTiltAttackLeft;
	bool bTiltDefending;
	bool bMultipleInjury;
	FTimerHandle _DelayDeathHandle;
	FTimerHandle _DelayResetMultipleInjury;
	
	//Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPPComponents")
		UCameraComponent * TPCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPPComponents")
		USpringArmComponent * CameraArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPPComponents")
		UStaticMeshComponent * PrimaryWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPPComponents")
		UStaticMeshComponent * SecondaryWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPPComponents")
		UArrowComponent * KickArrowComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPPComponents")
		UArrowComponent * WeaponArrowComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPPComponents")
		USkeletalMesh * SilverArmorMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPPComponents")
		USkeletalMesh * RedArmorMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CPPComponents")
		UBoxComponent* WeaponCollision;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input*
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
