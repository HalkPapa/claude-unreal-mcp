# Claude Unreal MCP Project

![Unreal Engine](https://img.shields.io/badge/Unreal_Engine-5.6-blue?logo=unrealengine)
![MCP Ready](https://img.shields.io/badge/MCP-Ready-green)
![Platform](https://img.shields.io/badge/Platform-Mobile%20%7C%20Desktop%20%7C%20VR-orange)

Unreal Engine 5.6プロジェクトのMCP（Model Context Protocol）統合用テンプレート

## 🎯 プロジェクト概要

このプロジェクトは、AI開発支援ツール（Claude Codeなど）を使用してUnreal Engineプロジェクトを効率的に開発するためのテンプレートです。MCPプロトコルに対応し、AIアシスタントが最適に動作するよう設計されています。

### 主な特徴

- ✨ **Unreal Engine 5.6** - 最新のレンダリング機能（Lumen、Nanite、Ray Tracing）
- 📱 **モバイル最適化** - iOS/Android向けパフォーマンス設定
- 🥽 **VR対応** - Oculus、Valve Index、Mixed Reality サポート
- 🤖 **AI開発支援** - Claude Code完全対応
- 📚 **多言語ドキュメント** - 英語・日本語対応

## 🚀 クイックスタート

### 必要な環境

- Unreal Engine 5.6
- Epic Games Launcher
- Git

### セットアップ

```bash
# リポジトリをクローン
git clone https://github.com/HalkPapa/claude-unreal-mcp.git
cd claude-unreal-mcp

# プロジェクトを開く
open ClaudeTest/ClaudeTest.uproject
```

## 📁 プロジェクト構造

```
claude-unreal-mcp/
├── 📝 CLAUDE.md           # AI開発支援ドキュメント（英語・日本語）
├── 🔧 MCP_SETUP.md        # MCP統合設定ガイド
├── 🎮 ClaudeTest/         # Unrealプロジェクト本体
│   ├── ClaudeTest.uproject
│   ├── Config/            # エンジン設定
│   └── Content/           # ゲームアセット
└── 🚫 .gitignore          # Unreal Engine用除外設定
```

## 🤖 AI開発支援機能

### Claude Codeとの連携

このプロジェクトは以下のAI支援機能に対応しています：

- **プロジェクト分析** - 設定ファイルの理解と最適化提案
- **Blueprint開発** - 視覚的スクリプティング支援
- **プラットフォーム設定** - マルチプラットフォーム最適化
- **ドキュメント生成** - 技術仕様書の自動生成

### 使用例

```bash
# Claude Codeでの開発支援例
claude "モバイルVR向けにプロジェクトを最適化して"
claude "Enhanced Input Systemを使用したプレイヤー移動を実装して"
claude "レンダリング設定をモバイル向けに調整して"
```

## 🎨 レンダリング機能

- **Lumen Global Illumination** - リアルタイムグローバルイルミネーション
- **Hardware Ray Tracing** - ハードウェアレイトレーシング
- **Nanite Virtualized Geometry** - 仮想化ジオメトリシステム
- **Virtual Shadow Maps** - 仮想シャドウマップ

## 📱 プラットフォームサポート

### モバイル
- iOS / Android対応
- タッチインターフェース
- モバイルHDR
- パフォーマンス最適化

### デスクトップ  
- Windows (DirectX 12)
- Mac (Metal)
- Linux (Vulkan)

### VR
- Oculus Touch
- Valve Index
- Mixed Reality

## 📖 ドキュメント

- **[CLAUDE.md](./CLAUDE.md)** - 開発ガイド（英語・日本語）
- **[MCP_SETUP.md](./MCP_SETUP.md)** - MCP統合設定
- プロジェクト設定詳細は各Configファイル参照

## 🛠️ 開発ワークフロー

1. **プロジェクト解析** - AIが現在の設定を理解
2. **コード生成** - Blueprint論理設計の支援
3. **設定管理** - 異なる展開シナリオ向けの.iniファイル調整
4. **ドキュメント維持** - プロジェクト文書の自動更新
5. **問題解決** - プロジェクト構造とログを使用した診断

## 🔒 セキュリティとベストプラクティス

- 機密データは`.gitignore`で除外
- コンパイル済みバイナリはリポジトリに含まない
- 設定ファイルを安全にバージョン管理
- 全設定の明確な文書化

## 🤝 貢献

プロジェクトの改善提案やバグレポートは [Issues](https://github.com/HalkPapa/claude-unreal-mcp/issues) でお願いします。

## 📄 ライセンス

このプロジェクトはMIT Licenseの下で公開されています。

## 🙏 謝辞

- Unreal Engineチーム
- Claude Code開発チーム  
- MCPコミュニティ

---

**🚀 AIと一緒に、次世代のゲーム開発を始めよう！**