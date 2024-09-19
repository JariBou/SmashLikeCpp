// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "SmashCharacterStateSettings.h"
#include "GameFramework/Character.h"
#include "SmashCharacter.generated.h"

enum class ESmashCharacterStateID : uint8;
class USmashCharacterState;
class USmashCharacterInputData;
class UInputMappingContext;
class USmashCharacterStateMachine;

UCLASS()
class SMASHUE_API ASmashCharacter : public ACharacter
{
	GENERATED_BODY()

#pragma region Unreal Default

public:
	// Sets default values for this character's properties
	ASmashCharacter();

	UPROPERTY(EditAnywhere)
	TMap<ESmashCharacterStateID, FSmashCharacterStateSettings> OverrideStates;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma endregion

#pragma region Orient

public:
	float GetOrientX() const;

	void SetOrientX(float NewOrientX);
	
	void ResetJumps(int NumberOfJumps);

protected:
	UPROPERTY(EditAnywhere)
	float OrientX = 1.f;

	void RotateMeshUsingOrientX() const;


#pragma endregion

#pragma region StateMachine

public:
	void CreateStateMachine();

	void InitStateMachine();

	void TickStateMachine(float DeltaTime) const;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USmashCharacterStateMachine> StateMachine;
	
#pragma endregion

#pragma region Input Data / Mapping Context

public:
	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY()
	TObjectPtr<USmashCharacterInputData> InputData;

protected:
	void SetupMappingContextIntoController() const;

	
	
#pragma endregion

#pragma region Input Move X

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputMoveXEvent, float, InputMoveX);

public:
	float GetInputMoveX() const;

	UPROPERTY()
	FInputMoveXEvent InputMoveXFastEvent;
	
protected:
	UPROPERTY()
	float InputMoveX = 0.f;

private:
	void OnInputMoveX(const FInputActionValue& InputActionValue);

	void OnFastFallInput(const FInputActionValue& InputActionValue);
	
	void BindInputMoveXAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent);
	
	void OnInputMoveXFast(const FInputActionValue& InputActionValue);
	
	
#pragma endregion

#pragma region Jump
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInputJumpEvent);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInputFastFallEvent);
	
	UPROPERTY()
	FInputJumpEvent InputJumpEvent;
	
	UPROPERTY()
	FInputFastFallEvent InputFastFallEvent;

	bool CanDoJump() const;

	void ConsumeJump();

	void DoJump();

	bool ShouldFastFall() const;

private:
	UPROPERTY(EditAnywhere)
	int NumberOfJumps = 2;

	UPROPERTY()
	int JumpsLeft;

	UPROPERTY()
	float FastFallPressedTime = 0.f;

	UPROPERTY()
	float FastFallWindowTime = .5;
	
	void OnInputJump(const FInputActionValue& InputActionValue);

#pragma endregion
};
