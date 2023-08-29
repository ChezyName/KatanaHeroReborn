// Copyright Epic Games, Inc. All Rights Reserved.

#include "KatanaHeroRebornCharacter.h"
#include "DefaultEnemy.h"
#include "DrawDebugHelpers.h"
#include "KatanaHeroRebornGameMode.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void AKatanaHeroRebornCharacter::DisplayAfterImages()
{
	AActor* VFX = UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(),AfterImages,FTransform(FRotator::ZeroRotator,GetMesh()->GetComponentLocation()),ESpawnActorCollisionHandlingMethod::AlwaysSpawn,this);
	UGameplayStatics::FinishSpawningActor(VFX, FTransform(FRotator::ZeroRotator,GetMesh()->GetComponentLocation()));
}

void AKatanaHeroRebornCharacter::TakeDamageChar(float Damage)
{
	if(Attacking || Dashing) return;
	if(Damage <= 0) return;
	
	Health -= Damage;
	DisplayAfterImages();
	LaunchCharacter((-GetActorForwardVector() + GetActorUpVector()*0.6) * 1500,true,true);
}

FVector AKatanaHeroRebornCharacter::GetMouseLocation()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		FVector WorldLoc, WorldDir;
		if (PC->DeprojectMousePositionToWorld(WorldLoc, WorldDir))
		{
			return WorldLoc;
		}
	}
	return FVector::ZeroVector;
}

void AKatanaHeroRebornCharacter::LookAtMouse()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
		return;

	FVector MouseWorldPos, MouseWorldDir;
	if (!PC->DeprojectMousePositionToWorld(MouseWorldPos, MouseWorldDir))
		return;

	FVector CharacterLocation = GetActorLocation();
	FVector DirectionToMouse = MouseWorldPos - CharacterLocation;
	DirectionToMouse.Z = 0.0f; // Ignore the Z component to stay in the 2D plane
	DirectionToMouse.Normalize();

	FRotator LookAtRotation = DirectionToMouse.Rotation();
	LookAtRotation.Pitch = 0.0f; // Set pitch to zero to keep character upright
	SetActorRotation(LookAtRotation);

	GetCharacterMovement()->bOrientRotationToMovement = false; // Face in the direction we are moving..

	DrawDebugSphere(GetWorld(), MouseWorldPos, 5, 8, FColor::Green, false, 60, 0, 1);
	DrawDebugLine(GetWorld(), CharacterLocation, MouseWorldPos, FColor::Red, false, 60, 0, 1);
}

void AKatanaHeroRebornCharacter::StopLookingAtMouse()
{
	SetActorRotation(FRotator::ZeroRotator);
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
}

void AKatanaHeroRebornCharacter::Dash()
{
	if(DashCD > 0 || Dashing) return;
	//const FVector MoveDir = FVector(0,-GetInputAxisValue("MoveRight"),GetInputAxisValue("LookUp"));
	StopMovement = true;
	Dashing = true;

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]
	{
		DashCD = 3;
		StopMovement = false;
		Dashing = false;
	});
		
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, DashTime, false);
}

AKatanaHeroRebornCharacter::AKatanaHeroRebornCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->SetRelativeRotation(FRotator(0.f,180.f,0.f));

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 999999.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	Katana = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KatanaModel"));
	Katana -> SetupAttachment(GetMesh(),"RHand");

	KatanaHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("KatanaHitbox"));
	KatanaHitbox -> SetupAttachment(RootComponent);
	KatanaHitbox->OnComponentBeginOverlap.AddDynamic(this, &AKatanaHeroRebornCharacter::OnKatanaHit);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AKatanaHeroRebornCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this,  &AKatanaHeroRebornCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &AKatanaHeroRebornCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AKatanaHeroRebornCharacter::LookUp);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AKatanaHeroRebornCharacter::Dash);
	
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &AKatanaHeroRebornCharacter::KatanaAttack);
	PlayerInputComponent->BindAction("Tornado", IE_Pressed, this, &AKatanaHeroRebornCharacter::SummonTornado);
}

