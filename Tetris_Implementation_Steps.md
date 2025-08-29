# 🎮 Tetris Blueprint Implementation Steps

## 📋 実装手順書

この手順書では、Unreal Engine 5.6のブループリントエディタで段階的にテトリスゲームを作成します。

## Phase 1: プロジェクト準備とGameMode設定

### Step 1.1: 新しいレベル作成
```
1. Content Browser で右クリック
2. Blueprint Class を選択
3. Game Mode Base を親クラスに選択
4. 名前を「BP_TetrisGameMode」に設定
```

### Step 1.2: カメラとライト設定
```
1. レベルに Camera Actor を配置
2. Location: X=0, Y=-800, Z=500
3. Rotation: X=0, Y=0, Z=0 (ボードを上から見下ろす)
4. Field of View: 60度

5. Directional Light を配置
6. Intensity: 3.0
7. Light Color: 白 (1,1,1)
```

## Phase 2: TetrisBoard Blueprint作成

### Step 2.1: ボードActor作成
```
1. Actor クラスから「BP_TetrisBoard」を作成
2. Scene Component をルートに設定
3. Static Mesh Component を追加（ボード背景用）

Variables:
- BoardWidth: Integer = 10
- BoardHeight: Integer = 20  
- BlockSize: Float = 50.0
- GridArray: Boolean の2次元配列
- BlockMeshArray: Static Mesh Component の配列
```

### Step 2.2: グリッドシステム実装
```
Functions:

InitializeBoard:
1. GridArray のサイズを [BoardHeight][BoardWidth] に設定
2. すべてを false で初期化
3. グリッド線の表示用メッシュを生成

IsPositionValid(X, Y):
1. X が 0 <= X < BoardWidth の範囲内かチェック  
2. Y が 0 <= Y < BoardHeight の範囲内かチェック
3. GridArray[Y][X] が false（空）かチェック
4. すべて true なら有効な位置を返す

SetBlock(X, Y, bOccupied):
1. IsPositionValid でチェック
2. GridArray[Y][X] = bOccupied
3. 対応する表示ブロックを更新
```

### Step 2.3: 表示システム
```
CreateVisualGrid:
1. ボード背景のStatic Meshを配置
2. グリッド線を表示（Line Componentまたは材質で）
3. ブロック表示用のInstanced Static Mesh Componentを準備

UpdateBlockDisplay:
1. すべてのGridArrayをスキャン
2. true の位置にブロックメッシュを表示
3. false の位置のブロックを非表示
```

## Phase 3: TetrisBlock Blueprint作成

### Step 3.1: 基本ブロック
```
1. Actor クラスから「BP_TetrisBlock」を作成
2. Static Mesh Component を追加
3. 基本的なCubeメッシュを設定
4. サイズ: 50x50x50 units

Variables:
- BlockColor: Linear Color
- BlockType: Enum (Empty, I, O, T, S, Z, J, L)
- IsActive: Boolean

Material:
- 基本マテリアル「M_TetrisBlock」を作成
- Base Color をパラメータ化
- Emissive も追加（光る効果用）
```

## Phase 4: TetrisPiece Blueprint作成

### Step 4.1: ピースデータ構造
```
Structure「S_PieceShape」を作成:
- ShapeArray: Boolean の2次元配列 [4][4]
- PieceColor: Linear Color
- PieceType: Enum

Enum「E_PieceType」を作成:
- None
- I_Piece  
- O_Piece
- T_Piece
- S_Piece
- Z_Piece
- J_Piece
- L_Piece
```

### Step 4.2: TetrisPiece Actor作成
```
Blueprint「BP_TetrisPiece」を作成:

Variables:
- CurrentPieceType: E_PieceType
- CurrentRotation: Integer (0-3)
- BoardPosition: Vector2D (Int)
- PieceShapes: S_PieceShape の配列
- BlockComponents: Scene Component の配列

Functions:
InitializePiece(PieceType):
1. CurrentPieceType を設定
2. 対応するShapeDataを取得
3. ブロックComponentsを生成
4. 初期位置を設定 (BoardWidth/2, BoardHeight-1)
```

### Step 4.3: ピース形状データ定義
```
BeginPlay または Data Table で各ピース形状を定義:

I_Piece:
[0,0,0,0]
[1,1,1,1] 
[0,0,0,0]
[0,0,0,0]

O_Piece:
[0,0,0,0]
[0,1,1,0]
[0,1,1,0]
[0,0,0,0]

T_Piece:
[0,0,0,0]
[0,1,0,0]
[1,1,1,0]
[0,0,0,0]

(他のピースも同様に定義)
```

### Step 4.4: 回転システム
```
Function「RotatePiece」:
1. 現在の形状配列を取得
2. 90度回転の数学的変換を適用:
   新しい[x][y] = 元の[3-y][x]
3. 回転後の位置が有効かチェック
4. 有効なら CurrentRotation をインクリメント
5. 表示を更新

Function「CanRotate」:
1. 回転後の各ブロック位置を計算
2. TetrisBoard の IsPositionValid でチェック
3. すべて有効なら true を返す
```

## Phase 5: 入力システム (Enhanced Input)

### Step 5.1: Input Actions作成
```
1. Input Action「IA_MoveLeft」を作成
2. Input Action「IA_MoveRight」を作成  
3. Input Action「IA_MoveDown」を作成
4. Input Action「IA_Rotate」を作成
5. Input Action「IA_HardDrop」を作成
```

