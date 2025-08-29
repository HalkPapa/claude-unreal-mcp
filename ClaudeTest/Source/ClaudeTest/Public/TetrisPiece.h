#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TetrisTypes.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "TetrisPiece.generated.h"

UCLASS(BlueprintType, Blueprintable)
class CLAUDETEST_API ATetrisPiece : public AActor
{
	GENERATED_BODY()
	
public:	
	ATetrisPiece();

protected:
	virtual void BeginPlay() override;

	// 現在のピースタイプ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Piece")
	EPieceType CurrentPieceType;

	// 現在の回転状態 (0-3)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Piece")
	int32 CurrentRotation;

	// ボード上での位置
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Piece")
	FTetrisCoordinate BoardPosition;

	// ピースのデータ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Piece")
	TArray<FTetrisPieceShape> PieceRotations;

	// ピースの色
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Piece")
	FLinearColor PieceColor;

	// ブロック表示用メッシュコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rendering")
	UInstancedStaticMeshComponent* BlockMeshComponent;

	// ピースが固定されているか
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Piece")
	bool bIsFixed;

	// 参照するボード
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piece")
	class ATetrisBoard* TetrisBoard;

public:	
	virtual void Tick(float DeltaTime) override;

	// ピース初期化
	UFUNCTION(BlueprintCallable, Category = "Piece")
	void InitializePiece(EPieceType PieceType, ATetrisBoard* Board);

	// ピース移動
	UFUNCTION(BlueprintCallable, Category = "Piece")
	bool MovePiece(EMoveDirection Direction);

	UFUNCTION(BlueprintCallable, Category = "Piece")
	bool MovePieceBy(const FTetrisCoordinate& Delta);

	// ピース回転
	UFUNCTION(BlueprintCallable, Category = "Piece")
	bool RotatePiece(bool bClockwise = true);

	// ハードドロップ
	UFUNCTION(BlueprintCallable, Category = "Piece")
	int32 HardDrop();

	// ピース固定
	UFUNCTION(BlueprintCallable, Category = "Piece")
	void FixPiece();

	// 現在のブロック位置を取得
	UFUNCTION(BlueprintCallable, Category = "Piece")
	TArray<FTetrisCoordinate> GetCurrentBlockPositions() const;

	// 移動可能かチェック
	UFUNCTION(BlueprintCallable, Category = "Piece")
	bool CanMoveTo(const FTetrisCoordinate& NewPosition) const;

	// 回転可能かチェック
	UFUNCTION(BlueprintCallable, Category = "Piece")
	bool CanRotateTo(int32 NewRotation) const;

	// Getter関数
	UFUNCTION(BlueprintCallable, Category = "Piece")
	EPieceType GetPieceType() const { return CurrentPieceType; }

	UFUNCTION(BlueprintCallable, Category = "Piece")
	int32 GetCurrentRotation() const { return CurrentRotation; }

	UFUNCTION(BlueprintCallable, Category = "Piece")
	FTetrisCoordinate GetBoardPosition() const { return BoardPosition; }

	UFUNCTION(BlueprintCallable, Category = "Piece")
	FLinearColor GetPieceColor() const { return PieceColor; }

	UFUNCTION(BlueprintCallable, Category = "Piece")
	bool IsFixed() const { return bIsFixed; }

	// 表示更新
	UFUNCTION(BlueprintCallable, Category = "Rendering")
	void UpdatePieceDisplay();

	// デバッグ用
	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DebugPrintPiece() const;

private:
	// 内部ヘルパー関数
	void InitializePieceData();
	void CreateIPieceData();
	void CreateOPieceData();
	void CreateTPieceData();
	void CreateSPieceData();
	void CreateZPieceData();
	void CreateJPieceData();
	void CreateLPieceData();

	TArray<FTetrisCoordinate> GetBlockPositionsForRotation(int32 Rotation) const;
	FVector GetWorldPositionFromBoard(const FTetrisCoordinate& BoardPos) const;
	void UpdateBlockDisplay();
	bool IsValidPositionOnBoard(const TArray<FTetrisCoordinate>& BlockPositions) const;

	// Wall Kick システム（回転時の位置調整）
	bool TryWallKick(int32 FromRotation, int32 ToRotation);
	TArray<FTetrisCoordinate> GetWallKickOffsets(EPieceType PieceType, int32 FromRotation, int32 ToRotation) const;
};

// ピース関連のデリゲート
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPieceMoved, ATetrisPiece*, Piece);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPieceRotated, ATetrisPiece*, Piece);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPieceFixed, ATetrisPiece*, Piece);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPieceHardDrop, ATetrisPiece*, Piece, int32, DropDistance);