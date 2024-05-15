// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"	
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//DefaultSettings
	MaxHP = 100.f;
	CurrentHP = 100.f;
	bDead = false;
	bRunning = false;
	Damage = 25.f;
	GetCharacterMovement()->MaxWalkSpeed= 500;
	bUseControllerRotationYaw = false;
	bDefending = false;
	AttackSpeed = 1.0f;
	AttackDirection = 0;
	CurrentStamina = 100.f;
	MaxStamina = 100.f;
	TiltX = 0;
	TiltY = 0;
	AttackDirection = 0;
	TimeToRespawn = 5.f;
	//UserSetting
	bEnableCameraShake = true;
	bEnablePlayerVibration = true;
	PlayerName = "None";
	bEnableChromaticAbberration = true;	
	VibrationRate = 1.f;
	Volume = 1.f;
	//UserInput
	DoubleClick = 0;
	TouchSensitive = -7.f;
	TouchLocation = FVector(0);
	//Components
	TPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraArm->SetupAttachment(GetMesh());
	TPCamera->SetupAttachment(CameraArm);
	CameraArm->TargetArmLength = 300.f;
	CameraArm->bUsePawnControlRotation = true;
	PrimaryWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PW"));
	PrimaryWeapon->SetupAttachment(GetMesh());
	SecondaryWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SW"));
	SecondaryWeapon->SetupAttachment(GetMesh());
	PrimaryWeapon->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	PrimaryWeapon->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SecondaryWeapon->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SecondaryWeapon->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	KickArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("B_KickArrowComponent"));
	KickArrowComponent->SetupAttachment(GetMesh());
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	WeaponArrowComponent  = CreateDefaultSubobject<UArrowComponent>(TEXT("B_WeaponArrowComponent"));
	WeaponArrowComponent->SetupAttachment(GetMesh());
	//WeaponCollision
	WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon"));
	WeaponCollision->SetupAttachment(PrimaryWeapon);
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WeaponCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void ABaseCharacter::MoveForward(float Val)
{
	
	const FRotator LocalRotation = GetControlRotation();
		const FVector LocalInputVector = FRotationMatrix(FRotator(0,LocalRotation.Yaw,0)).GetUnitAxis(EAxis::X);
	AddMovementInput(LocalInputVector,Val);
}

void ABaseCharacter::MoveRight(float Val)
{
	
	const FRotator LocalRotation = GetControlRotation();
	const FVector LocalInputVector = FRotationMatrix(FRotator(0,LocalRotation.Yaw,0)).GetUnitAxis(EAxis::Y);
	AddMovementInput(LocalInputVector,Val);
}

void ABaseCharacter::MouseX(float Val)
{
	APawn::AddControllerYawInput(Val);
}

void ABaseCharacter::MouseY(float Val)
{
	APawn::AddControllerPitchInput(Val);
}

/*void ABaseCharacter::StartRunning()
{
	RunningServer(500.f);
	bDefending = false;
	bRunning = true;
}

void ABaseCharacter::StopRunning()
{
	RunningServer(100.f);
	bRunning = false;
}
*/
void ABaseCharacter::LeftAttack()
{
	if(!bAttacking&& !bInjury && CurrentStamina > 10 && !bDead)
	{
		if(bEnableCameraShake)
		{
			PlayCameraFeedback();
		}
		if(bEnablePlayerVibration)
		{
			PlayVibrationFeedback(false);
		}
		CurrentStamina -= 12;
		bAttacking = true;
		bDefending = false;
		ResetInjuryBool();
		UpdateAttackDirection_Server(2);
		MontagePlay_Server(LeftAttackMontageWithoutRootMotion,AttackSpeed);
	}
	else
	{
		if(CurrentStamina <10)
		{
			if(IsLocallyControlled())
			{
				DisplayLowStaminaWidget();
			}
		}
		if (bEnablePlayerVibration)
		{
			PlayVibrationFeedback(false);
		}
	}
}

