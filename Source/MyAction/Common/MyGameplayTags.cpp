#include "Common/MyGamePlayTags.h"


#define MY_DEFINE_GAMEPLAY_TAG(Category, SubCategory, Name) UE_DEFINE_GAMEPLAY_TAG(Category##_##SubCategory##_##Name, #Category"."#SubCategory"."#Name)

namespace MyGameplayTags
{
	MY_DEFINE_GAMEPLAY_TAG(Character, State, Attacking);
	MY_DEFINE_GAMEPLAY_TAG(Character, State, Rolling);
	MY_DEFINE_GAMEPLAY_TAG(Character, State, GeneralAction);
	
	MY_DEFINE_GAMEPLAY_TAG(Character, Action, Equip);
	MY_DEFINE_GAMEPLAY_TAG(Character, Action, Unequip);
	
	MY_DEFINE_GAMEPLAY_TAG(Character, Attack, Light);
	MY_DEFINE_GAMEPLAY_TAG(Character, Attack, Running);
	MY_DEFINE_GAMEPLAY_TAG(Character, Attack, Special);
	MY_DEFINE_GAMEPLAY_TAG(Character, Attack, Heavy);
}

#undef MY_GAMEPLAY_TAG