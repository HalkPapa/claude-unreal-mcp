# 🎮 Tetris Blueprint Code Snippets

## 📝 重要なブループリント関数の詳細実装

このドキュメントでは、テトリスゲームの各ブループリント関数の詳細な実装方法を説明します。

## 1. TetrisBoard (BP_TetrisBoard)

### InitializeBoard Function
```
Blueprint Nodes:

1. Set BoardWidth = 10
2. Set BoardHeight = 20
3. Set BlockSize = 50.0

4. Make Array (Size: BoardHeight)
   └─ For Each Loop (0 to BoardHeight-1)
      └─ Make Array (Size: BoardWidth, filled with False)
      └─ Add to GridArray

5. Spawn Actor from Class (BP_TetrisBlock)
   └─ Location: X=0, Y=0, Z=0
   └─ Store in BlockMeshArray
```

### IsPositionValid Function
```
Input Parameters:
- X (Integer)
- Y (Integer)

Blueprint Logic:
1. X >= 0 AND X < BoardWidth
   └─ AND Gate
2. Y >= 0 AND Y < BoardHeight  
   └─ AND Gate
3. GridArray[Y][X] == False
   └─ AND Gate
4. Return Boolean Result
```

### SetBlock Function
```
Input Parameters:
- X (Integer)
- Y (Integer)  
- bOccupied (Boolean)
- BlockType (E_PieceType)

Blueprint Logic:
1. Call IsPositionValid(X, Y)
2. Branch: If Valid
   └─ Set GridArray[Y][X] = bOccupied
   └─ Call UpdateBlockDisplay(X, Y, BlockType)
```

### CheckCompleteLines Function
```
Return: Array of Integers (Complete Line Indices)

Blueprint Logic:
1. Clear CompletedLines Array
2. For Each Loop (0 to BoardHeight-1) as Y
   └─ Set LineComplete = True
   └─ For Each Loop (0 to BoardWidth-1) as X
      └─ If GridArray[Y][X] == False
         └─ Set LineComplete = False
         └─ Break
   └─ If LineComplete == True
      └─ Add Y to CompletedLines Array
3. Return CompletedLines
```

### ClearLine Function
```
Input Parameters:
- LineY (Integer)

Blueprint Logic:
1. For Each Loop (LineY down to 1) as Y
   └─ For Each Loop (0 to BoardWidth-1) as X
      └─ Set GridArray[Y][X] = GridArray[Y-1][X]
2. For Each Loop (0 to BoardWidth-1) as X
   └─ Set GridArray[0][X] = False
3. Call UpdateAllBlockDisplays()
```

## 2. TetrisPiece (BP_TetrisPiece)

### InitializePiece Function
```
Input Parameters:
- PieceType (E_PieceType)

Blueprint Logic:
1. Set CurrentPieceType = PieceType
2. Set CurrentRotation = 0
3. Set BoardPosition = (BoardWidth/2, BoardHeight-2)

4. Switch on Enum (PieceType):
   Case I_Piece:
   └─ Set ShapeData = I_PIECE_SHAPES[0]
   
   Case O_Piece:
   └─ Set ShapeData = O_PIECE_SHAPES[0]
   
   [Continue for all piece types]

5. Call CreateBlockComponents()
6. Call UpdatePosition()
```

### GetCurrentShape Function
```
Return: Array of Vector2D (Block Positions)

Blueprint Logic:
1. Get ShapeData = PIECE_SHAPES[CurrentPieceType][CurrentRotation]
2. Clear BlockPositions Array
3. For Each Loop (0 to 3) as Y
   └─ For Each Loop (0 to 3) as X
      └─ If ShapeData[Y][X] == True
         └─ Make Vector2D(X, Y)
         └─ Add to BlockPositions
4. Return BlockPositions
```

