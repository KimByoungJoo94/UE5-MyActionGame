#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "MyMontageActionDataAsset.generated.h"


class UAnimMontage;

USTRUCT(BlueprintType)
struct FMyMontageGroup
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UAnimMontage>> AnimMontageArray;

};

UCLASS()
class MYACTION_API UMyMontageActionDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	const TObjectPtr<UAnimMontage> GetMontage(const FGameplayTag& InGameplayTag, const int32 InIndex) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, FMyMontageGroup> MontageGroupMap;
};
