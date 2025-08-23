import sys
import os
from pathlib import Path
import codecs


# ファイルのエンコーディングを変換する
def convert_file_encoding(file_path, encoding) -> bool:
    # ファイルのエンコードがUTF-8か確認
    with open(file_path, "rb") as f:
        raw = f.read()
        try:
            raw.decode('utf-8')
            return False
        except UnicodeDecodeError:
            pass

    with open(file_path, "r", encoding=encoding) as f:
        lines = f.readlines()

    with open(file_path, "w", encoding="utf-8") as f:
        f.writelines(lines)

    return True


# メイン関数
def main():
    args = sys.argv

    if len(args) != 2:
        print("Usage: python convert.py <input_path>")
        return

    input_path = Path(args[1])

    # パスが存在するか確認
    if not input_path.exists():
        print("Path does not exist")
        return

    # 絶対パスに変換
    if input_path.is_absolute() == False:
        input_path = input_path.resolve()

    dir = os.listdir(str(input_path))

    # ファイルのみ取得
    files = []
    for file in dir:
        full_path = os.path.join(input_path, file)
        if os.path.isfile(full_path):
            files.append(full_path)

    # ファイルのエンコーディングを変換
    for i in range(len(files)):
        if convert_file_encoding(files[i], "shift-jis"):
            print(f"[{i:3}] Converted: {files[i]}")
        else:
            print(f"[{i:3}] Already UTF-8: {files[i]}")


if __name__ == "__main__":
    main()