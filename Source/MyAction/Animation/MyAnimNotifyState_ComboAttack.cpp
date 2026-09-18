#include "Animation/MyAnimNotifyState_ComboAttack.h"
#include "Character/MyCharacter.h"

void UMyAnimNotifyState_ComboAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp)
	{
		if (AMyCharacter* OwnerCharacter = Cast<AMyCharacter>(MeshComp->GetOwner()))
		{
			OwnerCharacter->EnableComboAttack();
		}
	}
}

void UMyAnimNotifyState_ComboAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		if (AMyCharacter* OwnerCharacter = Cast<AMyCharacter>(MeshComp->GetOwner()))
		{
			OwnerCharacter->DisableComboAttack();
		}
	}
}
