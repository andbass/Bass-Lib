
from ctypes import CDLL
from fnmatch import fnmatch

import sys
import fileinput
import time
import datetime as dt

class colors:
    BOLD = '\033[1m'
    BLUE = '\033[94m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    RED = '\033[91m'
    END = '\033[0m'

bar = "-----"

test_filter = "*"
if len(sys.argv) > 1:
    test_filter = sys.argv[1]

test_lib = CDLL("build/libtest.so")

test_syms = []
with open("build/test-syms") as syms:
    for line in syms.readlines():
        spaced = line.split(" ")
        symbol = spaced[0]

        if symbol.startswith("test_") and fnmatch(symbol[5:], test_filter):
            test_syms.append(symbol)

passed = 0
failed = 0

failed_tests = []

for i, sym in enumerate(test_syms):
    test_name = sym.replace("test_", "")

    running_text = ("Running " + colors.BOLD + colors.YELLOW + test_name + colors.END).ljust(40, " ")
    completion_ratio = str(i + 1) + "/" + str(len(test_syms))

    print(running_text + completion_ratio, end="\t")
    sys.stdout.flush()

    func = test_lib[sym]

    start_time = time.time()
    ret = func()
    end_time = time.time()

    if ret == 0:
        failed += 1
        failed_tests.append(test_name)
    elif ret == 1:
        passed += 1

        time_elapsed = end_time - start_time
        time_factor = 1
        time_unit = "s"

        if time_elapsed < 1e-4:
            time_factor = 1e6
            time_unit = "\u00b5s"
        elif time_elapsed < 1e-2:
            time_factor = 1e3
            time_unit = "ms"

        print(colors.GREEN + str(round(time_elapsed * time_factor, 3)).rjust(10) + " " + time_unit + colors.END)
    else:
        print("INVALID RETURN CODE FROM TEST: " + sym)
        break

print(bar * 5)
print("Tests passed: " + colors.BOLD + colors.GREEN + str(passed) + "/" + str(len(test_syms)) + colors.END)
print("Tests failed: " + colors.BOLD + colors.RED + str(failed) + "/" + str(len(test_syms)) + colors.END, end=" ")

if len(failed_tests) > 0:
    failed_tests_color = [colors.BOLD + colors.YELLOW + test + colors.END for test in failed_tests]
    print("(" + ", ".join(failed_tests_color) + ")")
else:
    print()


