import os
import subprocess

EXECUTABLE_NAME = 'text'
ENCODING = "utf-8"
LOGFILE = ".~valgrind.txt"
NO_LEAK_PHRASE = "no leaks are possible"
NO_ERROR_PHRASE = "0 errors from 0 contexts"
LEAK_PHRASE = "LEAK SUMMARY:"
ERROR_PHRASE = "ERROR SUMMARY:"

class colors:
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'

tests = [file for file in os.listdir(os.path.join(os.getcwd(), 'tests')) if file.startswith('in')]
text_test_ok, text_test_fail, leak_test_ok, leak_test_fail, error_test_ok, error_test_fail = 0, 0, 0, 0, 0, 0
os.system('make')
print()

for test in tests:
    test_no = int(test[2:-4])
    command = f'./{EXECUTABLE_NAME} < {os.path.join(os.getcwd(), "tests", test)}'
    valgrind_command = f'valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=\"{LOGFILE}\" {command}'
    
    text_result = subprocess.check_output(valgrind_command, shell=True).decode(ENCODING)
    with open(os.path.join(os.getcwd(), "tests", f"out{test_no}.txt")) as file:
        correct_result = file.read()

    with open(LOGFILE) as file:
        valgrind_result = file.read()
    leaks_index = valgrind_result.find(NO_LEAK_PHRASE)
    errors_index = valgrind_result.find(NO_ERROR_PHRASE)

    print(f"TEST NO {test_no} RESULTS:\n========================================")
    if text_result == correct_result:
        print(colors.OKGREEN+f"TEXT OUTPUT TEST NO {test_no} PASSED"+colors.ENDC)
        text_test_ok += 1
    elif text_result == '':
        print(colors.WARNING+f"TEXT OUTPUT TEST NO {test_no} MAY HAVE BEEN PASSED"+colors.ENDC)
        text_test_ok += 1
    else:
        print(colors.FAIL+f"TEXT OUTPUT TEST NO {test_no} FAILED"+colors.ENDC)
        text_test_fail += 1
    
    if leaks_index >= 0:
        print(f"{colors.OKGREEN}LEAK TEST NO {test_no} PASSED{colors.ENDC}")
        leak_test_ok += 1
    else:
        print(f"{colors.FAIL}LEAK TEST NO {test_no} FAILED{colors.ENDC}")
        leak_summary_index = valgrind_result.find(LEAK_PHRASE)
        print(valgrind_result[leak_summary_index:valgrind_result.find("suppressed:", leak_summary_index)])
        leak_test_fail += 1
    
    if errors_index >= 0:
        print(f"{colors.OKGREEN}ERROR TEST NO {test_no} PASSED{colors.ENDC}")
        error_test_ok += 1
    else:
        print(f"{colors.FAIL}ERROR TEST NO {test_no} FAILED{colors.ENDC}")
        print(valgrind_result[valgrind_result.find(ERROR_PHRASE):])
        error_test_fail += 1
    print("========================================\n")
os.remove(LOGFILE)

print("REPORT SUMMARY:")
print(f"TEXT: {colors.OKGREEN}passed {text_test_ok}{colors.ENDC}, {colors.FAIL}failed {text_test_fail}{colors.ENDC} of {text_test_ok+text_test_fail}")
print(f"LEAKS: {colors.OKGREEN}passed {leak_test_ok}{colors.ENDC}, {colors.FAIL}failed {leak_test_fail}{colors.ENDC} of {leak_test_ok+leak_test_fail}")
print(f"ERRORS: {colors.OKGREEN}passed {error_test_ok}{colors.ENDC}, {colors.FAIL}failed {error_test_fail}{colors.ENDC} of {error_test_ok+error_test_fail}")