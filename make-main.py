#! /usr/bin/env python3

import argparse
import subprocess
import sys

FUNCDECL_COMMAND='clang -Xclang -ast-dump -fsyntax-only %s | grep FunctionDecl'

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("file_name", help="the name of the file to create a `.cpp' driver for.")
    args = parser.parse_args()
    make_main(args.file_name)

def make_main(file_name: str) -> None:
    proc = subprocess.run(
        FUNCDECL_COMMAND%(file_name),
        shell=True,
        capture_output=True,
    )
    check_clang_error(proc.stderr)
    parse_func_decl_line(proc.stdout.decode("utf-8").strip())

def check_clang_error(stderr: bytes) -> None:
    if len(stderr):
        print(stderr.decode("utf-8"), file=sys.stderr, end="")
        exit(1)

def parse_func_decl_line(decl_lines: str) -> list[tuple[str, list[str]]]:
    print(decl_lines)
    return []

if __name__ == '__main__':
    main()
