#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TetrisTypes.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "TetrisBoard.generated.h"

UCLASS(BlueprintType, Blueprintable)
class CLAUDETEST_API ATetrisBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	ATetrisBoard();

protected:
	virtual void BeginPlay() override;

	// ボードのグリッド状態（true = 占有, false = 空）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board")
	TArray<TArray<bool>> BoardGrid;

	// 各グリッドのピース種類
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board")
	TArray<TArray<EPieceType>> BoardPieceTypes;

	// ボードの寸法
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
	int32 BoardWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
	int32 BoardHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
	float BlockSize;

	// ブロック表示用メッシュコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rendering")
	UInstancedStaticMeshComponent* BlockMeshComponent;

	// ボード背景メッシュ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rendering")
	UStaticMeshComponent* BoardBackgroundMesh;

public:	
	virtual void Tick(float DeltaTime) override;

	// ボード初期化
	UFUNCTION(BlueprintCallable, Category = "Board")
	void InitializeBoard();

	// 位置が有効かチェック
	UFUNCTION(BlueprintCallable, Category = "Board")
	bool IsPositionValid(int32 X, int32 Y) const;

	// ブロックを配置/削除
	UFUNCTION(BlueprintCallable, Category = "Board")
	void SetBlock(int32 X, int32 Y, bool bOccupied, EPieceType PieceType = EPieceType::None);

	// 完成した行をチェック
	UFUNCTION(BlueprintCallable, Category = "Board")
	TArray<int32> CheckCompleteLines() const;

	// 行を削除
	UFUNCTION(BlueprintCallable, Category = "Board")
	void ClearLine(int32 LineY);

	// 複数行を同時削除
	UFUNCTION(BlueprintCallable, Category = "Board")
	void ClearLines(const TArray<int32>& LinesToClear);

	// ボード状態をクリア
	UFUNCTION(BlueprintCallable, Category = "Board")
	void ClearBoard();

	// 表示を更新
	UFUNCTION(BlueprintCallable, Category = "Rendering")
	void UpdateBoardDisplay();

	// 特定位置の表示を更新
	UFUNCTION(BlueprintCallable, Category = "Rendering")
	void UpdateBlockDisplay(int32 X, int32 Y);

	// ボードの状態を取得
	UFUNCTION(BlueprintCallable, Category = "Board")
	bool GetBlockState(int32 X, int32 Y) const;

	UFUNCTION(BlueprintCallable, Category = "Board")
	EPieceType GetBlockPieceType(int32 X, int32 Y) const;

	// ゲームオーバー判定
	UFUNCTION(BlueprintCallable, Category = "Board")
	bool IsGameOver() const;

	// デバッグ用：ボード状態をログ出力
	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DebugPrintBoard() const;

private:
	// 内部ヘルパー関数
	void InitializeGridArrays();
	void CreateBoardMesh();
	void UpdateSingleBlockDisplay(int32 X, int32 Y, bool bVisible, EPieceType PieceType);
	FLinearColor GetColorForPieceType(EPieceType PieceType) const;
	FVector GetWorldPositionFromGrid(int32 X, int32 Y) const;
};

// ボード関連のデリゲート
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLinesCleared, int32, LinesCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBoardCleared);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);