#include "TetrisPiece.h"
#include "TetrisBoard.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"

ATetrisPiece::ATetrisPiece()
{
	PrimaryActorTick.bCanEverTick = true;

	// ルートコンポーネントの設定
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// ブロック表示用のInstanced Static Mesh Component
	BlockMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("BlockMeshComponent"));
	BlockMeshComponent->SetupAttachment(RootComponent);

	// デフォルトメッシュの設定
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Engine/BasicShapes/Cube"));
	if (CubeMeshAsset.Succeeded())
	{
		BlockMeshComponent->SetStaticMesh(CubeMeshAsset.Object);
	}

	// 初期値の設定
	CurrentPieceType = EPieceType::None;
	CurrentRotation = 0;
	BoardPosition = FTetrisCoordinate(0, 0);
	PieceColor = FLinearColor::White;
	bIsFixed = false;
	TetrisBoard = nullptr;

	// ピースデータの初期化
	PieceRotations.SetNum(4);
}

void ATetrisPiece::BeginPlay()
{
	Super::BeginPlay();
}

void ATetrisPiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATetrisPiece::InitializePiece(EPieceType PieceType, ATetrisBoard* Board)
{
	CurrentPieceType = PieceType;
	CurrentRotation = 0;
	TetrisBoard = Board;
	bIsFixed = false;

	// ピースデータの初期化
	InitializePieceData();

	// 初期位置の設定（ボード上部中央）
	BoardPosition = FTetrisCoordinate(TetrisConstants::BOARD_WIDTH / 2 - 2, 0);

	// 表示の更新
	UpdatePieceDisplay();

	UE_LOG(LogTemp, Warning, TEXT("Initialized Piece Type: %d at position (%d, %d)"), 
		(int32)PieceType, BoardPosition.X, BoardPosition.Y);
}

bool ATetrisPiece::MovePiece(EMoveDirection Direction)
{
	FTetrisCoordinate Delta(0, 0);

	switch (Direction)
	{
	case EMoveDirection::Left:
		Delta = FTetrisCoordinate(-1, 0);
		break;
	case EMoveDirection::Right:
		Delta = FTetrisCoordinate(1, 0);
		break;
	case EMoveDirection::Down:
		Delta = FTetrisCoordinate(0, 1);
		break;
	}

	return MovePieceBy(Delta);
}

bool ATetrisPiece::MovePieceBy(const FTetrisCoordinate& Delta)
{
	if (bIsFixed || !TetrisBoard)
	{
		return false;
	}

	FTetrisCoordinate NewPosition = BoardPosition + Delta;

	if (CanMoveTo(NewPosition))
	{
		BoardPosition = NewPosition;
		UpdatePieceDisplay();
		return true;
	}

	// 下方向に移動できない場合はピースを固定
	if (Delta.Y > 0)
	{
		FixPiece();
	}

	return false;
}

bool ATetrisPiece::RotatePiece(bool bClockwise)
{
	if (bIsFixed || !TetrisBoard)
	{
		return false;
	}

	int32 NewRotation = CurrentRotation;
	if (bClockwise)
	{
		NewRotation = (NewRotation + 1) % 4;
	}
	else
	{
		NewRotation = (NewRotation + 3) % 4; // -1 % 4 in positive form
	}

	if (CanRotateTo(NewRotation))
	{
		CurrentRotation = NewRotation;
		UpdatePieceDisplay();
		return true;
	}

	// Wall Kickを試行
	if (TryWallKick(CurrentRotation, NewRotation))
	{
		CurrentRotation = NewRotation;
		UpdatePieceDisplay();
		return true;
	}

	return false;
}

int32 ATetrisPiece::HardDrop()
{
	if (bIsFixed || !TetrisBoard)
	{
		return 0;
	}

	int32 DropDistance = 0;
	while (MovePiece(EMoveDirection::Down))
	{
		DropDistance++;
	}

	return DropDistance;
}

void ATetrisPiece::FixPiece()
{
	if (bIsFixed || !TetrisBoard)
	{
		return;
	}

	bIsFixed = true;

	// ボードにピースを固定
	TArray<FTetrisCoordinate> BlockPositions = GetCurrentBlockPositions();
	for (const FTetrisCoordinate& BlockPos : BlockPositions)
	{
		TetrisBoard->SetBlock(BlockPos.X, BlockPos.Y, true, CurrentPieceType);
	}

	// 表示を非表示に
	BlockMeshComponent->ClearInstances();

	UE_LOG(LogTemp, Warning, TEXT("Piece fixed at position (%d, %d)"), BoardPosition.X, BoardPosition.Y);
}

