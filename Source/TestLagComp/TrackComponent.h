// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LagCompensationManager.h"
#include "Components/ActorComponent.h"
#include "GameFramework/GameStateBase.h"
#include "TrackComponent.generated.h"


struct TESTLAGCOMP_API TrackedPointData
{
	FQuat Rotation;
	FVector Position;

	TrackedPointData() {  };
	TrackedPointData(FVector pos, FQuat rot) : Rotation(rot), Position(pos) {  }
};

UCLASS(ClassGroup = Custom, editinlinenew, meta = (BlueprintSpawnableComponent, DisplayName = "Trackable Object"))
class TESTLAGCOMP_API UTrackComponent final : public UActorComponent
{
	TMap<float, TrackedPointData> FrameData;
	FixedQueue<float> FrameKeys;

	FQuat LastRotation;
	FVector LastPosition;
	
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTrackComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Network", meta = ( ClampMin = 1, ClampMax = 5) )
	float SaveHistorySeconds = 5.0f;
	
protected:
	void AddFrame();	

	UINT MaxFrameCount;
	UINT CurrentFrameSaveIndex = 0;

	friend class LagCompensationManager;
public:

	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;


	UFUNCTION(BlueprintCallable)
	float GetServerTime() const;

	UFUNCTION(BlueprintCallable)
	float GetRTT(float clientTime) const;

	UFUNCTION(BlueprintCallable)
	float GetAvgTimeBetweenPointMs();

	UFUNCTION(BlueprintCallable)
	float GetTotalHistoryTime();

	UFUNCTION(BlueprintCallable)
	void ReverseTransform(UPARAM() const float SecondsAgo);

	UFUNCTION(BlueprintCallable)
	void ResetStateTransform() const;

	UFUNCTION(BlueprintCallable)
	static void StartSimulation(UPARAM() float time);

	UFUNCTION(BlueprintCallable)
	static void EndSimulation();
};
