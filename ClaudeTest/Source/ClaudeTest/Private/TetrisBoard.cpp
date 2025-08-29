#include "TetrisBoard.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInterface.h"
#include "Engine/Engine.h"

ATetrisBoard::ATetrisBoard()
{
	PrimaryActorTick.bCanEverTick = true;

	// デフォルトの寸法を設定
	BoardWidth = TetrisConstants::BOARD_WIDTH;
	BoardHeight = TetrisConstants::BOARD_HEIGHT;
	BlockSize = 100.0f; // 100 Unreal units per block

	// ルートコンポーネントの設定
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// ボード背景メッシュの設定
	BoardBackgroundMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoardBackgroundMesh"));
	BoardBackgroundMesh->SetupAttachment(RootComponent);

	// ブロック表示用のInstanced Static Mesh Component
	BlockMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("BlockMeshComponent"));
	BlockMeshComponent->SetupAttachment(RootComponent);

	// デフォルトメッシュとマテリアルの設定（エディタで設定可能）
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Engine/BasicShapes/Cube"));
	if (CubeMeshAsset.Succeeded())
	{
		BlockMeshComponent->SetStaticMesh(CubeMeshAsset.Object);
	}
}

void ATetrisBoard::BeginPlay()
{
	Super::BeginPlay();
	InitializeBoard();
}

void ATetrisBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATetrisBoard::InitializeBoard()
{
	// グリッド配列の初期化
	InitializeGridArrays();
	
	// ボードメッシュの作成
	CreateBoardMesh();
	
	// 表示の更新
	UpdateBoardDisplay();

	UE_LOG(LogTemp, Warning, TEXT("Tetris Board Initialized: %dx%d"), BoardWidth, BoardHeight);
}

void ATetrisBoard::InitializeGridArrays()
{
	// ボードグリッドの初期化
	BoardGrid.Empty();
	BoardPieceTypes.Empty();

	BoardGrid.SetNum(BoardHeight);
	BoardPieceTypes.SetNum(BoardHeight);

	for (int32 Y = 0; Y < BoardHeight; Y++)
	{
		BoardGrid[Y].SetNum(BoardWidth);
		BoardPieceTypes[Y].SetNum(BoardWidth);

		for (int32 X = 0; X < BoardWidth; X++)
		{
			BoardGrid[Y][X] = false;
			BoardPieceTypes[Y][X] = EPieceType::None;
		}
	}
}

void ATetrisBoard::CreateBoardMesh()
{
	if (!BoardBackgroundMesh)
	{
		return;
	}

	// ボード背景のスケールを設定
	FVector BackgroundScale = FVector(
		BoardWidth * BlockSize / 100.0f,
		BoardHeight * BlockSize / 100.0f,
		0.1f
	);
	BoardBackgroundMesh->SetWorldScale3D(BackgroundScale);

	// ボード背景の位置を調整
	FVector BackgroundPosition = FVector(
		(BoardWidth - 1) * BlockSize * 0.5f,
		(BoardHeight - 1) * BlockSize * 0.5f,
		-50.0f
	);
	BoardBackgroundMesh->SetWorldLocation(BackgroundPosition);
}

bool ATetrisBoard::IsPositionValid(int32 X, int32 Y) const
{
	// 範囲内チェック
	if (X < 0 || X >= BoardWidth || Y < 0 || Y >= BoardHeight)
	{
		return false;
	}

	// 既に占有されていないかチェック
	return !BoardGrid[Y][X];
}

void ATetrisBoard::SetBlock(int32 X, int32 Y, bool bOccupied, EPieceType PieceType)
{
	if (X < 0 || X >= BoardWidth || Y < 0 || Y >= BoardHeight)
	{
		return;
	}

	BoardGrid[Y][X] = bOccupied;
	BoardPieceTypes[Y][X] = bOccupied ? PieceType : EPieceType::None;

	// 表示を更新
	UpdateBlockDisplay(X, Y);
}

TArray<int32> ATetrisBoard::CheckCompleteLines() const
{
	TArray<int32> CompleteLines;

	for (int32 Y = 0; Y < BoardHeight; Y++)
	{
		bool bLineComplete = true;
		for (int32 X = 0; X < BoardWidth; X++)
		{
			if (!BoardGrid[Y][X])
			{
				bLineComplete = false;
				break;
			}
		}

		if (bLineComplete)
		{
			CompleteLines.Add(Y);
		}
	}

	return CompleteLines;
}

