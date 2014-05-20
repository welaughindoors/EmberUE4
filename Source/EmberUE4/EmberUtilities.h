#include "Vector.h"

class EmberUtilities
{
public:
	static FVector Normal(FVector input)
	{
		input.Normalize();
		return input;
	}
	static float VSize(FVector input)
	{
		return input.Size();
	}

	static void Debug(int number)
	{
		char buffer[256];
		itoa(number, buffer, 10);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, buffer);
	}
	static void Debug(char * string)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, string);
	}
	static void Debug(FName string)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, string.ToString());
	}
	static void Debug(FString string)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, string);
	}
};