### Step 5.2: Input Mapping Context
```
「IMC_Tetris」を作成して以下をマッピング:
- IA_MoveLeft: A キー, Left Arrow
- IA_MoveRight: D キー, Right Arrow  
- IA_MoveDown: S キー, Down Arrow
- IA_Rotate: W キー, Up Arrow, Space
- IA_HardDrop: Enter キー
```

### Step 5.3: PlayerController設定
```
「BP_TetrisPlayerController」を作成:

BeginPlay:
1. Input Mapping Context を追加
2. AutoFall Timer を開始

Input Event Functions:
- OnMoveLeft: 現在のピースを左に移動
- OnMoveRight: 現在のピースを右に移動
- OnMoveDown: 現在のピースを下に移動
- OnRotate: 現在のピースを回転
- OnHardDrop: 現在のピースを一番下まで移動
```

## Phase 6: ゲームロジック実装

### Step 6.1: ピース移動システム
```
Function「MovePiece」(Direction):
1. 新しい位置を計算
2. CanMoveToPosition でチェック
3. 移動可能なら位置を更新
4. 移動不可能で下方向なら、ピースを固定

Function「CanMoveToPosition」(NewX, NewY):
1. 現在のピース形状の各ブロックをチェック
2. 新しい位置が有効範囲内かチェック  
3. ボードの対応位置が空いているかチェック
```

### Step 6.2: 自動落下システム
```
Variables:
- AutoFallTimer: Float
- FallSpeed: Float = 1.0 (秒)
- CurrentFallTime: Float

Tick Event:
1. CurrentFallTime に DeltaTime を加算
2. FallSpeed を超えたら自動的に下に移動
3. CurrentFallTime をリセット
```

### Step 6.3: ライン完成判定
```
Function「CheckCompleteLines」:
1. ボードの各行をチェック (下から上へ)
2. 行がすべて埋まっているかチェック
3. 完成した行のインデックスを配列で返す

Function「ClearLines」(LineIndices):
1. 完成した行を削除
2. 上の行を下にシフト
3. 新しい空の行を上部に追加
4. スコアを更新
```

## Phase 7: UI システム

### Step 7.1: HUD Widget作成
```
「WBP_TetrisHUD」Widget Blueprintを作成:

UI Elements:
- Score Text Block
- Lines Text Block  
- Level Text Block
- Next Piece Image
- Game Over Panel
- Pause Panel

Functions:
- UpdateScore(NewScore)
- UpdateLines(NewLines)
- UpdateLevel(NewLevel)
- ShowGameOver()
- ShowPause()
```

### Step 7.2: HUD Integration
```
「BP_TetrisHUD」Blueprint作成:
1. Draw HUD eventで WBP_TetrisHUD を表示
2. GameMode と連携してゲーム状態を更新

BeginPlay:
1. Widget を作成
2. Viewport に追加
3. 初期値を設定
```

## Phase 8: ゲーム管理システム

### Step 8.1: GameMode完成
```
「BP_TetrisGameMode」Variables:
- CurrentScore: Integer
- LinesCleared: Integer
- CurrentLevel: Integer
- GameState: Enum (Playing, Paused, GameOver)
- CurrentPiece: BP_TetrisPiece Reference
- NextPieceType: E_PieceType
- TetrisBoard: BP_TetrisBoard Reference

Functions:
BeginPlay:
1. ボードを初期化
2. 最初のピースを生成
3. 次のピースタイプを決定

SpawnNewPiece:
1. NextPieceType でピースを生成
2. 新しい NextPieceType をランダム決定
3. ゲームオーバー判定

AddScore(Lines):
1. ライン数に応じてスコア加算
2. レベルアップ判定
3. 落下速度調整
```

### Step 8.2: ゲームオーバー処理
```
Function「CheckGameOver」:
1. 新しいピースが配置可能かチェック
2. 不可能ならゲームオーバー状態に変更
3. ゲームオーバーUIを表示

Function「RestartGame」:
1. ボードをクリア
2. スコア・レベルをリセット
3. 新しいゲームを開始
```

## Phase 9: エフェクトとサウンド

### Step 9.1: パーティクルエフェクト
```
ライン消去エフェクト:
1. Niagara System「NS_LineClear」を作成
2. 消去される行に配置
3. 光の粒子が散るエフェクト

ピース着地エフェクト:
1. 小さな衝撃波エフェクト
2. ピースが固定される瞬間に再生
```

### Step 9.2: サウンドシステム
```
Sound Cues:
- SC_PieceMove: ピース移動音
- SC_PieceRotate: 回転音
- SC_LineClear: ライン消去音
- SC_GameOver: ゲームオーバー音
- SC_LevelUp: レベルアップ音

Audio Components:
各サウンドを適切なタイミングで再生
```

## 🚀 実装チェックリスト

### Core Systems
- [ ] ボードグリッド表示
- [ ] 7種類のテトリミノ生成
- [ ] ピース移動（左右下）
- [ ] ピース回転
- [ ] 衝突判定
- [ ] 自動落下
- [ ] ライン完成判定
- [ ] ライン消去

### Input System  
- [ ] キーボード操作
- [ ] Enhanced Input設定
- [ ] 操作レスポンス

### UI System
- [ ] スコア表示
- [ ] ライン数表示
- [ ] レベル表示  
- [ ] 次のピース表示
- [ ] ゲームオーバー画面

### Game Features
- [ ] スコアリング
- [ ] レベルアップ
- [ ] ゲーム速度変化
- [ ] ポーズ機能
- [ ] リスタート機能

### Polish
- [ ] サウンドエフェクト
- [ ] パーティクルエフェクト
- [ ] アニメーション
- [ ] UI アニメーション

この実装手順書に従って、完全なテトリスゲームを作成しましょう！