#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TetrisTypes.generated.h"

// テトリスピースの種類
UENUM(BlueprintType)
enum class EPieceType : uint8
{
	None		UMETA(DisplayName = "None"),
	I_Piece		UMETA(DisplayName = "I Piece"),
	O_Piece		UMETA(DisplayName = "O Piece"),
	T_Piece		UMETA(DisplayName = "T Piece"),
	S_Piece		UMETA(DisplayName = "S Piece"),
	Z_Piece		UMETA(DisplayName = "Z Piece"),
	J_Piece		UMETA(DisplayName = "J Piece"),
	L_Piece		UMETA(DisplayName = "L Piece")
};

// ゲームの状態
UENUM(BlueprintType)
enum class ETetrisGameState : uint8
{
	Menu		UMETA(DisplayName = "Menu"),
	Playing		UMETA(DisplayName = "Playing"),
	Paused		UMETA(DisplayName = "Paused"),
	GameOver	UMETA(DisplayName = "Game Over")
};

// 移動方向
UENUM(BlueprintType)
enum class EMoveDirection : uint8
{
	Left		UMETA(DisplayName = "Left"),
	Right		UMETA(DisplayName = "Right"),
	Down		UMETA(DisplayName = "Down")
};

// ピースの形状データ（4x4グリッド）
USTRUCT(BlueprintType)
struct FTetrisPieceShape
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TArray<bool>> Shape;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Color;

	FTetrisPieceShape()
	{
		// 4x4グリッドを初期化
		Shape.SetNum(4);
		for (int32 i = 0; i < 4; i++)
		{
			Shape[i].SetNum(4);
			for (int32 j = 0; j < 4; j++)
			{
				Shape[i][j] = false;
			}
		}
		Color = FLinearColor::White;
	}
};

// ピースの回転状態データ
USTRUCT(BlueprintType)
struct FTetrisPieceData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPieceType PieceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTetrisPieceShape> Rotations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor PieceColor;

	FTetrisPieceData()
	{
		PieceType = EPieceType::None;
		Rotations.SetNum(4); // 4つの回転状態
		PieceColor = FLinearColor::White;
	}
};

// ゲーム統計
USTRUCT(BlueprintType)
struct FTetrisGameStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Score;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LinesCleared;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PiecesPlaced;

	FTetrisGameStats()
	{
		Score = 0;
		Level = 1;
		LinesCleared = 0;
		PiecesPlaced = 0;
	}
};

// 座標構造体
USTRUCT(BlueprintType)
struct FTetrisCoordinate
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Y;

	FTetrisCoordinate()
	{
		X = 0;
		Y = 0;
	}

	FTetrisCoordinate(int32 InX, int32 InY)
	{
		X = InX;
		Y = InY;
	}

	bool operator==(const FTetrisCoordinate& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}

	bool operator!=(const FTetrisCoordinate& Other) const
	{
		return !(*this == Other);
	}

	FTetrisCoordinate operator+(const FTetrisCoordinate& Other) const
	{
		return FTetrisCoordinate(X + Other.X, Y + Other.Y);
	}
};

// テトリスの定数
namespace TetrisConstants
{
	const int32 BOARD_WIDTH = 10;
	const int32 BOARD_HEIGHT = 20;
	const int32 BOARD_VISIBLE_HEIGHT = 20;
	const int32 BOARD_BUFFER_HEIGHT = 4;
	const int32 PIECE_SIZE = 4;
	
	const float DEFAULT_FALL_SPEED = 1.0f;
	const float MIN_FALL_SPEED = 0.1f;
	const float SPEED_INCREASE_PER_LEVEL = 0.1f;
	
	const int32 LINES_PER_LEVEL = 10;
	const int32 SCORE_SINGLE_LINE = 100;
	const int32 SCORE_DOUBLE_LINE = 300;
	const int32 SCORE_TRIPLE_LINE = 500;
	const int32 SCORE_TETRIS = 800;
}

// ピースカラー定数
namespace TetrisPieceColors
{
	const FLinearColor I_COLOR = FLinearColor(0.0f, 1.0f, 1.0f, 1.0f); // シアン
	const FLinearColor O_COLOR = FLinearColor(1.0f, 1.0f, 0.0f, 1.0f); // イエロー
	const FLinearColor T_COLOR = FLinearColor(0.5f, 0.0f, 0.5f, 1.0f); // パープル
	const FLinearColor S_COLOR = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f); // グリーン
	const FLinearColor Z_COLOR = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f); // レッド
	const FLinearColor J_COLOR = FLinearColor(0.0f, 0.0f, 1.0f, 1.0f); // ブルー
	const FLinearColor L_COLOR = FLinearColor(1.0f, 0.5f, 0.0f, 1.0f); // オレンジ
}