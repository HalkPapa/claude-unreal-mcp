#include "TetrisPlayerController.h"
#include "TetrisGameMode.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Engine/Engine.h"

ATetrisPlayerController::ATetrisPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	// デフォルト設定
	RepeatDelay = 0.3f;    // 最初のリピートまでの遅延
	RepeatRate = 0.05f;    // リピート間隔
	bInputEnabled = true;

	// 入力状態の初期化
	bIsMovingLeft = false;
	bIsMovingRight = false;
	bIsMovingDown = false;

	LeftMoveTimer = 0.0f;
	RightMoveTimer = 0.0f;
	DownMoveTimer = 0.0f;

	TetrisGameMode = nullptr;
}

void ATetrisPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Enhanced Input Subsystem の設定
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// ゲームモードの参照を取得
	CacheGameModeReference();
}

void ATetrisPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Enhanced Input Component にキャスト
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	if (EnhancedInputComponent)
	{
		// 移動入力（継続入力対応）
		if (MoveLeftAction)
		{
			EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Started, this, &ATetrisPlayerController::OnMoveLeftStarted);
			EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Completed, this, &ATetrisPlayerController::OnMoveLeftCompleted);
			EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Canceled, this, &ATetrisPlayerController::OnMoveLeftCompleted);
		}

		if (MoveRightAction)
		{
			EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Started, this, &ATetrisPlayerController::OnMoveRightStarted);
			EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Completed, this, &ATetrisPlayerController::OnMoveRightCompleted);
			EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Canceled, this, &ATetrisPlayerController::OnMoveRightCompleted);
		}

		if (MoveDownAction)
		{
			EnhancedInputComponent->BindAction(MoveDownAction, ETriggerEvent::Started, this, &ATetrisPlayerController::OnMoveDownStarted);
			EnhancedInputComponent->BindAction(MoveDownAction, ETriggerEvent::Completed, this, &ATetrisPlayerController::OnMoveDownCompleted);
			EnhancedInputComponent->BindAction(MoveDownAction, ETriggerEvent::Canceled, this, &ATetrisPlayerController::OnMoveDownCompleted);
		}

		// ワンショット入力
		if (RotateAction)
		{
			EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ATetrisPlayerController::OnRotate);
		}

		if (HardDropAction)
		{
			EnhancedInputComponent->BindAction(HardDropAction, ETriggerEvent::Triggered, this, &ATetrisPlayerController::OnHardDrop);
		}

		if (PauseAction)
		{
			EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &ATetrisPlayerController::OnPause);
		}

		if (RestartAction)
		{
			EnhancedInputComponent->BindAction(RestartAction, ETriggerEvent::Triggered, this, &ATetrisPlayerController::OnRestart);
		}
	}
}

void ATetrisPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInputEnabled)
	{
		HandleRepeatInput(DeltaTime);
	}
}

void ATetrisPlayerController::HandleRepeatInput(float DeltaTime)
{
	ProcessLeftMovement(DeltaTime);
	ProcessRightMovement(DeltaTime);
	ProcessDownMovement(DeltaTime);
}

void ATetrisPlayerController::ProcessLeftMovement(float DeltaTime)
{
	if (!bIsMovingLeft)
	{
		LeftMoveTimer = 0.0f;
		return;
	}

	LeftMoveTimer += DeltaTime;

	// 最初の移動後、リピート処理
	if (LeftMoveTimer >= RepeatDelay)
	{
		float TimeSinceFirstRepeat = LeftMoveTimer - RepeatDelay;
		int32 RepeatCount = FMath::FloorToInt(TimeSinceFirstRepeat / RepeatRate);

		// 前回のRepeatCountから変化があった場合のみ移動
		static int32 LastLeftRepeatCount = -1;
		if (RepeatCount > LastLeftRepeatCount)
		{
			LastLeftRepeatCount = RepeatCount;
			if (TetrisGameMode)
			{
				TetrisGameMode->HandleMoveLeft();
			}
		}
	}
}

