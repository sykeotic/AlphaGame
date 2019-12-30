#include "Feedback.h"

USoundCue* UFeedback::PickRandomSound()
{
	if (SoundFX.Num() > 0) {
		int8 Index = FMath::RandRange(0, SoundFX.Num() - 1);
		return SoundFX[Index];
	}
	return nullptr;
}