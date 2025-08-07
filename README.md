# 読み込み
## unistd.h
システムコール
read
write

## fcntl.h
open

# ディスクリプタ関連
おさえておきたいところ↓

このコードではファイルから読み取って、標準出力に書き込むことをしている。

## 間違ったところ
if (write(fd, bf, n) < 0)
のときはtest.txt: Bad file descriptorと表示されるのに
if (write(STDOUT_FILENO, bf, n) < 0)
のときは正しく表示されるのはなぜか？

この違いは、**ファイルディスクリプタのアクセス権限**によるもの。

## 2つのケースの違い

### ケース1: `write(fd, bf, n)` - エラーが発生
```c
fd = open(path, O_RDONLY);  // fd=3, 読み取り専用で開く
write(fd, bf, n);           // fd=3 に書き込もうとする → ❌ エラー
```

### ケース2: `write(STDOUT_FILENO, bf, n)` - 正常動作
```c
fd = open(path, O_RDONLY);      // fd=3, 読み取り専用で開く
write(STDOUT_FILENO, bf, n);    // fd=1 (標準出力) に書き込む → ✅ 正常
```

## なぜエラーが発生するのか

**ファイルディスクリプタには、開く時に指定したアクセス権限が設定されます：**

| ファイルディスクリプタ | 開き方 | 読み取り | 書き込み |
|-------------------|--------|----------|----------|
| fd=3 (test.txt) | `O_RDONLY` | ✅ 可能 | ❌ **不可** |
| fd=1 (標準出力) | 自動設定 | ❌ 不可 | ✅ **可能** |

## 実際の動作

```c
// これは正常に動作
n = read(fd, bf, sizeof(bf));        // fd=3から読み取り ✅
write(STDOUT_FILENO, bf, n);         // fd=1に書き込み ✅

// これはエラーになる
n = read(fd, bf, sizeof(bf));        // fd=3から読み取り ✅  
write(fd, bf, n);                    // fd=3に書き込み ❌ "Bad file descriptor"
```

## エラーメッセージの理由

`write(fd, bf, n)` で fd=3（読み取り専用）に書き込もうとすると：

1. システムが「このファイルディスクリプタは書き込み禁止」と判断
2. **EBADF (Bad file descriptor)** エラーを返す
3. `perror("test.txt")` が「test.txt: Bad file descriptor」と表示

## 正しい cat の動作

```c
read(fd, bf, n);           // ファイルから読み取り
write(STDOUT_FILENO, bf, n); // 標準出力に書き込み（画面表示）
```

つまり、**ファイルから読み取って、画面に表示する**のが `cat` コマンドの正しい動作です。