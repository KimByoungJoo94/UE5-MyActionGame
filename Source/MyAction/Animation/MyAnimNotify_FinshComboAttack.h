#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MyAnimNotify_FinshComboAttack.generated.h"


UCLASS()
class MYACTION_API UMyAnimNotify_FinshComboAttack : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ResetDelay = 1.0f;
};
