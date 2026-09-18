#include "Animation/MyAnimNotify_FinshComboAttack.h"
#include "Character/MyCharacter.h"

void UMyAnimNotify_FinshComboAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		if (AMyCharacter* OwnerCharacter = Cast<AMyCharacter>(MeshComp->GetOwner()))
		{
			OwnerCharacter->FinishComboAttack(ResetDelay);
		}
	}
}