### CanMoveTo Function
```
Input Parameters:
- NewX (Integer)
- NewY (Integer)

Blueprint Logic:
1. Call GetCurrentShape() -> BlockPositions
2. For Each (BlockPosition in BlockPositions)
   └─ Calculate WorldX = NewX + BlockPosition.X
   └─ Calculate WorldY = NewY + BlockPosition.Y
   └─ Call TetrisBoard->IsPositionValid(WorldX, WorldY)
   └─ If False, Return False
3. Return True
```

### MovePiece Function
```
Input Parameters:
- Direction (Vector2D)

Blueprint Logic:
1. Calculate NewPosition = BoardPosition + Direction
2. Call CanMoveTo(NewPosition.X, NewPosition.Y)
3. Branch: If True
   └─ Set BoardPosition = NewPosition
   └─ Call UpdatePosition()
   └─ Return True
4. Else Return False
```

### RotatePiece Function
```
Blueprint Logic:
1. Store OldRotation = CurrentRotation
2. Set CurrentRotation = (CurrentRotation + 1) % 4
3. Call CanMoveTo(BoardPosition.X, BoardPosition.Y)
4. Branch: If True
   └─ Call UpdatePosition()
   └─ Return True
5. Else
   └─ Set CurrentRotation = OldRotation
   └─ Return False
```

## 3. TetrisGameMode (BP_TetrisGameMode)

### BeginPlay Function
```
Blueprint Logic:
1. Call Super::BeginPlay
2. Spawn Actor from Class (BP_TetrisBoard)
   └─ Location: (0, 0, 0)
   └─ Store in TetrisBoardRef
3. Call TetrisBoardRef->InitializeBoard()
4. Call SpawnNewPiece()
5. Call SetFallSpeed(1.0)
6. Call StartAutoFallTimer()
```

### SpawnNewPiece Function
```
Blueprint Logic:
1. If CurrentPieceRef != None
   └─ Call PlacePieceOnBoard()
   └─ Call CheckCompleteLines()

2. Set PieceType = (NextPieceType != None) ? NextPieceType : GetRandomPieceType()
3. Set NextPieceType = GetRandomPieceType()

4. Spawn Actor from Class (BP_TetrisPiece)
   └─ Location: GetSpawnLocation()
   └─ Store in CurrentPieceRef

5. Call CurrentPieceRef->InitializePiece(PieceType)

6. Call CheckGameOver()
```

### PlacePieceOnBoard Function
```
Blueprint Logic:
1. Get BlockPositions = CurrentPieceRef->GetCurrentShape()
2. Get PiecePosition = CurrentPieceRef->GetBoardPosition()

3. For Each (BlockPos in BlockPositions)
   └─ Calculate WorldX = PiecePosition.X + BlockPos.X
   └─ Calculate WorldY = PiecePosition.Y + BlockPos.Y
   └─ Call TetrisBoardRef->SetBlock(WorldX, WorldY, True, CurrentPieceType)

4. Destroy CurrentPieceRef
5. Set CurrentPieceRef = None
```

### ProcessCompletedLines Function
```
Blueprint Logic:
1. Call TetrisBoardRef->CheckCompleteLines() -> CompletedLines
2. Get LineCount = CompletedLines.Length

3. If LineCount > 0:
   └─ For Each (LineIndex in CompletedLines)
      └─ Call TetrisBoardRef->ClearLine(LineIndex)
      └─ Call PlayLineClearEffect(LineIndex)
   
   └─ Call AddScore(LineCount)
   └─ Set LinesCleared += LineCount
   └─ Call CheckLevelUp()
```

### AddScore Function
```
Input Parameters:
- Lines (Integer)

Blueprint Logic:
1. Switch on Int (Lines):
   Case 1: Set ScoreToAdd = 100 * CurrentLevel
   Case 2: Set ScoreToAdd = 300 * CurrentLevel  
   Case 3: Set ScoreToAdd = 500 * CurrentLevel
   Case 4: Set ScoreToAdd = 800 * CurrentLevel
   Default: Set ScoreToAdd = 0

2. Set CurrentScore += ScoreToAdd
3. Call UpdateScoreUI()
```

