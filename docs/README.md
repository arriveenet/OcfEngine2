# OcfEngine2 ドキュメント

## 概要

OcfEngine2は、C++17で書かれたレンダリングエンジンです。このドキュメントでは、エンジンの主要なコンポーネントとアーキテクチャについて説明します。

## ドキュメント一覧

- [ハンドルシステム](handle-system.md) - バックエンドのハンドルの仕組みについて
- [メモリアロケーション](memory-allocation.md) - ハンドルのメモリ確保の詳細
- [ハンドル使用例](handle-usage.md) - ハンドルの使い方と実例

## 主要コンポーネント

### バックエンドレンダラー

バックエンドレンダラーは、以下の主要なコンポーネントで構成されています：

- **Handle**: 型安全なハンドルシステム
- **HandleAllocator**: 効率的なメモリアロケーター
- **Driver**: レンダリングAPIの抽象化レイヤー

### ハンドルシステムの特徴

1. **型安全性**: テンプレートベースの型安全なハンドル
2. **効率的なメモリ管理**: 3段階のメモリプーリング
3. **世代管理**: ABA問題を防ぐための世代カウンター

## 関連ファイル

- `engine/include/ocf/renderer/backend/Handle.h` - ハンドル定義
- `engine/src/renderer/backend/HandleAllocator.h` - アロケーター実装
- `engine/src/renderer/backend/HandleAllocator.cpp` - アロケーター実装
- `engine/include/ocf/core/Allocator.h` - 基本アロケーター
