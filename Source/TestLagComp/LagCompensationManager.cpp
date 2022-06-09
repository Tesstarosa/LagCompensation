// Fill out your copyright notice in the Description page of Project Settings.


#include "LagCompensationManager.h"

#include "TrackComponent.h"

TArray<UTrackComponent*> LagCompensationManager::SimulationObjects = TArray<UTrackComponent*>();

void LagCompensationManager::Register(UTrackComponent* component)
{
	SimulationObjects.Add(component);
}

void LagCompensationManager::UnRegister(UTrackComponent* component)
{
	SimulationObjects.Remove(component);
}

void LagCompensationManager::StartSimulate(float secondsAgo)
{
	Simulate(secondsAgo);
}

void LagCompensationManager::EndSimulation()
{
	Reset();
}

void LagCompensationManager::Simulate(float seceondsAgo)
{
	//TODO: Raise message about only server can simulate and work with lag compensation
	if (GIsServer == false)
	{
		return;
	}

	for (auto&& object : SimulationObjects)
	{
		object->ReverseTransform(seceondsAgo);
	}
}

void LagCompensationManager::Reset()
{
	//TODO: Raise message about only server can simulate and work with lag compensation
	if (GIsServer == false)
	{
		return;
	}
 	
	for (auto&& object : SimulationObjects)
	{
		object->ResetStateTransform();
	}
}

void LagCompensationManager::SaveFrame()
{
	for (auto&& object : SimulationObjects)
	{
		object->AddFrame();
	}
}
