// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


class UTrackComponent;

class TESTLAGCOMP_API LagCompensationManager final
{
public:
	static void Register(UTrackComponent* component);
	static void UnRegister(UTrackComponent* component);


	/// Turns time back a given amount of seconds, invokes an action and turns it back
	static void StartSimulate(UPARAM() float secondsAgo);
	static void EndSimulation();
private:
	static void Simulate(float seceondsAgo);
	static void Reset();
	static void SaveFrame();

	static TArray<UTrackComponent*> SimulationObjects;
	friend class ATestLagCompGameMode;
};

template<class T>
class FixedQueue
{

public:
    int Count() const
    {
        return queueCount;
    }

    FixedQueue(int maxSize = 16)
    {
        queue = TArray<T>();
        queue.SetNum(maxSize);
        queueStart = 0;
    }

    void SetSize(int newSize)
    {
        queue.SetNum(newSize);
    }

    bool Enqueue(T t)
    {
        queue[(queueStart + queueCount) % queue.Num()] = t;
        if (queueCount + 1 <= queue.Num())
        {
            queueCount++;
            return true;
        }

        return false;
    }

    T Dequeue()
    {
        T res = queue[queueStart];
        queueStart = (queueStart + 1) % queue.Num();
        --queueCount;
        return res;
    }

    T ElementAt(int index)
    {
        return queue[(queueStart + index) % queue.Num()];
    }


private:
    TArray<T> queue;
    int queueCount = 0;
    int queueStart;
};
