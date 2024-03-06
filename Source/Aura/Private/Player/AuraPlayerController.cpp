// Copyright Sosil202403


#include "Player/AuraPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);

	// 마우스 커서가 항상 보이게
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	// 마우스 커서가 뷰포트 내부 한정 X
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	
}

void AAuraPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CursorTrace();
	
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if(!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/*
	 * Line trace from Cursor. There are several scenarios.
	 *	1. LastActor is null && ThisActor is null
	 *		- Do Nothing
	 *	2. LastActor is null && ThisActor is valid
	 *		- Highlight This Actor
	 *	3. LastActor is valid && ThisActor is null
	 *		- UnHighLight Last Actor
	 *	4. Both Actor is valid But LastActor != ThisActor
	 *		- UnHighLight Last Actor && Highlight This Actor
	 *	5. Both Actor is valid && LastActor is ThisActor
	 *		- Do Nothing
	 */

	if(!LastActor)
	{
		if(ThisActor)
		{
			// Case 2
			ThisActor->HighlightActor();
		}
		// Case 1
	}
	else
	{
		if(ThisActor)
		{
			if(ThisActor != LastActor)
			{
				// Case 5
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			// Case 4
		}
		else
		{
			// Case 3
			LastActor->UnHighlightActor();
		}
	}

	
}