TArray<FTetrisCoordinate> ATetrisPiece::GetCurrentBlockPositions() const
{
	return GetBlockPositionsForRotation(CurrentRotation);
}

TArray<FTetrisCoordinate> ATetrisPiece::GetBlockPositionsForRotation(int32 Rotation) const
{
	TArray<FTetrisCoordinate> BlockPositions;

	if (Rotation < 0 || Rotation >= PieceRotations.Num())
	{
		return BlockPositions;
	}

	const FTetrisPieceShape& Shape = PieceRotations[Rotation];

	for (int32 Y = 0; Y < 4; Y++)
	{
		for (int32 X = 0; X < 4; X++)
		{
			if (Shape.Shape.IsValidIndex(Y) && Shape.Shape[Y].IsValidIndex(X) && Shape.Shape[Y][X])
			{
				FTetrisCoordinate WorldPos = BoardPosition + FTetrisCoordinate(X, Y);
				BlockPositions.Add(WorldPos);
			}
		}
	}

	return BlockPositions;
}

bool ATetrisPiece::CanMoveTo(const FTetrisCoordinate& NewPosition) const
{
	if (!TetrisBoard)
	{
		return false;
	}

	FTetrisCoordinate OriginalPosition = BoardPosition;
	const_cast<ATetrisPiece*>(this)->BoardPosition = NewPosition;

	TArray<FTetrisCoordinate> BlockPositions = GetCurrentBlockPositions();
	bool bCanMove = IsValidPositionOnBoard(BlockPositions);

	const_cast<ATetrisPiece*>(this)->BoardPosition = OriginalPosition;

	return bCanMove;
}

bool ATetrisPiece::CanRotateTo(int32 NewRotation) const
{
	if (!TetrisBoard || NewRotation < 0 || NewRotation >= 4)
	{
		return false;
	}

	TArray<FTetrisCoordinate> BlockPositions = GetBlockPositionsForRotation(NewRotation);
	return IsValidPositionOnBoard(BlockPositions);
}

bool ATetrisPiece::IsValidPositionOnBoard(const TArray<FTetrisCoordinate>& BlockPositions) const
{
	if (!TetrisBoard)
	{
		return false;
	}

	for (const FTetrisCoordinate& BlockPos : BlockPositions)
	{
		if (!TetrisBoard->IsPositionValid(BlockPos.X, BlockPos.Y))
		{
			return false;
		}
	}

	return true;
}

void ATetrisPiece::UpdatePieceDisplay()
{
	if (!BlockMeshComponent || bIsFixed)
	{
		return;
	}

	// 既存のインスタンスをクリア
	BlockMeshComponent->ClearInstances();

	// 現在のピースのブロック位置を取得
	TArray<FTetrisCoordinate> BlockPositions = GetCurrentBlockPositions();

	// 各ブロックを表示
	for (const FTetrisCoordinate& BlockPos : BlockPositions)
	{
		FVector WorldPosition = GetWorldPositionFromBoard(BlockPos);
		FTransform BlockTransform(FRotator::ZeroRotator, WorldPosition, FVector(1.0f));
		BlockMeshComponent->AddInstance(BlockTransform);
	}
}

FVector ATetrisPiece::GetWorldPositionFromBoard(const FTetrisCoordinate& BoardPos) const
{
	if (!TetrisBoard)
	{
		return FVector::ZeroVector;
	}

	// ボードのブロックサイズを使用して世界座標を計算
	float BlockSize = 100.0f; // TetrisBoard->GetBlockSize() if available
	return FVector(BoardPos.X * BlockSize, BoardPos.Y * BlockSize, 50.0f);
}

bool ATetrisPiece::TryWallKick(int32 FromRotation, int32 ToRotation)
{
	TArray<FTetrisCoordinate> WallKickOffsets = GetWallKickOffsets(CurrentPieceType, FromRotation, ToRotation);

	for (const FTetrisCoordinate& Offset : WallKickOffsets)
	{
		FTetrisCoordinate TestPosition = BoardPosition + Offset;
		if (CanMoveTo(TestPosition))
		{
			BoardPosition = TestPosition;
			return true;
		}
	}

	return false;
}