void ABaseCharacter::RightAttack()
{
	if(!bAttacking&& !bInjury && CurrentStamina > 10 && !bDead)
	{
		if(bEnableCameraShake)
		{
			PlayCameraFeedback();
		}
		if(bEnablePlayerVibration)
		{
			PlayVibrationFeedback(false);
		}
		CurrentStamina -= 12;
		bAttacking = true;
		bDefending = false;
		ResetInjuryBool();
		UpdateAttackDirection_Server(1);
		MontagePlay_Server(RightAttackMontageWithoutRootMotion,AttackSpeed);
	}
	else
	{
		if(CurrentStamina < 10)
		{
			if(IsLocallyControlled())
			{
				DisplayLowStaminaWidget();
			}
			if (bEnablePlayerVibration)
			{
				PlayVibrationFeedback(false);
			}
		}
	}
}

void ABaseCharacter::MidAttack()
{
	if(!bAttacking && !bInjury&& CurrentStamina > 10 && !bDead)
	{
		if(bEnableCameraShake)
		{
			PlayCameraFeedback();
		}
		if(bEnablePlayerVibration)
		{
			PlayVibrationFeedback(false);
		}
		CurrentStamina -= 12;
		bAttacking = true;
		bDefending = false;
		ResetInjuryBool();
		UpdateAttackDirection_Server(0);
		MontagePlay_Server(MidAttackMontageWithoutRootMotion,AttackSpeed);
	}
	else
	{
		if(CurrentStamina < 10)
		{
			if(IsLocallyControlled())
			{
				DisplayLowStaminaWidget();
			}
			if (bEnablePlayerVibration)
			{
				PlayVibrationFeedback(false);
			}
		}
	}
}


void ABaseCharacter::ResetAttackBoolean()
{
	bAttacking = false;
}

void ABaseCharacter::ResetInjuryBool()
{
	bInjury = false;	
}


void ABaseCharacter::KickSphereTrace()
{
	FVector ArrowLocation = KickArrowComponent->GetComponentLocation();
	FVector ArrowForwardLocation = KickArrowComponent->GetForwardVector() * 35;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	TArray<FHitResult> HitArray;
	bool Hit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), ArrowLocation, ArrowLocation + ArrowForwardLocation, 35.f, HitObjectTypes, false, ActorsToIgnore,
		EDrawDebugTrace::None , HitArray, true);
	if(Hit)	
	{
		for(const FHitResult HitTarget : HitArray)
		{ if(Cast<ABaseCharacter>(HitTarget.Actor) != this)
		  {
			 ABaseCharacter * Target = Cast<ABaseCharacter>(HitTarget.Actor);
			 if (Target)
			 {
				 if (bEnableCameraShake)
				 {
					 PlayCameraFeedback();
				 }
				 if (bEnablePlayerVibration)
				 {
					 PlayVibrationFeedback(true);
				 }
				 FRotator AttackerRotation = UKismetMathLibrary::FindLookAtRotation(Target->GetActorLocation(), this->GetActorLocation());
				 float AttackDeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(Target->GetActorRotation(), AttackerRotation).Yaw;

				 if (AttackDeltaRotation >= 100 || AttackDeltaRotation < -100)
				 {
					 UpdateAttackDirection_Server(3);
					 Target->ReceiveDamage(15, true, this, HitTarget.ImpactPoint, true);
				 }
				 else if (Target->bDefending)
				 {
					 Target->ReceiveDamage(10, true, this, HitTarget.ImpactPoint, true);
				 }
				 else
				 {
					 UpdateAttackDirection_Server(0);
					 Target->ReceiveDamage(5, true, this, HitTarget.ImpactPoint, true);
				 }
				 if (IsLocallyControlled())
				 {
					 DisplayHitFeedbackWidget();
					 if (Target->EF_Types == EF_Types)
					 {
						 DisplayTeammateAttackWidget();
					 }
				 }
			 }
		  }
		}
		
	}
}