void ATetrisBoard::ClearLine(int32 LineY)
{
	if (LineY < 0 || LineY >= BoardHeight)
	{
		return;
	}

	// 指定された行より上のすべての行を1つ下に移動
	for (int32 Y = LineY; Y > 0; Y--)
	{
		for (int32 X = 0; X < BoardWidth; X++)
		{
			BoardGrid[Y][X] = BoardGrid[Y - 1][X];
			BoardPieceTypes[Y][X] = BoardPieceTypes[Y - 1][X];
		}
	}

	// 最上行をクリア
	for (int32 X = 0; X < BoardWidth; X++)
	{
		BoardGrid[0][X] = false;
		BoardPieceTypes[0][X] = EPieceType::None;
	}

	// 表示を更新
	UpdateBoardDisplay();
}

void ATetrisBoard::ClearLines(const TArray<int32>& LinesToClear)
{
	// 行を下から上の順序でソート
	TArray<int32> SortedLines = LinesToClear;
	SortedLines.Sort([](const int32& A, const int32& B) { return A > B; });

	// 各行を削除
	for (int32 LineY : SortedLines)
	{
		ClearLine(LineY);
	}

	UE_LOG(LogTemp, Warning, TEXT("Cleared %d lines"), LinesToClear.Num());
}

void ATetrisBoard::ClearBoard()
{
	InitializeGridArrays();
	UpdateBoardDisplay();
}

void ATetrisBoard::UpdateBoardDisplay()
{
	if (!BlockMeshComponent)
	{
		return;
	}

	// すべてのインスタンスをクリア
	BlockMeshComponent->ClearInstances();

	// ボード全体を再描画
	for (int32 Y = 0; Y < BoardHeight; Y++)
	{
		for (int32 X = 0; X < BoardWidth; X++)
		{
			if (BoardGrid[Y][X])
			{
				UpdateSingleBlockDisplay(X, Y, true, BoardPieceTypes[Y][X]);
			}
		}
	}
}

void ATetrisBoard::UpdateBlockDisplay(int32 X, int32 Y)
{
	if (X < 0 || X >= BoardWidth || Y < 0 || Y >= BoardHeight)
	{
		return;
	}

	UpdateSingleBlockDisplay(X, Y, BoardGrid[Y][X], BoardPieceTypes[Y][X]);
}

void ATetrisBoard::UpdateSingleBlockDisplay(int32 X, int32 Y, bool bVisible, EPieceType PieceType)
{
	if (!BlockMeshComponent)
	{
		return;
	}

	FVector BlockPosition = GetWorldPositionFromGrid(X, Y);
	FTransform BlockTransform(FRotator::ZeroRotator, BlockPosition, FVector(BlockSize / 100.0f));

	if (bVisible)
	{
		BlockMeshComponent->AddInstance(BlockTransform);
		// TODO: ピースタイプに基づいて色を設定する機能を追加
	}
}

FLinearColor ATetrisBoard::GetColorForPieceType(EPieceType PieceType) const
{
	switch (PieceType)
	{
	case EPieceType::I_Piece:
		return TetrisPieceColors::I_COLOR;
	case EPieceType::O_Piece:
		return TetrisPieceColors::O_COLOR;
	case EPieceType::T_Piece:
		return TetrisPieceColors::T_COLOR;
	case EPieceType::S_Piece:
		return TetrisPieceColors::S_COLOR;
	case EPieceType::Z_Piece:
		return TetrisPieceColors::Z_COLOR;
	case EPieceType::J_Piece:
		return TetrisPieceColors::J_COLOR;
	case EPieceType::L_Piece:
		return TetrisPieceColors::L_COLOR;
	default:
		return FLinearColor::White;
	}
}

FVector ATetrisBoard::GetWorldPositionFromGrid(int32 X, int32 Y) const
{
	return FVector(X * BlockSize, Y * BlockSize, 0.0f);
}

bool ATetrisBoard::GetBlockState(int32 X, int32 Y) const
{
	if (X < 0 || X >= BoardWidth || Y < 0 || Y >= BoardHeight)
	{
		return false;
	}
	return BoardGrid[Y][X];
}

EPieceType ATetrisBoard::GetBlockPieceType(int32 X, int32 Y) const
{
	if (X < 0 || X >= BoardWidth || Y < 0 || Y >= BoardHeight)
	{
		return EPieceType::None;
	}
	return BoardPieceTypes[Y][X];
}

bool ATetrisBoard::IsGameOver() const
{
	// 最上行にブロックがあるかチェック
	for (int32 X = 0; X < BoardWidth; X++)
	{
		if (BoardGrid[0][X])
		{
			return true;
		}
	}
	return false;
}

void ATetrisBoard::DebugPrintBoard() const
{
	FString DebugString = TEXT("Board State:\n");
	
	for (int32 Y = 0; Y < BoardHeight; Y++)
	{
		FString LineString = TEXT("");
		for (int32 X = 0; X < BoardWidth; X++)
		{
			LineString += BoardGrid[Y][X] ? TEXT("■") : TEXT("□");
		}
		DebugString += LineString + TEXT("\n");
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *DebugString);
}