// Copyright Sosil202403


#include "AbilitySystem/Ability/AuraGameplayAbility.h"

#include "Kismet/KismetSystemLibrary.h"

void UAuraGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UKismetSystemLibrary::PrintString(
		this,
		FString("ActivateAbility (C++)"),
		true,
		true,
		FColor::Blue,
		3
		);
}