void ABaseCharacter::WeaponSphereTrace()
{
	FVector ArrowLocation = WeaponArrowComponent->GetComponentLocation();
	FVector ArrowForwardLocation = WeaponArrowComponent->GetForwardVector() * 5;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	TArray<FHitResult> HitArray;
	const bool Hit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(),ArrowLocation,ArrowLocation+ArrowForwardLocation,45.f,UEngineTypes::ConvertToTraceType(ECC_Pawn),false,ActorsToIgnore,
		EDrawDebugTrace::None,HitArray,true);
	if(Hit)	
	{
		for(const FHitResult HitTarget : HitArray)
		{
			if(Cast<ABaseCharacter>(HitTarget.Actor) != this)
			{
				ABaseCharacter * Target = Cast<ABaseCharacter>(HitTarget.Actor);	
				if(Target)
				{
					if(bEnableCameraShake)
					{
						PlayCameraFeedback();
					}
					if(bEnablePlayerVibration)
					{
						PlayVibrationFeedback(true);
					}
					FRotator AttackerRotation = UKismetMathLibrary::FindLookAtRotation(Target->GetActorLocation(),this->GetActorLocation());
					float AttackDeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(Target->GetActorRotation(),AttackerRotation).Yaw;
					if(AttackDeltaRotation >= 100 || AttackDeltaRotation < -100)
					{
						UpdateAttackDirection_Server(3);
						Target->ReceiveDamage(Damage+10, true,this,HitTarget.ImpactPoint,false);
						
					}
					else if(Target->bDefending)
					{
						//Target->Injury(true,3,10,this);
						Target->ReceiveDamage(0,false, this,HitTarget.ImpactPoint,false);
					}
					else
					{
						Target->ReceiveDamage(Damage, false,this,HitTarget.ImpactPoint,false);
					}
					if (IsLocallyControlled())
					{
						DisplayHitFeedbackWidget();
						if (Target->EF_Types == EF_Types)
						{
							DisplayTeammateAttackWidget();
						}
					}
				}
			}
		}
		
	}
}

void ABaseCharacter::RunningSoundPlay()
{
	 SoundPlay_Server(RunningSound[0],GetActorLocation(),4.15,0.4);
}


void ABaseCharacter::DelayEnableDeath()
{
	EnableDeath_Server();
}

void ABaseCharacter::ResetMultipleInjury()
{
	bMultipleInjury = false;
}

void ABaseCharacter::EnableDeath_Multicast_Implementation()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void ABaseCharacter::EnableDeath_Server_Implementation()
{
	EnableDeath_Multicast();
	Respawn();
	bDead = true;
	DisableInput(Cast<APlayerController>(GetController()));
}

void ABaseCharacter::StopPlayAllMontage_Multicast_Implementation()
{
	UAnimInstance * CurrentMeshAnimInstance = GetMesh()->GetAnimInstance();
	if(CurrentMeshAnimInstance)
	{
		CurrentMeshAnimInstance->StopAllMontages(0.3);
	}
}

void ABaseCharacter::StopPlayAllMontage_Server_Implementation()
{
	StopPlayAllMontage_Multicast();
}

void ABaseCharacter::UpdateAttackDirection_Server_Implementation(int f_AttackDirection)
{
	UpdateAttackDirection_Multicast(f_AttackDirection);
}

void ABaseCharacter::UpdateAttackDirection_Multicast_Implementation(int f_AttackDirection)
{
	AttackDirection = f_AttackDirection;
}

//NetworkReplication

void ABaseCharacter::Shield_Multicast_Implementation(bool RShield)
{
	bDefending = RShield;
}

void ABaseCharacter::Shield_Server_Implementation(bool RShield)
{
	Shield_Multicast(RShield);
}

void ABaseCharacter::Running_Server_Implementation(float WalkSpeed)
{
	RunningNet_Multicast(WalkSpeed);
}

void ABaseCharacter::RunningNet_Multicast_Implementation(float WalkSpeed)
{
	 GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}


void ABaseCharacter::MontagePlay_Multicast_Implementation(UAnimMontage* CurrentMontageToPlay, float PlaySpeed)
{
	UAnimInstance * CurrentMeshAnimInstance = GetMesh()->GetAnimInstance();
	if(CurrentMeshAnimInstance)
	{
		CurrentMeshAnimInstance->Montage_Play(CurrentMontageToPlay,PlaySpeed);
	}
}

void ABaseCharacter::MontagePlay_Server_Implementation(UAnimMontage* CurrentMontageToPlay, float PlaySpeed)
{
	MontagePlay_Multicast(CurrentMontageToPlay,PlaySpeed);
	
}


