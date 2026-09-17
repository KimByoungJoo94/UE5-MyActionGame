#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTags.h"
#include "MyAnimNotify_EquipWeapon.generated.h"



UCLASS()
class MYACTION_API UMyAnimNotify_EquipWeapon : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag MontageActionTag;
};
