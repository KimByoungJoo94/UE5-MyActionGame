#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"


UCLASS()
class MYACTION_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	void OpenMyLevel();
};