void ABaseCharacter::Defending()
{
	if(CurrentStamina <= 0 )
	{
		Shield_Server(false);
		if(IsLocallyControlled())
		{
			DisplayLowStaminaWidget();
		}
		if (bEnablePlayerVibration)
		{
			PlayVibrationFeedback(false);
		}
	}
	else
	{
		if(!bAttacking)
		{
			Shield_Server(true);
			
		}
	}
	
}

void ABaseCharacter::StopDefending()
{
	Shield_Server(false);
}

/*void ABaseCharacter::Dodge()

{	if(!bAttacking && !bInjury )	
	{
		
		UAnimInstance * CurrentMeshAnimInstance  = GetMesh()->GetAnimInstance();
		if(CurrentMeshAnimInstance)
		{
			CurrentMeshAnimInstance->Montage_Play(DodgeAnimMontage);
		}
	}
}*/



void ABaseCharacter::Kick()
{
	if(!bAttacking && !bInjury && CurrentStamina >= 10)
	{
		if(bEnableCameraShake)
		{
			PlayCameraFeedback();	
		}
		if(bEnablePlayerVibration)
		{
			PlayVibrationFeedback(false);
		}
		CurrentStamina -=10;
		bAttacking = true;
		MontagePlay_Server(KickMontage,AttackSpeed  * 1.15);
	}
	else
	{
		if(CurrentStamina < 10)
		{
			if(IsLocallyControlled())
			{
				DisplayLowStaminaWidget();
			}
			if (bEnablePlayerVibration)
			{
				PlayVibrationFeedback(false);
			}
		}
	}
}
	
FTransform ABaseCharacter::ReturnSpawnPoint()
{
	TSubclassOf<APlayerStart> Start;
	Start = APlayerStart::StaticClass();
	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),Start,SpawnPoints);
	int RandomSpawnPoints = UKismetMathLibrary::RandomIntegerInRange(0,SpawnPoints.Num()-1);
	return SpawnPoints[RandomSpawnPoints]->GetTransform();
}

void ABaseCharacter::InitalizeCharactersInfo_Muticast_Implementation(ESoliderTypes SoliderTypes, EFaction FactionTypes)
{
	EST_Types = SoliderTypes;
	EF_Types = FactionTypes;
	if(EF_Types == EF_Silver)
	{	
		GetMesh()->SetSkeletalMesh(SilverArmorMesh);
	}
	else if (EF_Types == EF_Red)
	{
		GetMesh()->SetSkeletalMesh(RedArmorMesh);
	
	}
}	

void ABaseCharacter::InitalizeCharactersInfo_Server_Implementation(ESoliderTypes SoliderTypes, EFaction FactionTypes)
{
	InitalizeCharactersInfo_Muticast(SoliderTypes,FactionTypes);
} 
//RPCImplmentationxc

void ABaseCharacter::SoundPlayNe_Muticast_Implementation(USoundCue* Sound, FVector PlayLocation, float Time, float volume)
{
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Sound, PlayLocation, FRotator(0), volume * Volume, 1, Time);
}
void ABaseCharacter::SoundPlay_Server_Implementation(USoundCue* Sound,FVector PlayLocation, float Time, float volume = 1.f)
{
	SoundPlayNe_Muticast(Sound,PlayLocation,Time,volume);
}

void ABaseCharacter::SpawnEmitterAtLocationNet_Muticast_Implementation(UParticleSystem* Particle, FVector SpawnLocation,
                                                                      FRotator SpawnRotation, FVector Scale)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),Particle,SpawnLocation,SpawnRotation,Scale);
}

void ABaseCharacter::SpawnEmitterAtLocation_Server_Implementation(UParticleSystem* Particle, FVector SpawnLocation,
FRotator SpawnRotation, FVector Scale)
{		
	SpawnEmitterAtLocationNet_Muticast(Particle,SpawnLocation,SpawnRotation,Scale);
}

void ABaseCharacter::HitEffect(bool Shield,FVector HitLocation)
{
	SpawnEmitterAtLocation_Server(HitParticleEffect,HitLocation,UKismetMathLibrary::RandomRotator(true),FVector(0.5,0.5,0.5));
}

