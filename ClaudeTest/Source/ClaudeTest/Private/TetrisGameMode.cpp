#include "TetrisGameMode.h"
#include "TetrisBoard.h"
#include "TetrisPiece.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

ATetrisGameMode::ATetrisGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	// 初期設定
	CurrentGameState = ETetrisGameState::Menu;
	GameStats = FTetrisGameStats();
	TetrisBoard = nullptr;
	CurrentPiece = nullptr;
	NextPieceType = EPieceType::None;

	// ゲーム設定
	BaseFallSpeed = TetrisConstants::DEFAULT_FALL_SPEED;
	FallSpeed = BaseFallSpeed;
	FallTimer = 0.0f;
	bEnableGhost = true;
	MaxLevel = 15;

	// バッグシステムの初期化
	BagIndex = 0;
}

void ATetrisGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitializeGame();
}

void ATetrisGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentGameState == ETetrisGameState::Playing)
	{
		HandleAutoFall(DeltaTime);
	}
}

void ATetrisGameMode::InitializeGame()
{
	// ピースバッグの初期化
	InitializePieceBag();

	// ボードのセットアップ
	SetupBoard();

	UE_LOG(LogTemp, Warning, TEXT("Tetris Game Mode Initialized"));
}

void ATetrisGameMode::SetupBoard()
{
	// ボードアクターをスポーン
	if (!TetrisBoard)
	{
		FVector BoardLocation = FVector(0.0f, 0.0f, 0.0f);
		FRotator BoardRotation = FRotator::ZeroRotator;

		TetrisBoard = GetWorld()->SpawnActor<ATetrisBoard>(ATetrisBoard::StaticClass(), BoardLocation, BoardRotation);
		
		if (TetrisBoard)
		{
			UE_LOG(LogTemp, Warning, TEXT("Tetris Board created successfully"));
		}
	}
}

void ATetrisGameMode::StartNewGame()
{
	// ゲーム統計のリセット
	GameStats = FTetrisGameStats();
	
	// ボードのクリア
	if (TetrisBoard)
	{
		TetrisBoard->ClearBoard();
	}

	// 現在のピースをクリア
	CleanupCurrentPiece();

	// ゲーム速度のリセット
	FallSpeed = BaseFallSpeed;
	FallTimer = 0.0f;

	// 次のピースを設定
	NextPieceType = GenerateRandomPieceType();

	// 最初のピースをスポーン
	SpawnNewPiece();

	// ゲーム状態を更新
	CurrentGameState = ETetrisGameState::Playing;

	UE_LOG(LogTemp, Warning, TEXT("New game started"));
}

void ATetrisGameMode::PauseGame()
{
	if (CurrentGameState == ETetrisGameState::Playing)
	{
		CurrentGameState = ETetrisGameState::Paused;
		UE_LOG(LogTemp, Warning, TEXT("Game paused"));
	}
}

void ATetrisGameMode::ResumeGame()
{
	if (CurrentGameState == ETetrisGameState::Paused)
	{
		CurrentGameState = ETetrisGameState::Playing;
		UE_LOG(LogTemp, Warning, TEXT("Game resumed"));
	}
}

void ATetrisGameMode::EndGame()
{
	CurrentGameState = ETetrisGameState::GameOver;
	CleanupCurrentPiece();

	UE_LOG(LogTemp, Warning, TEXT("Game Over! Final Score: %d"), GameStats.Score);
}

void ATetrisGameMode::RestartGame()
{
	StartNewGame();
}

void ATetrisGameMode::SpawnNewPiece()
{
	if (!TetrisBoard)
	{
		return;
	}

	// 前のピースをクリーンアップ
	CleanupCurrentPiece();

	// 新しいピースを生成
	FVector PieceLocation = FVector(0.0f, 0.0f, 100.0f);
	CurrentPiece = GetWorld()->SpawnActor<ATetrisPiece>(ATetrisPiece::StaticClass(), PieceLocation, FRotator::ZeroRotator);

	if (CurrentPiece)
	{
		EPieceType PieceType = NextPieceType != EPieceType::None ? NextPieceType : GenerateRandomPieceType();
		CurrentPiece->InitializePiece(PieceType, TetrisBoard);

		// 次のピースを生成
		NextPieceType = GenerateRandomPieceType();

		// ゲームオーバー判定
		if (IsGameOverConditionMet())
		{
			EndGame();
			return;
		}

		GameStats.PiecesPlaced++;
		UpdateGameStats();

		UE_LOG(LogTemp, Warning, TEXT("New piece spawned: %d"), (int32)PieceType);
	}
}

