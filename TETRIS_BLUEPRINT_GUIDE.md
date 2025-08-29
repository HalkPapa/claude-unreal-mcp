# 🎮 Tetris Blueprint Implementation Guide

## 📝 プロジェクト概要

UnrealEngine 5.6でブループリントを使用してテトリスゲームを作成するための完全なガイド。

## 🏗️ ゲーム構造

### Core Components

1. **TetrisGameMode** - ゲーム全体の管理
2. **TetrisPlayerController** - プレイヤー入力処理
3. **TetrisBoard** - ゲームボード管理
4. **TetrisPiece** - テトリミノ（テトリスピース）
5. **TetrisBlock** - 個別ブロック
6. **TetrisHUD** - UI表示

## 📐 ゲームボード設計

```
標準テトリスボード: 10×20 グリッド
- 幅: 10 ブロック
- 高さ: 20 ブロック  
- 表示領域: 下から20行
- 隠し領域: 上部4行（ピース生成用）
```

## 🧩 テトリミノ（7種類のピース）

### I-Piece (直線)
```
■■■■
```

### O-Piece (正方形)
```
■■
■■
```

### T-Piece (T字)
```
 ■
■■■
```

### S-Piece (S字)
```
 ■■
■■
```

### Z-Piece (Z字)
```
■■
 ■■
```

### J-Piece (J字)
```
■
■■■
```

### L-Piece (L字)
```
  ■
■■■
```

## 🎯 Blueprint実装計画

### Phase 1: 基本システム
- [ ] ゲームボード作成
- [ ] 基本ブロック表示
- [ ] グリッドシステム

### Phase 2: テトリミノシステム
- [ ] 7種類のピース作成
- [ ] ピース回転システム
- [ ] 落下システム

### Phase 3: ゲームロジック
- [ ] 衝突検出
- [ ] ライン完成判定
- [ ] ライン消去アニメーション

### Phase 4: 入力システム
- [ ] キーボード操作
- [ ] モバイルタッチ操作
- [ ] ゲームパッド対応

### Phase 5: UI/UXシステム
- [ ] スコア表示
- [ ] 次のピース表示
- [ ] レベル表示
- [ ] ゲームオーバー画面

### Phase 6: 効果・演出
- [ ] サウンドエフェクト
- [ ] パーティクルエフェクト
- [ ] アニメーション

## 🗂️ Blueprint構成

### 1. TetrisGameMode
```
Variables:
- CurrentScore: Integer
- CurrentLevel: Integer
- LinesCleared: Integer
- GameState: Enum (Playing, Paused, GameOver)

Functions:
- BeginPlay()
- AddScore(Lines)
- CheckLevelUp()
- GameOver()
```

### 2. TetrisBoard
```
Variables:
- BoardWidth: Integer = 10
- BoardHeight: Integer = 20
- GridArray: Array of Arrays (Boolean)
- BlockActors: Array of Actors

Functions:
- InitializeBoard()
- IsPositionValid(X, Y)
- PlaceBlock(X, Y, BlockType)
- CheckCompleteLines()
- ClearLine(LineIndex)
- GetBoardState()
```

### 3. TetrisPiece
```
Variables:
- PieceType: Enum (I, O, T, S, Z, J, L)
- CurrentRotation: Integer
- Position: Vector2D
- BlockPositions: Array of Vector2D

Functions:
- Initialize(Type)
- Move(Direction)
- Rotate()
- CanMove(Direction)
- CanRotate()
- GetBlockPositions()
```

### 4. TetrisPlayerController
```
Variables:
- InputEnabled: Boolean
- AutoDropTimer: Float

Functions:
- SetupInputBindings()
- MoveLeft()
- MoveRight()
- MoveDown()
- RotatePiece()
- HardDrop()
- HandleAutoFall()
```

## 🎨 Visual Design

### ブロック色設定
- I-Piece: シアン (#00FFFF)
- O-Piece: イエロー (#FFFF00)
- T-Piece: パープル (#800080)
- S-Piece: グリーン (#00FF00)
- Z-Piece: レッド (#FF0000)
- J-Piece: ブルー (#0000FF)
- L-Piece: オレンジ (#FFA500)

### UI色設定
- 背景: ダークグレー (#2F2F2F)
- グリッド線: ライトグレー (#CCCCCC)
- テキスト: ホワイト (#FFFFFF)

## 🎵 サウンド設計

### 効果音
- ピース移動: 軽いクリック音
- ピース回転: 回転音
- ライン消去: 消去音（ライン数により変化）
- レベルアップ: ファンファーレ
- ゲームオーバー: ゲームオーバー音

### BGM
- メインゲーム: テトリス風ループBGM
- メニュー: 軽快なメニューBGM

## 📱 入力マッピング

### キーボード
- A/←: 左移動
- D/→: 右移動  
- S/↓: 下移動
- W/↑: 回転
- Space: ハードドロップ
- P: ポーズ
- R: リスタート

### モバイルタッチ
- 左スワイプ: 左移動
- 右スワイプ: 右移動
- 下スワイプ: 下移動
- タップ: 回転
- 長押し: ハードドロップ

### ゲームパッド
- 左スティック/十字キー: 移動
- A/X: 回転
- B/Y: ハードドロップ
- Start: ポーズ

## 🎯 スコアリングシステム

### 基本スコア
- 1ライン: 100点 × レベル
- 2ライン: 300点 × レベル
- 3ライン: 500点 × レベル
- 4ライン: 800点 × レベル

### ボーナス
- ソフトドロップ: 1点/ブロック
- ハードドロップ: 2点/ブロック

### レベル進行
- 10ライン消去毎にレベルアップ
- レベル上昇で落下速度増加

## 🚀 実装順序

1. **基本ボード作成** (Day 1)
2. **ピース生成・表示** (Day 2)
3. **基本移動・回転** (Day 3)
4. **衝突判定** (Day 4)
5. **ライン消去** (Day 5)
6. **スコア・UI** (Day 6)
7. **入力システム** (Day 7)
8. **効果・演出** (Day 8)

## 🐛 デバッグ・テスト項目

### 基本動作
- [ ] ピースが正常に生成される
- [ ] 移動・回転が正確に動作
- [ ] 衝突判定が正確
- [ ] ライン消去が正常に動作

### エッジケース
- [ ] ボード端での回転
- [ ] 複数ライン同時消去
- [ ] 最上段でのゲームオーバー
- [ ] 高速入力への対応

### パフォーマンス
- [ ] 60fps安定動作
- [ ] メモリリーク無し
- [ ] 長時間プレイ対応

この設計書に従って、段階的にテトリスゲームを実装していきます！