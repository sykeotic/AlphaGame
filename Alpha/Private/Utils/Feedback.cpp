#include "Feedback.h"

USoundCue* UFeedback::PickRandomSound()
{
	int8 Index = FMath::RandRange(0, SoundFX.Num() - 1);
	return SoundFX[Index];
}