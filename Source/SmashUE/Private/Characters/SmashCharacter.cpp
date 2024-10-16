﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacter.h"

#include "Characters/SmashCharacterStateMachine.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Characters/SmashCharacterInputData.h"

// Sets default values
ASmashCharacter::ASmashCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	JumpsLeft = NumberOfJumps;
}

// Called when the game starts or when spawned
void ASmashCharacter::BeginPlay()
{
	Super::BeginPlay();
	CreateStateMachine();
	InitStateMachine();
}

// Called every frame
void ASmashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateMeshUsingOrientX();
	TickStateMachine(DeltaTime);
}

// Called to bind functionality to input
void ASmashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupMappingContextIntoController();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent == nullptr) return;

	BindInputMoveXAxisAndActions(EnhancedInputComponent);
}

float ASmashCharacter::GetOrientX() const
{
	return OrientX;
}

void ASmashCharacter::SetOrientX(float NewOrientX)
{
	OrientX = NewOrientX;
}

void ASmashCharacter::ResetJumps(int RecoveredJumps)
{
	if (RecoveredJumps < 0)
	{
		JumpsLeft = NumberOfJumps;
	} else
	{
		JumpsLeft = FMath::Min(NumberOfJumps, JumpsLeft + RecoveredJumps);
	}
}

void ASmashCharacter::RotateMeshUsingOrientX() const
{
	FRotator Rotation = GetMesh()->GetRelativeRotation();
	Rotation.Yaw = -90.f * OrientX;
	GetMesh()->SetRelativeRotation(Rotation);
}

void ASmashCharacter::CreateStateMachine()
{
	StateMachine = NewObject<USmashCharacterStateMachine>(this);
}

void ASmashCharacter::InitStateMachine()
{
	if (StateMachine == nullptr) return;
	StateMachine->Init(this);
}

void ASmashCharacter::TickStateMachine(float DeltaTime) const
{
	if (StateMachine == nullptr) return;
	StateMachine->Tick(DeltaTime);
}

void ASmashCharacter::SetupMappingContextIntoController() const
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController == nullptr) return;

	ULocalPlayer* Player = PlayerController->GetLocalPlayer();
	if (Player == nullptr) return;
	

	UEnhancedInputLocalPlayerSubsystem* InputSystem = Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (InputSystem == nullptr) return;

	InputSystem->AddMappingContext(InputMappingContext, 0);
}

float ASmashCharacter::GetInputMoveX() const
{
	return InputMoveX;
}

void ASmashCharacter::OnInputMoveX(const FInputActionValue& InputActionValue)
{
	InputMoveX = InputActionValue.Get<float>();
}

void ASmashCharacter::OnFastFallInput(const FInputActionValue& InputActionValue)
{
	FastFallPressedTime = GetWorld()->GetTime().GetRealTimeSeconds();
	InputFastFallEvent.Broadcast();
}

void ASmashCharacter::BindInputMoveXAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (InputData == nullptr) return;

	if (InputData->InputActionMoveX)
	{
		EnhancedInputComponent->BindAction(
		InputData->InputActionMoveX,
		ETriggerEvent::Started,
		this,
		&ASmashCharacter::OnInputMoveX);

		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveX,
			ETriggerEvent::Completed,
			this,
			&ASmashCharacter::OnInputMoveX);
	
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveX,
			ETriggerEvent::Triggered,
			this,
			&ASmashCharacter::OnInputMoveX);
	}

	if (InputData->InputActionMoveXFast)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveXFast,
			ETriggerEvent::Triggered,
			this,
			&ASmashCharacter::OnInputMoveXFast);
	}

	if (InputData->InputActionJump)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionJump,
			ETriggerEvent::Started,
			this,
			&ASmashCharacter::OnInputJump);
	}

	if (InputData->InputActionJumpFast)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionJumpFast,
			ETriggerEvent::Started,
			this,
			&ASmashCharacter::OnInputJump);
	}

	if (InputData->InputActionFastFall)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionFastFall,
			ETriggerEvent::Triggered,
			this,
			&ASmashCharacter::OnFastFallInput);
	}
}

void ASmashCharacter::OnInputMoveXFast(const FInputActionValue& InputActionValue)
{
	InputMoveX = InputActionValue.Get<float>();
	InputMoveXFastEvent.Broadcast(InputMoveX);
}

bool ASmashCharacter::CanDoJump() const
{
	return JumpsLeft > 0;
}

void ASmashCharacter::ConsumeJump()
{
	JumpsLeft--;
}

void ASmashCharacter::DoJump()
{
	Jump();
	ConsumeJump();
}

bool ASmashCharacter::ShouldFastFall() const
{
	return (GetWorld()->GetTime().GetRealTimeSeconds() - FastFallPressedTime) < FastFallWindowTime;
}

void ASmashCharacter::OnInputJump(const FInputActionValue& InputActionValue)
{
	InputJumpEvent.Broadcast();
}

