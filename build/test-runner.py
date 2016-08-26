
from ctypes import CDLL

import sys
import fileinput

class colors:
    BOLD = '\033[1m'
    BLUE = '\033[94m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    RED = '\033[91m'
    END = '\033[0m'

bar = "-----"

test_lib = CDLL("build/libtest.so")

test_syms = []
with open("build/test-syms") as syms:
    for line in syms.readlines():
        spaced = line.split(" ")
        symbol = spaced[0]

        if symbol.startswith("test_"):
            test_syms.append(symbol)

passed = 0
failed = 0

failed_tests = []

for i, sym in enumerate(test_syms):
    test_name = sym.replace("test_", "")

    print(("Running " + colors.BOLD + colors.YELLOW + test_name + colors.END).ljust(40, " ") + str(i + 1) + "/" + str(len(test_syms)), end="\t")
    sys.stdout.flush()

    ret = test_lib[sym]()

    if ret == 0:
        failed += 1
        failed_tests.append(test_name)
    elif ret == 1:
        passed += 1
        print()
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


