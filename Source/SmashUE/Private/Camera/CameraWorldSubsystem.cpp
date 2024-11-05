// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraWorldSubsystem.h"

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

void UCameraWorldSubsystem::PostInitialize()
{
	Super::PostInitialize();
}

void UCameraWorldSubsystem::AddFollowTarget(AActor* FollowTarget)
{
	FollowTargets.Add(FollowTarget);
}

void UCameraWorldSubsystem::RemoveFollowTarget(AActor* FollowTarget)
{
	FollowTargets.Remove(FollowTarget);
}

void UCameraWorldSubsystem::TickUpdateCameraPosition(float DeltaTime)
{
	FVector AverageLocation = CalculateAveragePositionBetweenTargets();
	AverageLocation.Y = CameraMain->GetComponentLocation().Y;
	CameraMain->SetWorldLocation(AverageLocation);
}

FVector UCameraWorldSubsystem::CalculateAveragePositionBetweenTargets()
{
	FVector averagePos;
	for (AActor* FollowTarget : FollowTargets)
	{
		averagePos += FollowTarget->GetActorLocation();
	}
	averagePos /= FollowTargets.Num();
	return averagePos;
}

UCameraComponent* UCameraWorldSubsystem::FindCameraByTag(const FName& Tag) const
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag, FoundActors);

	
	// UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		UActorComponent* Component = Actor->GetComponentByClass(UCameraComponent::StaticClass());
		UCameraComponent* CameraComponent = Cast<UCameraComponent>(Component);
		if (CameraComponent != nullptr)
		{
			return CameraComponent;
		}
	}
	return nullptr;
}

void UCameraWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	CameraMain = FindCameraByTag(TEXT("CameraMain"));
}

void UCameraWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickUpdateCameraPosition(DeltaTime);
}
