# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is an Unreal Engine 5.6 project named "ClaudeTest" - a blank project template set up for game development. The project is configured for mobile-targeted hardware class with maximum graphics performance settings, supporting multiple rendering paths including Lumen, Nanite, and ray tracing features.

## Project Structure

```
ClaudeTest/
├── ClaudeTest.uproject          # Main project file (UE 5.6)
├── Config/                      # Engine and project configuration
│   ├── DefaultEngine.ini        # Engine settings and rendering pipeline
│   ├── DefaultGame.ini          # Game-specific settings
│   ├── DefaultEditor.ini        # Editor configuration
│   └── DefaultInput.ini         # Input mapping configuration  
├── Content/                     # Game assets and blueprints
│   ├── Collections/             # Asset collections
│   └── Developers/              # Developer-specific content
├── Intermediate/                # Build artifacts and cache (auto-generated)
├── Saved/                       # Editor saves and user settings
└── DerivedDataCache/            # Asset derivative cache
```

## Development Commands

Since this is an Unreal Engine project, development is primarily done through the Unreal Editor:

### Opening the Project
```bash
# Primary method: Double-click the .uproject file
open ClaudeTest/ClaudeTest.uproject

# Via Epic Games Launcher (if installed)
# Launch Epic Games Launcher -> Library -> ClaudeTest

# Command line (if UE is in PATH - uncommon on Mac)
# UnrealEditor ClaudeTest/ClaudeTest.uproject
```

### Building the Project
```bash
# Unreal projects typically build through the editor, but for C++ projects:
# UnrealBuildTool ClaudeTest Development Mac -Project="ClaudeTest.uproject"
```

### Cooking/Packaging
```bash
# Cook content for target platform (done through editor or command line)
# UnrealEditor-Cmd ClaudeTest.uproject -run=cook -targetplatform=Mac
```

## Key Configuration Settings

### Graphics & Rendering (DefaultEngine.ini)
- **Target Hardware**: Mobile with Maximum performance
- **Rendering Features**: Lumen GI, Hardware Ray Tracing, Nanite enabled
- **Graphics API**: 
  - Windows: DirectX 12 (SM6)
  - Mac: Metal (SM5/SM6/ES3.1)
  - Linux: Vulkan (SM6)

### Project Settings
- **Engine Version**: 5.6
- **Project ID**: 54943DFBBC437580A8281F8F78EA4EB0
- **Active Plugins**: ModelingToolsEditorMode (Editor-only)

## Architecture Notes

- This is a **Blueprint-only project** (no C++ source code currently)
- Configured as a blank template with mobile optimization
- Uses Unreal's standard project structure and conventions
- Editor startup map set to OpenWorld template
- Virtual texturing and mesh distance fields enabled for performance
- Enhanced Input System enabled (EnhancedPlayerInput/EnhancedInputComponent)
- Supports multiple VR platforms (Oculus, Valve Index, Mixed Reality)

## Working with Assets

- **Content Browser**: Primary asset management interface
- **Asset Organization**: Use Collections for grouping related assets
- **Developer Content**: Personal content goes in `Content/Developers/[username]/`
- **Shared Content**: Team assets go in appropriate `Content/` subdirectories

## Important Files to Never Modify Directly

- `Intermediate/` - Auto-generated build artifacts
- `Saved/` - Editor preferences and temporary files  
- `DerivedDataCache/` - Asset processing cache
- `.uasset` binary files should be edited through the UE Editor only

## Platform-Specific Notes

### Mac Development
- Uses Metal rendering API (SM5/SM6/ES3.1 support)
- Universal binary architecture support
- Fast intrinsics disabled for compatibility

### Mobile Optimization
- Hardware-targeted for mobile devices
- Mobile HDR enabled
- Static and CSM shadow receivers supported
- Forward rendering optimizations available
- Touch interface configured with virtual joysticks
- Four-finger tap console access enabled for debugging

## Input Configuration

- **Enhanced Input System**: Modern input handling with EnhancedPlayerInput
- **Platform Support**: Mouse, keyboard, gamepad, and VR controller support
- **VR Ready**: Configured for Oculus Touch, Valve Index, and Mixed Reality
- **Mobile Touch**: Default virtual joysticks for mobile platforms
- **Console Access**: Tilde key (~) or four-finger tap on touch devices

---

# 日本語ガイド

## プロジェクト概要

このプロジェクトは「ClaudeTest」という名前のUnreal Engine 5.6プロジェクトです。モバイル向けゲーム開発用のブランクテンプレートとして設定されており、最新のレンダリング機能（Lumen、Nanite、レイトレーシング）を活用できます。

## 開発環境のセットアップ

### プロジェクトの開き方
```bash
# 主な方法：.uprojectファイルをダブルクリック
open ClaudeTest/ClaudeTest.uproject

# Epic Games Launcherから起動する場合
# Epic Games Launcher → ライブラリ → ClaudeTest を選択
```

### よく使用するディレクトリ
- `Content/` - ゲームアセットやブループリント
- `Config/` - エンジンとプロジェクトの設定ファイル
- `Saved/` - エディタの設定と一時ファイル

## 重要な設定

### レンダリング設定
- **対象ハードウェア**: モバイル（最大パフォーマンス）
- **グラフィックスAPI**: Metal (Mac)、DirectX 12 (Windows)、Vulkan (Linux)
- **先進機能**: Lumen GI、ハードウェアレイトレーシング、Nanite有効

### 入力システム
- **Enhanced Input System**を使用
- VRコントローラー対応（Oculus、Valve Index、Mixed Reality）
- モバイル用タッチインターフェース（バーチャルジョイスティック）
- コンソールアクセス：チルダキー（~）または4本指タップ

## ファイル操作の注意点

### 絶対に直接編集してはいけないファイル/フォルダ
- `Intermediate/` - 自動生成されるビルド成果物
- `Saved/` - エディタの設定と一時ファイル
- `DerivedDataCache/` - アセット処理キャッシュ
- `.uasset`ファイル - Unrealエディタでのみ編集可能

### 安全に編集できる設定ファイル
- `Config/DefaultEngine.ini` - エンジン設定
- `Config/DefaultGame.ini` - ゲーム固有設定
- `Config/DefaultInput.ini` - 入力マッピング

## 開発のベストプラクティス

### アセット管理
- 個人用コンテンツは `Content/Developers/[ユーザー名]/` に配置
- 共有アセットは適切な `Content/` サブディレクトリに配置
- コレクション機能を使用してアセットをグループ化

### VR開発
- プロジェクトは既にVR対応設定済み
- Oculus Touch、Valve Index、Mixed Realityコントローラーをサポート
- VR固有の入力設定は `DefaultInput.ini` で確認可能

### モバイル開発
- モバイル最適化が既に設定済み
- HDR対応、タッチインターフェース有効
- フォワードレンダリング最適化利用可能

## トラブルシューティング

### プロジェクトが開かない場合
1. Epic Games Launcherが正しくインストールされているか確認
2. Unreal Engine 5.6がインストールされているか確認
3. `.uproject`ファイルのUnreal Engine関連付けを確認

### パフォーマンスの問題
- `DerivedDataCache/`を削除して再生成を試行
- `Intermediate/`フォルダを削除してプロジェクトを再オープン