void ATetrisPlayerController::ProcessRightMovement(float DeltaTime)
{
	if (!bIsMovingRight)
	{
		RightMoveTimer = 0.0f;
		return;
	}

	RightMoveTimer += DeltaTime;

	if (RightMoveTimer >= RepeatDelay)
	{
		float TimeSinceFirstRepeat = RightMoveTimer - RepeatDelay;
		int32 RepeatCount = FMath::FloorToInt(TimeSinceFirstRepeat / RepeatRate);

		static int32 LastRightRepeatCount = -1;
		if (RepeatCount > LastRightRepeatCount)
		{
			LastRightRepeatCount = RepeatCount;
			if (TetrisGameMode)
			{
				TetrisGameMode->HandleMoveRight();
			}
		}
	}
}

void ATetrisPlayerController::ProcessDownMovement(float DeltaTime)
{
	if (!bIsMovingDown)
	{
		DownMoveTimer = 0.0f;
		return;
	}

	DownMoveTimer += DeltaTime;

	if (DownMoveTimer >= RepeatRate) // 下移動は高速リピート
	{
		DownMoveTimer = 0.0f;
		if (TetrisGameMode)
		{
			TetrisGameMode->HandleMoveDown();
		}
	}
}

// 入力アクション処理関数
void ATetrisPlayerController::OnMoveLeft(const FInputActionValue& Value)
{
	if (!bInputEnabled || !TetrisGameMode) return;
	TetrisGameMode->HandleMoveLeft();
}

void ATetrisPlayerController::OnMoveRight(const FInputActionValue& Value)
{
	if (!bInputEnabled || !TetrisGameMode) return;
	TetrisGameMode->HandleMoveRight();
}

void ATetrisPlayerController::OnMoveDown(const FInputActionValue& Value)
{
	if (!bInputEnabled || !TetrisGameMode) return;
	TetrisGameMode->HandleMoveDown();
}

void ATetrisPlayerController::OnRotate(const FInputActionValue& Value)
{
	if (!bInputEnabled || !TetrisGameMode) return;
	TetrisGameMode->HandleRotate();
}

void ATetrisPlayerController::OnHardDrop(const FInputActionValue& Value)
{
	if (!bInputEnabled || !TetrisGameMode) return;
	TetrisGameMode->HandleHardDrop();
}

void ATetrisPlayerController::OnPause(const FInputActionValue& Value)
{
	if (!TetrisGameMode) return;
	TetrisGameMode->HandlePause();
}

void ATetrisPlayerController::OnRestart(const FInputActionValue& Value)
{
	if (!TetrisGameMode) return;
	TetrisGameMode->RestartGame();
}

// 入力開始/終了処理
void ATetrisPlayerController::OnMoveLeftStarted(const FInputActionValue& Value)
{
	if (!bInputEnabled) return;
	
	bIsMovingLeft = true;
	LeftMoveTimer = 0.0f;
	
	// 即座に1回移動
	if (TetrisGameMode)
	{
		TetrisGameMode->HandleMoveLeft();
	}
}

void ATetrisPlayerController::OnMoveLeftCompleted(const FInputActionValue& Value)
{
	bIsMovingLeft = false;
	LeftMoveTimer = 0.0f;
}

void ATetrisPlayerController::OnMoveRightStarted(const FInputActionValue& Value)
{
	if (!bInputEnabled) return;
	
	bIsMovingRight = true;
	RightMoveTimer = 0.0f;
	
	if (TetrisGameMode)
	{
		TetrisGameMode->HandleMoveRight();
	}
}

void ATetrisPlayerController::OnMoveRightCompleted(const FInputActionValue& Value)
{
	bIsMovingRight = false;
	RightMoveTimer = 0.0f;
}

void ATetrisPlayerController::OnMoveDownStarted(const FInputActionValue& Value)
{
	if (!bInputEnabled) return;
	
	bIsMovingDown = true;
	DownMoveTimer = 0.0f;
	
	if (TetrisGameMode)
	{
		TetrisGameMode->HandleMoveDown();
	}
}

void ATetrisPlayerController::OnMoveDownCompleted(const FInputActionValue& Value)
{
	bIsMovingDown = false;
	DownMoveTimer = 0.0f;
}

void ATetrisPlayerController::CacheGameModeReference()
{
	if (GetWorld())
	{
		TetrisGameMode = Cast<ATetrisGameMode>(GetWorld()->GetAuthGameMode());
		if (TetrisGameMode)
		{
			UE_LOG(LogTemp, Warning, TEXT("Tetris PlayerController connected to GameMode"));
		}
	}
}