TArray<FTetrisCoordinate> ATetrisPiece::GetWallKickOffsets(EPieceType PieceType, int32 FromRotation, int32 ToRotation) const
{
	TArray<FTetrisCoordinate> Offsets;

	// 簡単なWall Kick実装（SRS準拠ではないが基本的な機能を提供）
	if (PieceType == EPieceType::I_Piece)
	{
		// I-Piece用の特別なWall Kick
		Offsets.Add(FTetrisCoordinate(0, 0));
		Offsets.Add(FTetrisCoordinate(-1, 0));
		Offsets.Add(FTetrisCoordinate(1, 0));
		Offsets.Add(FTetrisCoordinate(0, -1));
	}
	else if (PieceType != EPieceType::O_Piece)
	{
		// O-Piece以外の標準的なWall Kick
		Offsets.Add(FTetrisCoordinate(0, 0));
		Offsets.Add(FTetrisCoordinate(-1, 0));
		Offsets.Add(FTetrisCoordinate(1, 0));
		Offsets.Add(FTetrisCoordinate(0, -1));
		Offsets.Add(FTetrisCoordinate(-1, -1));
		Offsets.Add(FTetrisCoordinate(1, -1));
	}

	return Offsets;
}

void ATetrisPiece::InitializePieceData()
{
	// 各ピース形状の初期化
	PieceRotations.SetNum(4);
	for (int32 i = 0; i < 4; i++)
	{
		PieceRotations[i] = FTetrisPieceShape();
	}

	switch (CurrentPieceType)
	{
	case EPieceType::I_Piece:
		CreateIPieceData();
		PieceColor = TetrisPieceColors::I_COLOR;
		break;
	case EPieceType::O_Piece:
		CreateOPieceData();
		PieceColor = TetrisPieceColors::O_COLOR;
		break;
	case EPieceType::T_Piece:
		CreateTPieceData();
		PieceColor = TetrisPieceColors::T_COLOR;
		break;
	case EPieceType::S_Piece:
		CreateSPieceData();
		PieceColor = TetrisPieceColors::S_COLOR;
		break;
	case EPieceType::Z_Piece:
		CreateZPieceData();
		PieceColor = TetrisPieceColors::Z_COLOR;
		break;
	case EPieceType::J_Piece:
		CreateJPieceData();
		PieceColor = TetrisPieceColors::J_COLOR;
		break;
	case EPieceType::L_Piece:
		CreateLPieceData();
		PieceColor = TetrisPieceColors::L_COLOR;
		break;
	default:
		break;
	}
}

void ATetrisPiece::CreateIPieceData()
{
	// I-Piece (■■■■)
	// Rotation 0: Horizontal
	PieceRotations[0].Shape[1][0] = true;
	PieceRotations[0].Shape[1][1] = true;
	PieceRotations[0].Shape[1][2] = true;
	PieceRotations[0].Shape[1][3] = true;

	// Rotation 1: Vertical
	PieceRotations[1].Shape[0][2] = true;
	PieceRotations[1].Shape[1][2] = true;
	PieceRotations[1].Shape[2][2] = true;
	PieceRotations[1].Shape[3][2] = true;

	// Rotation 2: Horizontal
	PieceRotations[2].Shape[2][0] = true;
	PieceRotations[2].Shape[2][1] = true;
	PieceRotations[2].Shape[2][2] = true;
	PieceRotations[2].Shape[2][3] = true;

	// Rotation 3: Vertical
	PieceRotations[3].Shape[0][1] = true;
	PieceRotations[3].Shape[1][1] = true;
	PieceRotations[3].Shape[2][1] = true;
	PieceRotations[3].Shape[3][1] = true;
}

void ATetrisPiece::CreateOPieceData()
{
	// O-Piece (正方形) - 回転しても同じ
	for (int32 Rotation = 0; Rotation < 4; Rotation++)
	{
		PieceRotations[Rotation].Shape[0][1] = true;
		PieceRotations[Rotation].Shape[0][2] = true;
		PieceRotations[Rotation].Shape[1][1] = true;
		PieceRotations[Rotation].Shape[1][2] = true;
	}
}

void ATetrisPiece::CreateTPieceData()
{
	// T-Piece
	// Rotation 0: T shape upward
	PieceRotations[0].Shape[0][1] = true;
	PieceRotations[0].Shape[1][0] = true;
	PieceRotations[0].Shape[1][1] = true;
	PieceRotations[0].Shape[1][2] = true;

	// Rotation 1: T shape right
	PieceRotations[1].Shape[0][1] = true;
	PieceRotations[1].Shape[1][1] = true;
	PieceRotations[1].Shape[1][2] = true;
	PieceRotations[1].Shape[2][1] = true;

	// Rotation 2: T shape downward
	PieceRotations[2].Shape[1][0] = true;
	PieceRotations[2].Shape[1][1] = true;
	PieceRotations[2].Shape[1][2] = true;
	PieceRotations[2].Shape[2][1] = true;

	// Rotation 3: T shape left
	PieceRotations[3].Shape[0][1] = true;
	PieceRotations[3].Shape[1][0] = true;
	PieceRotations[3].Shape[1][1] = true;
	PieceRotations[3].Shape[2][1] = true;
}

