#! /usr/bin/env python3

import argparse
import os
import re
import subprocess
from ctypes import cdll
import sys
from typing import Any

NM_COMMAND = '%s -D %s'
RE_FUNCTION_CALL = r'(\w+)\((.*)\)'

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("shared_library", help="the shared libary file (ends with `.so`)")
    parser.add_argument("--function_call", help="a c function call with arguments", default=None)
    parser.add_argument("--nm_fp", help="the file path to the `nm` binary", default="nm")
    args = parser.parse_args()
    nm_output = nm(args.nm_fp, args.shared_library)
    c_functions = filter_c_functions(nm_output)
    sl = load_shared_library(args.shared_library)
    local_env = construct_local_dict(sl, c_functions)
    if args.function_call:
        function_name, function_args = parse_function_call(args.function_call)
        check_function_name(c_functions, function_name)
        print(sl.__getattr__(function_name)(*function_args))
    else:
        print_shared_functions(c_functions)
        interactive_loop(sl, local_env)
            
def interactive_loop(sl, local_env):
    global print_shared_functions
    global_dict = {}
    print_shared_functions = lambda *_, **__: None
    try:
        while (print("> ", end=""), fc := input()):
            if 0 == len(fc):
                continue
            try:
                result = eval_or_exec(fc, global_dict, local_env)
                if result != None:
                    print(result)
            except Exception as e:
                print(e)
    except EOFError:
        pass
    except KeyboardInterrupt:
        pass
    except:
        raise

def eval_or_exec(*args):
    try:
        return eval(*args)
    except:
        pass
    exec(*args)

def construct_local_dict(sl, c_functions):
    return {
        str(fname): (lambda *args: sl.__getattr__(fname)(*args))
        for fname in c_functions
    }

def load_shared_library(file_name: str):
    return cdll.LoadLibrary(file_name)

# Returns the output of `NM_COMMAND`, errors if the command fails
def nm(nm_file_path: str, shared_library: str) -> str:
    proc = subprocess.run(
        NM_COMMAND%(nm_file_path,shared_library),
        shell=True,
        capture_output=True,
    )
    check_stderr(proc.stderr)
    return proc.stdout.decode("utf-8")

def check_stderr(stderr: bytes) -> None:
    perror = lambda *args, **kwargs: print(*args, file=sys.stderr, **kwargs)
    if 0 < len(stderr):
        perror("ERROR")
        perror("-----\n")
        perror(stderr.decode("utf-8"), end="")
        exit(1)

# Given the output of the `nm -D` command, return the c bindings
# NOTE: we use the gimmick that all mangled c bindings start with r"_[^_]" in regex
def filter_c_functions(nm_output: str) -> list[str]:
    functions = [line.split()[2] for line in nm_output.strip().split("\n") if 3 <= len(line.split())]
    c_functions = [func for func in functions if 2 <= len(func) and func[0] == "_" and func[1] != "_"]
    # print(f"{functions=}")
    # print(f"{c_functions=}")
    return [func[1:] for func in c_functions] # omit the "_" at the beginning of the string

# Print the shared functions
def print_shared_functions(c_functions: list[str], print=print):
    if 0 == len(c_functions):
        print("No c function bindings found!")
        return
    print("C function bindings:")
    for func in c_functions:
        print(f"  - {func}")

# TODO: Better error handling!
# TODO: Use regex to check the validity of the function call, add hooks for different
#       ctypes....
def parse_function_call(function_call: str, exit_fn=exit) -> tuple[str, list[Any]]:
    rfunc_call = re.compile(RE_FUNCTION_CALL)
    matches = rfunc_call.match(function_call)
    if not matches:
        print_function_call_error(exit_fn)
        return function_call, []
    assert(matches != None)
    groups = matches.groups()
    fname = groups[0]
    arg_array = [s.strip() for s in groups[1].split(",") if len(s.strip())]
    fargs = parse_function_arguments(arg_array)
    return fname, fargs

def parse_function_arguments(function_args: list[str]) -> list[Any]:
    return [int(i) for i in function_args]

def check_function_name(allowed_funcs: list[str], func: str, exit_fn=exit) -> None:
    perror = lambda *args, **kwargs: print(*args, file=sys.stderr, **kwargs)
    if func not in allowed_funcs:
        perror(f"Function `{func}` is not a callable c function!")
        print_shared_functions(allowed_funcs, print=perror)
        exit_fn(1)

def print_function_call_error(exit_fn):
    perror = lambda *args, **kwargs: print(*args, file=sys.stderr, **kwargs)
    perror("The function call is improperly formatted, the parsing step failed")
    exit_fn(1)

if __name__ == '__main__':
    main()
