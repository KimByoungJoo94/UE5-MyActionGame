#include "Assets/MyMontageActionDataAsset.h"


const TObjectPtr<UAnimMontage> UMyMontageActionDataAsset::GetMontage(const FGameplayTag& InGameplayTag, const int32 InIndex) const
{
	if (const FMyMontageGroup* FindMontageGroup = MontageGroupMap.Find(InGameplayTag))
	{
		if (FindMontageGroup->AnimMontageArray.IsValidIndex(InIndex))
		{
			return FindMontageGroup->AnimMontageArray[InIndex];
		}
	}

	return nullptr;
}
