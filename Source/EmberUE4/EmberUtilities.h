#pragma once
#include "Vector.h"

class EmberUtilities
{
public:

	// Normal Vector from Vector
	static FVector Normal(FVector input)
	{
		input.Normalize();
		return input;
	}

	// Size of Vector from Vector
	static float VSize(FVector input)
	{
		return input.Size();
	}

	// Print number
	static void Debug(int number)
	{
		char buffer[256];
		itoa(number, buffer, 10);
		if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, buffer);
	}

	// Print char*
	static void Debug(char * string)
	{
		if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, string);
	}

	// Print FNAME
	static void Debug(FName string)
	{
		if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, string.ToString());
	}

	// Print FSTRING
	static void Debug(FString string)
	{
		if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, string);
	}

	/*
        Is Index out of bounds?
            If yes, it is NOT SAFE TO USE
            if no:
            	Is Index below 0?
                	If yes, it is NOT SAFE TO USE
                	if no:
                		it is safe to use
    */
	static bool SafeToUseInArray(int iContender, int iLimiter)
	{
		return iContender > iLimiter ? false : iContender < 0 ? false : true;
	}
};