void AKatanaHeroRebornCharacter::BeginPlay()
{
	Super::BeginPlay();
	X = GetActorLocation().X;
	SetActorRotation(FRotator(0,-90,0));
	KatanaHitbox->OnComponentBeginOverlap.AddDynamic(this, &AKatanaHeroRebornCharacter::OnKatanaHit);
}

void AKatanaHeroRebornCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	Katana->SetVisibility(SwordActive);
	DashCD -= DeltaSeconds;
	if(Dashing)
	{
		AKatanaHeroRebornGameMode* GM = Cast<AKatanaHeroRebornGameMode>(GetWorld()->GetAuthGameMode());
		float Modifier = 100;
		if(GM) Modifier = GM->SpeedUpgrade;
		LaunchCharacter(GetActorForwardVector() * (DodgeSpeed*100 * (Modifier/100)),true,false);
		DisplayAfterImages();
	}
	SetActorLocation(FVector(X,GetActorLocation().Y,GetActorLocation().Z));
}

void AKatanaHeroRebornCharacter::KatanaAttack()
{
	if(Attacking) return;
	EnemiesHit.Empty();
	SwordActive = true;
	Attacking = true;

	//Slash Attack HERE
	PlayAnimMontage(SwordAttachAnimation);
	StopMovement = true;
	//LookAtMouse();
	LaunchCharacter(GetActorForwardVector() * (DashSpeed*100),true,false);
	//DisplayAfterImages();

	PlaySlashVFX();

	TArray<AActor*> ActorsHit;
	KatanaHitbox->GetOverlappingActors(ActorsHit);
	for(AActor* A : ActorsHit)
	{
		ADefaultEnemy* Enemy = Cast<ADefaultEnemy>(A);
		if(Enemy && !EnemiesHit.Contains(Enemy))
		{
			//Deal Damage To Enemy
			Enemy->TakeSomeDamage(KatanaDamage);
			EnemiesHit.Add(Enemy);
		}
	}

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]
	{
		StopMovement = false;
		SwordActive = false;
		Attacking = false;
		StopAnimMontage(SwordAttachAnimation);
		//StopLookingAtMouse();
	});
		
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, HitboxActiveT, false);
}

void AKatanaHeroRebornCharacter::MoveRight(float Value)
{
	DashDir.Y = -Value;
	if(StopMovement) return;
	// add movement in that direction
	AddMovementInput(FVector(0.f,-1.f,0.f), Value);
	
	if(Value == 0) return;
	if(Value < 0) GetMesh()->SetRelativeScale3D(FVector(-1,1,1));
	else GetMesh()->SetRelativeScale3D(FVector(1,1,1));
}

void AKatanaHeroRebornCharacter::LookUp(float Val)
{
	DashDir.Y = Val;
}

void AKatanaHeroRebornCharacter::StartJump()
{
	if(!StopMovement) Jump();
}

void AKatanaHeroRebornCharacter::OnKatanaHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!Attacking) return;
	ADefaultEnemy* Enemy = Cast<ADefaultEnemy>(OtherActor);
	if(Enemy && !EnemiesHit.Contains(Enemy))
	{
		//Deal Damage To Enemy
		Enemy->TakeSomeDamage(KatanaDamage);
		EnemiesHit.Add(Enemy);
	}
}

void AKatanaHeroRebornCharacter::SummonTornado()
{
	AActor* Tornado = UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(),TornadoSpawnable,FTransform(FRotator::ZeroRotator,GetMesh()->GetComponentLocation()),ESpawnActorCollisionHandlingMethod::AlwaysSpawn,this);
	UGameplayStatics::FinishSpawningActor(Tornado, FTransform(FRotator::ZeroRotator,GetMesh()->GetComponentLocation()));
}