EPieceType ATetrisGameMode::GenerateRandomPieceType()
{
	return GetNextPieceFromBag();
}

void ATetrisGameMode::FixCurrentPiece()
{
	if (!CurrentPiece)
	{
		return;
	}

	// ピースを固定
	CurrentPiece->FixPiece();

	// 完成したラインをチェック
	ProcessCompletedLines();

	// 新しいピースを生成
	SpawnNewPiece();
}

void ATetrisGameMode::ProcessCompletedLines()
{
	if (!TetrisBoard)
	{
		return;
	}

	TArray<int32> CompletedLines = TetrisBoard->CheckCompleteLines();
	int32 LinesCleared = CompletedLines.Num();

	if (LinesCleared > 0)
	{
		// ラインを削除
		TetrisBoard->ClearLines(CompletedLines);

		// スコアを追加
		int32 LineScore = CalculateLineScore(LinesCleared);
		AddScore(LineScore);

		// ライン数を更新
		GameStats.LinesCleared += LinesCleared;

		// レベルアップチェック
		CheckLevelUp();

		UpdateGameStats();

		UE_LOG(LogTemp, Warning, TEXT("Cleared %d lines, Score: %d"), LinesCleared, LineScore);
	}
}

int32 ATetrisGameMode::CalculateLineScore(int32 LinesCleared)
{
	int32 BaseScore = 0;

	switch (LinesCleared)
	{
	case 1:
		BaseScore = TetrisConstants::SCORE_SINGLE_LINE;
		break;
	case 2:
		BaseScore = TetrisConstants::SCORE_DOUBLE_LINE;
		break;
	case 3:
		BaseScore = TetrisConstants::SCORE_TRIPLE_LINE;
		break;
	case 4:
		BaseScore = TetrisConstants::SCORE_TETRIS;
		break;
	default:
		BaseScore = TetrisConstants::SCORE_SINGLE_LINE * LinesCleared;
		break;
	}

	return BaseScore * GameStats.Level;
}

void ATetrisGameMode::AddScore(int32 Points)
{
	GameStats.Score += Points;
	UpdateGameStats();
}

void ATetrisGameMode::CheckLevelUp()
{
	int32 NewLevel = (GameStats.LinesCleared / TetrisConstants::LINES_PER_LEVEL) + 1;
	NewLevel = FMath::Min(NewLevel, MaxLevel);

	if (NewLevel > GameStats.Level)
	{
		GameStats.Level = NewLevel;
		UpdateFallSpeed();
		UE_LOG(LogTemp, Warning, TEXT("Level Up! New Level: %d"), GameStats.Level);
	}
}

void ATetrisGameMode::UpdateFallSpeed()
{
	FallSpeed = FMath::Max(
		BaseFallSpeed - (GameStats.Level - 1) * TetrisConstants::SPEED_INCREASE_PER_LEVEL,
		TetrisConstants::MIN_FALL_SPEED
	);
}

float ATetrisGameMode::GetCurrentFallSpeed() const
{
	return FallSpeed;
}

void ATetrisGameMode::HandleAutoFall(float DeltaTime)
{
	FallTimer += DeltaTime;

	if (FallTimer >= FallSpeed)
	{
		FallTimer = 0.0f;

		if (CurrentPiece && !CurrentPiece->IsFixed())
		{
			if (!CurrentPiece->MovePiece(EMoveDirection::Down))
			{
				// ピースが下に移動できない場合は固定
				FixCurrentPiece();
			}
		}
	}
}

