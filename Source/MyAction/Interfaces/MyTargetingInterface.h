#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MyTargetingInterface.generated.h"


UINTERFACE(MinimalAPI)
class UMyTargetingInterface : public UInterface
{
	GENERATED_BODY()
};


class MYACTION_API IMyTargetingInterface
{
	GENERATED_BODY()

public:
	virtual void OnTargeting(bool bInTargeting) = 0;
	virtual bool CanTargeting() = 0;
};