void ATetrisPiece::CreateSPieceData()
{
	// S-Piece
	// Rotation 0: S shape
	PieceRotations[0].Shape[0][1] = true;
	PieceRotations[0].Shape[0][2] = true;
	PieceRotations[0].Shape[1][0] = true;
	PieceRotations[0].Shape[1][1] = true;

	// Rotation 1: S shape rotated
	PieceRotations[1].Shape[0][1] = true;
	PieceRotations[1].Shape[1][1] = true;
	PieceRotations[1].Shape[1][2] = true;
	PieceRotations[1].Shape[2][2] = true;

	// Rotation 2: Same as 0
	PieceRotations[2] = PieceRotations[0];

	// Rotation 3: Same as 1
	PieceRotations[3] = PieceRotations[1];
}

void ATetrisPiece::CreateZPieceData()
{
	// Z-Piece (S-Pieceの逆)
	// Rotation 0: Z shape
	PieceRotations[0].Shape[0][0] = true;
	PieceRotations[0].Shape[0][1] = true;
	PieceRotations[0].Shape[1][1] = true;
	PieceRotations[0].Shape[1][2] = true;

	// Rotation 1: Z shape rotated
	PieceRotations[1].Shape[0][2] = true;
	PieceRotations[1].Shape[1][1] = true;
	PieceRotations[1].Shape[1][2] = true;
	PieceRotations[1].Shape[2][1] = true;

	// Rotation 2: Same as 0
	PieceRotations[2] = PieceRotations[0];

	// Rotation 3: Same as 1
	PieceRotations[3] = PieceRotations[1];
}

void ATetrisPiece::CreateJPieceData()
{
	// J-Piece
	// Rotation 0: J shape
	PieceRotations[0].Shape[0][0] = true;
	PieceRotations[0].Shape[1][0] = true;
	PieceRotations[0].Shape[1][1] = true;
	PieceRotations[0].Shape[1][2] = true;

	// Rotation 1: J shape rotated
	PieceRotations[1].Shape[0][1] = true;
	PieceRotations[1].Shape[0][2] = true;
	PieceRotations[1].Shape[1][1] = true;
	PieceRotations[1].Shape[2][1] = true;

	// Rotation 2: J shape inverted
	PieceRotations[2].Shape[1][0] = true;
	PieceRotations[2].Shape[1][1] = true;
	PieceRotations[2].Shape[1][2] = true;
	PieceRotations[2].Shape[2][2] = true;

	// Rotation 3: J shape rotated left
	PieceRotations[3].Shape[0][1] = true;
	PieceRotations[3].Shape[1][1] = true;
	PieceRotations[3].Shape[2][0] = true;
	PieceRotations[3].Shape[2][1] = true;
}

void ATetrisPiece::CreateLPieceData()
{
	// L-Piece (J-Pieceのミラー)
	// Rotation 0: L shape
	PieceRotations[0].Shape[0][2] = true;
	PieceRotations[0].Shape[1][0] = true;
	PieceRotations[0].Shape[1][1] = true;
	PieceRotations[0].Shape[1][2] = true;

	// Rotation 1: L shape rotated
	PieceRotations[1].Shape[0][1] = true;
	PieceRotations[1].Shape[1][1] = true;
	PieceRotations[1].Shape[2][1] = true;
	PieceRotations[1].Shape[2][2] = true;

	// Rotation 2: L shape inverted
	PieceRotations[2].Shape[1][0] = true;
	PieceRotations[2].Shape[1][1] = true;
	PieceRotations[2].Shape[1][2] = true;
	PieceRotations[2].Shape[2][0] = true;

	// Rotation 3: L shape rotated left
	PieceRotations[3].Shape[0][0] = true;
	PieceRotations[3].Shape[0][1] = true;
	PieceRotations[3].Shape[1][1] = true;
	PieceRotations[3].Shape[2][1] = true;
}

void ATetrisPiece::DebugPrintPiece() const
{
	FString DebugString = FString::Printf(TEXT("Piece Type: %d, Rotation: %d, Position: (%d, %d)\n"), 
		(int32)CurrentPieceType, CurrentRotation, BoardPosition.X, BoardPosition.Y);

	const FTetrisPieceShape& CurrentShape = PieceRotations[CurrentRotation];
	for (int32 Y = 0; Y < 4; Y++)
	{
		FString LineString = TEXT("");
		for (int32 X = 0; X < 4; X++)
		{
			LineString += CurrentShape.Shape[Y][X] ? TEXT("■") : TEXT("□");
		}
		DebugString += LineString + TEXT("\n");
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *DebugString);
}