## 4. TetrisPlayerController (BP_TetrisPlayerController)

### SetupInputBindings Function
```
Blueprint Logic:
1. Get Enhanced Input Subsystem
2. Add Mapping Context (IMC_Tetris)
3. Set Priority = 0

Enhanced Input Action Events:
- IA_MoveLeft -> OnMoveLeft
- IA_MoveRight -> OnMoveRight
- IA_MoveDown -> OnMoveDown
- IA_Rotate -> OnRotate
- IA_HardDrop -> OnHardDrop
```

### OnMoveLeft Function
```
Blueprint Logic:
1. Get GameMode as TetrisGameMode
2. Get CurrentPieceRef from GameMode
3. If CurrentPieceRef != None AND GameState == Playing:
   └─ Call CurrentPieceRef->MovePiece(Vector2D(-1, 0))
```

### OnRotate Function
```
Blueprint Logic:
1. Get GameMode as TetrisGameMode
2. Get CurrentPieceRef from GameMode
3. If CurrentPieceRef != None AND GameState == Playing:
   └─ Call CurrentPieceRef->RotatePiece()
```

### HandleAutoFall Function
```
Blueprint Logic:
1. Get GameMode as TetrisGameMode
2. Get CurrentPieceRef from GameMode
3. If CurrentPieceRef != None:
   └─ Call CurrentPieceRef->MovePiece(Vector2D(0, 1))
   └─ If Move Failed:
      └─ Call GameMode->SpawnNewPiece()
```

## 5. UI Widget (WBP_TetrisHUD)

### UpdateScore Function
```
Input Parameters:
- NewScore (Integer)

Blueprint Logic:
1. Convert Integer to Text (NewScore)
2. Set ScoreTextBlock->Text = "Score: " + ScoreText
```

### UpdateNextPiece Function
```
Input Parameters:
- NextPieceType (E_PieceType)

Blueprint Logic:
1. Switch on Enum (NextPieceType):
   Case I_Piece: Set NextPieceImage->Brush = I_PIECE_TEXTURE
   Case O_Piece: Set NextPieceImage->Brush = O_PIECE_TEXTURE
   [Continue for all types]
```

## 6. Data Structures

### Piece Shape Data (Data Table)
```
Create Data Table: DT_PieceShapes
Row Structure: S_PieceShapeData

I_Piece_0: [[0,0,0,0],[1,1,1,1],[0,0,0,0],[0,0,0,0]]
I_Piece_1: [[0,0,1,0],[0,0,1,0],[0,0,1,0],[0,0,1,0]]
I_Piece_2: [[0,0,0,0],[0,0,0,0],[1,1,1,1],[0,0,0,0]]
I_Piece_3: [[0,1,0,0],[0,1,0,0],[0,1,0,0],[0,1,0,0]]

O_Piece_0: [[0,0,0,0],[0,1,1,0],[0,1,1,0],[0,0,0,0]]
O_Piece_1: [[0,0,0,0],[0,1,1,0],[0,1,1,0],[0,0,0,0]]
O_Piece_2: [[0,0,0,0],[0,1,1,0],[0,1,1,0],[0,0,0,0]]
O_Piece_3: [[0,0,0,0],[0,1,1,0],[0,1,1,0],[0,0,0,0]]

[Continue for all piece types and rotations]
```

## 🎯 実装のポイント

### パフォーマンス最適化
1. **Object Pooling**: ブロックアクターを事前生成して再利用
2. **Batch Updates**: UIの更新は必要時のみ
3. **Efficient Collision**: グリッドベースの高速衝突判定

### デバッグ機能
1. **Debug Display**: グリッド状態の視覚化
2. **Console Commands**: 手動ピース生成、スコア調整
3. **Logging**: 重要なゲーム状態変化をログ出力

この実装ガイドに従って、完全に機能するテトリスゲームを作成できます！