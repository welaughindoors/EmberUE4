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
};