bool ATetrisGameMode::IsGameOverConditionMet()
{
	// ボードの最上行にブロックがある場合
	if (TetrisBoard && TetrisBoard->IsGameOver())
	{
		return true;
	}

	// 新しいピースが配置できない場合
	if (CurrentPiece)
	{
		TArray<FTetrisCoordinate> BlockPositions = CurrentPiece->GetCurrentBlockPositions();
		for (const FTetrisCoordinate& Pos : BlockPositions)
		{
			if (!TetrisBoard->IsPositionValid(Pos.X, Pos.Y))
			{
				return true;
			}
		}
	}

	return false;
}

void ATetrisGameMode::CleanupCurrentPiece()
{
	if (CurrentPiece)
	{
		CurrentPiece->Destroy();
		CurrentPiece = nullptr;
	}
}

void ATetrisGameMode::InitializePieceBag()
{
	PieceBag.Empty();
	PieceBag.Add(EPieceType::I_Piece);
	PieceBag.Add(EPieceType::O_Piece);
	PieceBag.Add(EPieceType::T_Piece);
	PieceBag.Add(EPieceType::S_Piece);
	PieceBag.Add(EPieceType::Z_Piece);
	PieceBag.Add(EPieceType::J_Piece);
	PieceBag.Add(EPieceType::L_Piece);

	// シャッフル
	for (int32 i = PieceBag.Num() - 1; i > 0; i--)
	{
		int32 j = FMath::RandRange(0, i);
		PieceBag.Swap(i, j);
	}

	BagIndex = 0;
}

EPieceType ATetrisGameMode::GetNextPieceFromBag()
{
	if (BagIndex >= PieceBag.Num())
	{
		InitializePieceBag();
	}

	return PieceBag[BagIndex++];
}

void ATetrisGameMode::UpdateGameStats()
{
	// ここでUIや他のシステムに統計更新を通知
	// 実際の実装ではデリゲートやイベントを使用
}

// 入力処理関数
void ATetrisGameMode::HandleMoveLeft()
{
	if (CurrentGameState != ETetrisGameState::Playing || !CurrentPiece)
	{
		return;
	}

	CurrentPiece->MovePiece(EMoveDirection::Left);
}

void ATetrisGameMode::HandleMoveRight()
{
	if (CurrentGameState != ETetrisGameState::Playing || !CurrentPiece)
	{
		return;
	}

	CurrentPiece->MovePiece(EMoveDirection::Right);
}

void ATetrisGameMode::HandleMoveDown()
{
	if (CurrentGameState != ETetrisGameState::Playing || !CurrentPiece)
	{
		return;
	}

	if (CurrentPiece->MovePiece(EMoveDirection::Down))
	{
		// ソフトドロップのスコア
		AddScore(1);
	}
}

void ATetrisGameMode::HandleRotate()
{
	if (CurrentGameState != ETetrisGameState::Playing || !CurrentPiece)
	{
		return;
	}

	CurrentPiece->RotatePiece(true);
}

void ATetrisGameMode::HandleHardDrop()
{
	if (CurrentGameState != ETetrisGameState::Playing || !CurrentPiece)
	{
		return;
	}

	int32 DropDistance = CurrentPiece->HardDrop();
	
	// ハードドロップのスコア
	AddScore(DropDistance * 2);

	// ピースを即座に固定
	FixCurrentPiece();
}

void ATetrisGameMode::HandlePause()
{
	if (CurrentGameState == ETetrisGameState::Playing)
	{
		PauseGame();
	}
	else if (CurrentGameState == ETetrisGameState::Paused)
	{
		ResumeGame();
	}
}

// デバッグ関数
void ATetrisGameMode::DebugAddScore(int32 Points)
{
	AddScore(Points);
}

void ATetrisGameMode::DebugSetLevel(int32 NewLevel)
{
	GameStats.Level = FMath::Clamp(NewLevel, 1, MaxLevel);
	UpdateFallSpeed();
}

void ATetrisGameMode::DebugClearBoard()
{
	if (TetrisBoard)
	{
		TetrisBoard->ClearBoard();
	}
}