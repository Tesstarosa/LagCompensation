// Fill out your copyright notice in the Description page of Project Settings.


#include "TrackComponent.h"
#include "LagCompensationManager.h"
#include "Engine/NetDriver.h"

// Sets default values for this component's properties
UTrackComponent::UTrackComponent()
{
	bWantsInitializeComponent = true;
}

void UTrackComponent::AddFrame()
{
	if(FrameKeys.Count() == MaxFrameCount)
	{
		FrameData.Remove(FrameKeys.Dequeue());
	}

	FTransform transform = GetOwner()->GetTransform();
	TrackedPointData data;
	data.Position = transform.GetLocation();
	data.Rotation = transform.GetRotation();

	const float time = GetServerTime();
	FrameData.Add(time, data);
	FrameKeys.Enqueue(time);
}

void UTrackComponent::InitializeComponent()
{
	MaxFrameCount = SaveHistorySeconds / (1.0f / 30.0f);
	if (const UWorld* world = GetWorld())
	{
		const UNetDriver* NetDriver = world->GetNetDriver();
		if (NetDriver)
			MaxFrameCount = SaveHistorySeconds / (1.0f / NetDriver->MaxNetTickRate);			
	}

	FrameKeys.SetSize(MaxFrameCount);

	Super::InitializeComponent();
	LagCompensationManager::Register(this);
}

void UTrackComponent::UninitializeComponent()
{
	LagCompensationManager::UnRegister(this);
	Super::UninitializeComponent();
}

float UTrackComponent::GetServerTime() const
{
	return GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
}

float UTrackComponent::GetRTT(const float clientTime) const
{
	return GetServerTime() - clientTime;
}

float UTrackComponent::GetAvgTimeBetweenPointMs()
{
	return (GetTotalHistoryTime() / FrameKeys.Count()) * 1000.0f;
}

float UTrackComponent::GetTotalHistoryTime()
{
	if (FrameKeys.Count() <= 0)
		return 0;

	return ((FrameKeys.ElementAt(0) - FrameKeys.ElementAt(FrameKeys.Count() - 1)));
}

void UTrackComponent::ReverseTransform(const float SecondsAgo)
{
	FTransform transform = GetOwner()->GetTransform();

	LastRotation = transform.GetRotation();
	LastPosition = transform.GetLocation();

	const float CurrentTime = GetServerTime();
	const float TargetTime = CurrentTime - SecondsAgo;

	float PreviousTime = 0.0f;
	float NextTime = 0.0f;

	for (int i = 0; i < FrameKeys.Count(); ++i)
	{
		const float Time = FrameKeys.ElementAt(i);
		if (PreviousTime <= TargetTime && Time >= TargetTime)
		{
			NextTime = Time;
			break;
		}

		PreviousTime = Time;
	}

	const float TimeBetweenFrames = NextTime - PreviousTime;
	const float TimeAwayFromPrevious = CurrentTime - PreviousTime;
	const float LerpProgress = TimeAwayFromPrevious / TimeBetweenFrames;
	
	const TrackedPointData& LastData = FrameData[PreviousTime];
	const TrackedPointData& NextData = NextTime != 0.0f ? FrameData[NextTime] : TrackedPointData(LastPosition, LastRotation);

	transform.SetLocation(FMath::Lerp(LastData.Position, NextData.Position, LerpProgress));
	transform.SetRotation(FQuat::Slerp(LastData.Rotation, NextData.Rotation, LerpProgress));
}

void UTrackComponent::ResetStateTransform() const
{
	//TODO: cached it
	FTransform transform = GetOwner()->GetTransform();
	transform.SetLocation(LastPosition);
	transform.SetRotation(LastRotation);
}

void UTrackComponent::StartSimulation(float time)
{
	LagCompensationManager::StartSimulate(time);
}

void UTrackComponent::EndSimulation()
{
	LagCompensationManager::EndSimulation();
}
