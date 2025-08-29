#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TetrisTypes.h"
#include "TetrisGameMode.generated.h"

class ATetrisBoard;
class ATetrisPiece;

UCLASS(BlueprintType, Blueprintable)
class CLAUDETEST_API ATetrisGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATetrisGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// ゲーム状態
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
	ETetrisGameState CurrentGameState;

	// ゲーム統計
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Stats")
	FTetrisGameStats GameStats;

	// ボード参照
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Objects")
	ATetrisBoard* TetrisBoard;

	// 現在のピース
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Objects")
	ATetrisPiece* CurrentPiece;

	// 次のピースタイプ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
	EPieceType NextPieceType;

	// 自動落下関連
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Settings")
	float FallSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
	float FallTimer;

	// ゲーム設定
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Settings")
	float BaseFallSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Settings")
	bool bEnableGhost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Settings")
	int32 MaxLevel;

public:
	// ゲーム制御
	UFUNCTION(BlueprintCallable, Category = "Game Control")
	void StartNewGame();

	UFUNCTION(BlueprintCallable, Category = "Game Control")
	void PauseGame();

	UFUNCTION(BlueprintCallable, Category = "Game Control")
	void ResumeGame();

	UFUNCTION(BlueprintCallable, Category = "Game Control")
	void EndGame();

	UFUNCTION(BlueprintCallable, Category = "Game Control")
	void RestartGame();

	// ピース管理
	UFUNCTION(BlueprintCallable, Category = "Pieces")
	void SpawnNewPiece();

	UFUNCTION(BlueprintCallable, Category = "Pieces")
	EPieceType GenerateRandomPieceType();

	UFUNCTION(BlueprintCallable, Category = "Pieces")
	void FixCurrentPiece();

	// スコアリング
	UFUNCTION(BlueprintCallable, Category = "Scoring")
	void AddScore(int32 Points);

	UFUNCTION(BlueprintCallable, Category = "Scoring")
	void ProcessCompletedLines();

	UFUNCTION(BlueprintCallable, Category = "Scoring")
	int32 CalculateLineScore(int32 LinesCleared);

	UFUNCTION(BlueprintCallable, Category = "Scoring")
	void CheckLevelUp();

	// ゲーム速度
	UFUNCTION(BlueprintCallable, Category = "Game Settings")
	void UpdateFallSpeed();

	UFUNCTION(BlueprintCallable, Category = "Game Settings")
	float GetCurrentFallSpeed() const;

	// Getter関数
	UFUNCTION(BlueprintCallable, Category = "Game State")
	ETetrisGameState GetGameState() const { return CurrentGameState; }

	UFUNCTION(BlueprintCallable, Category = "Game Stats")
	FTetrisGameStats GetGameStats() const { return GameStats; }

	UFUNCTION(BlueprintCallable, Category = "Game Objects")
	ATetrisBoard* GetTetrisBoard() const { return TetrisBoard; }

	UFUNCTION(BlueprintCallable, Category = "Game Objects")
	ATetrisPiece* GetCurrentPiece() const { return CurrentPiece; }

	UFUNCTION(BlueprintCallable, Category = "Game State")
	EPieceType GetNextPieceType() const { return NextPieceType; }

	// 入力処理（PlayerControllerから呼び出される）
	UFUNCTION(BlueprintCallable, Category = "Input")
	void HandleMoveLeft();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void HandleMoveRight();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void HandleMoveDown();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void HandleRotate();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void HandleHardDrop();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void HandlePause();

	// デバッグ関数
	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DebugAddScore(int32 Points);

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DebugSetLevel(int32 NewLevel);

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DebugClearBoard();

private:
	// 内部ヘルパー関数
	void InitializeGame();
	void SetupBoard();
	void HandleAutoFall(float DeltaTime);
	bool IsGameOverConditionMet();
	void CleanupCurrentPiece();

	// バッグシステム（テトリス標準のピース生成方式）
	TArray<EPieceType> PieceBag;
	int32 BagIndex;
	void InitializePieceBag();
	EPieceType GetNextPieceFromBag();

	// ゲーム統計更新
	void UpdateGameStats();
};

// ゲームモード関連のデリゲート
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStateChanged, ETetrisGameState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, NewScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelChanged, int32, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLinesChanged, int32, NewLines);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNextPieceChanged, EPieceType, NextPiece);