void ABaseCharacter::ReceiveDamage(float f_Damage, bool ShieldBroke, ABaseCharacter* Attacker, FVector HitLocation,
	bool bKick)
{
	if(!bMultipleInjury)
	{
		bMultipleInjury = true;
		GetWorld()->GetTimerManager().SetTimer(_DelayResetMultipleInjury,this,&ABaseCharacter::ResetMultipleInjury,0.2f,false,0.2f);
		bInjury = true;
		bAttacking = false;
		CurrentHP -= f_Damage;
		//HP Calculate
		if(CurrentHP <= 0)
		{
			SetActorTickEnabled(false);
			bDead = true;
			HealthPercentage = 0.f;
			StaminaPercentage = 0.f;
			GetWorld()->GetTimerManager().SetTimer(_DelayDeathHandle,this,&ABaseCharacter::DelayEnableDeath,0.4f,false,0.3f);
		}
		//Sfx Effects play
		int RandomArmourHitSound = ArmourHitSound.Num()-1;
		if(bEnableCameraShake)
		{
			PlayCameraFeedback();
		}
		if(bEnablePlayerVibration)
		{
			PlayVibrationFeedback(true);
		}
		//BackwardAttackReceive
		if(Attacker->AttackDirection == 3)
		{
			if(bEnableChromaticAbberration)
			{
				PlayHitChromaticAbberration();
			}
			if(bKick)
			{
				int RandomKickHitSound = KickHitSound.Num()-1;
				SoundPlay_Server(ShieldHitSound[UKismetMathLibrary::RandomIntegerInRange(0,RandomKickHitSound)],HitLocation,0,0.5f);
				CurrentStamina -= 20.f;
			}
			else
			{
				HitEffect(false,HitLocation);
				CurrentStamina -= 15.f;
				SoundPlay_Server(ArmourHitSound[UKismetMathLibrary::RandomIntegerInRange(0,RandomArmourHitSound)],HitLocation,0);
			}
			//BackwardDirectrions
			Shield_Server(false);
			MontagePlay_Server(HitMontage[3],1.f);
		}
		else
		{
			//DefendingAttackReceive
			if(bDefending)
			{
				int RandomKickHitSound = KickHitSound.Num() - 1;
				if(ShieldBroke)
				{
					if(!bKick)
					{
						CurrentStamina -= 20.f;
						SoundPlay_Server(ShieldHitSound[UKismetMathLibrary::RandomIntegerInRange(0,RandomKickHitSound)],HitLocation,0);
					}
					else
					{
						if(bEnableChromaticAbberration)
						{
							PlayHitChromaticAbberration();
						}
						CurrentStamina -= 10.f;
						SoundPlay_Server(ShieldHitSound[UKismetMathLibrary::RandomIntegerInRange(0, RandomKickHitSound)], HitLocation, 0,0.5f);
					}
					Shield_Server(false);
					MontagePlay_Server(ShieldBreakMontage,1.f);
				}
				else
				{
					HitEffect(true,HitLocation);
					CurrentStamina -= 5.f;
					int RandomShieldHitSound = ShieldHitSound.Num()-1;
					if (bKick)
					{
						SoundPlay_Server(ShieldHitSound[UKismetMathLibrary::RandomIntegerInRange(0, RandomKickHitSound)], HitLocation, 0, 0.5f);
					}
					SoundPlay_Server(ShieldHitSound[UKismetMathLibrary::RandomIntegerInRange(0, RandomShieldHitSound)], HitLocation, 0);
					MontagePlay_Server(ShieldHitMontage,1.f);
				}
			}
			else
			{
				//NormalAttackReceive
				if(bKick)
				{	
					int RandomKickHitSound = KickHitSound.Num()-1;
					SoundPlay_Server(ShieldHitSound[UKismetMathLibrary::RandomIntegerInRange(0, RandomKickHitSound)], HitLocation, 0, 0.5f);
				}
				else
				{
					if(bEnableChromaticAbberration)
					{
						PlayHitChromaticAbberration();
					}
					SoundPlay_Server(ArmourHitSound[UKismetMathLibrary::RandomIntegerInRange(0,RandomArmourHitSound)],HitLocation,0);
					HitEffect(false,HitLocation);
				}
				CurrentStamina -= 10.f;
				MontagePlay_Server(HitMontage[Attacker->AttackDirection],1.f);
			}
		
		}
	}
}


// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()  
{
	Super::BeginPlay();
	CurrentHP = MaxHP;
	CurrentStamina = MaxStamina;
	PrimaryWeapon->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,"PrimaryWeapon");
	SecondaryWeapon->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,"SecondaryWeapon");
	CameraArmExtend();
	CreateCharacterWidget();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//FaceToDesiredRotation
	if(GetVelocity().Size() > 0)
	{	
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
	}
	else
	{
		GetCharacterMovement()->bUseControllerDesiredRotation =	 false;
	}
	//StaminaRecovery
	if(CurrentStamina < 100 && !bDefending && !bAttacking && IsLocallyControlled())
	{
		if(GetVelocity().Size() <200)
		{	
			CurrentStamina+=0.4;
		}
		else
		{
			CurrentStamina+=0.2;
		}	
	}
	//Tilt
	TiltX = Tilt.X;
	TiltY = Tilt.Y;
	if(TiltY < -0.9f && !bTiltDefending && !bAttacking&& IsLocallyControlled())
	{
		bTiltAttackMid = false;
		bTiltAttackLeft = false;
		bTiltAttackRight = false;
		if(CurrentStamina <= 0)
		{
			bTiltDefending = true;
			Shield_Server(false);
			DisplayLowStaminaWidget();
			if (bEnablePlayerVibration)
			{
				PlayVibrationFeedback(false);
			}
		}
		else
		{
			Shield_Server(true);
			CurrentStamina -= 0.12;
		}
	}
	else if(TiltY > 0.5 && !bTiltAttackMid && IsLocallyControlled())
	{
		bTiltAttackMid = true;
		bTiltAttackLeft = false;
		bTiltAttackRight = false;
		MidAttack();
		bTiltDefending = false;
		Shield_Server(false);
	}
	else if( TiltX < -0.5 && !bTiltAttackRight && IsLocallyControlled())
	{

		bTiltAttackRight = true;
		bTiltAttackMid = false;
		bTiltAttackLeft = false;
		RightAttack();
		bTiltDefending = false;
		Shield_Server(false);
	}	
	else if(TiltX > 0.4 && !bTiltAttackLeft && IsLocallyControlled())
	{

		bTiltAttackRight = false;
		bTiltAttackMid = false;
		bTiltAttackLeft = true;	
		LeftAttack();
		bTiltDefending = false;
		Shield_Server(false);
	}
	else if(!bAttacking  && IsLocallyControlled())
	{
		if(TiltX > -0.5)
		{
			bTiltAttackRight = false;
		}
		if(TiltX < 0.5)
		{
			bTiltAttackLeft = false;
		}
		if(TiltY > -0.9f)
		{
			Shield_Server(false);
			bTiltDefending = false;
		}
		else if(TiltY < 0.5)
		{
			bTiltAttackMid = false;	
		}
	}
	//HealthPercentageCalcuate
	HealthPercentage = CurrentHP/MaxHP;
	StaminaPercentage = CurrentStamina/MaxStamina;
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	//BindAxis
	PlayerInputComponent->BindAxis("MoveForward",this,&ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&ABaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("MouseX",this,&ABaseCharacter::MouseX);
	PlayerInputComponent->BindAxis("MouseY",this,&ABaseCharacter::MouseY);
	//BindAction
/*	PlayerInputComponent->BindAction("Run",IE_Pressed,this,&ABaseCharacter::StartRunning);
	PlayerInputComponent->BindAction("Run",IE_Released,this,&ABaseCharacter::StopRunning);*/
	PlayerInputComponent->BindAction("Defend",IE_Pressed,this,&ABaseCharacter::Defending);
	PlayerInputComponent->BindAction("Defend",IE_Released,this,&ABaseCharacter::StopDefending);
	PlayerInputComponent->BindAction("LeftAttack",IE_Pressed,this,&ABaseCharacter::LeftAttack);
	PlayerInputComponent->BindAction("RightAttack",IE_Pressed,this,&ABaseCharacter::RightAttack);
	PlayerInputComponent->BindAction("MidAttack",IE_Pressed,this,&ABaseCharacter::MidAttack);
	PlayerInputComponent->BindAction("Kick",IE_Pressed,this,&ABaseCharacter::Kick);
	/*PlayerInputComponent->BindAction("Dodge",IE_Pressed,this,&ABaseCharacter::Dodge);*/
}




