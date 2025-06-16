import sys
import os
import subprocess
from colorama import init, Fore, Style

init(autoreset=True)

def ensure_directory(path):
    if not os.path.exists(path):
        os.makedirs(path)

def run_test(name, program, expected_r0, expected_r1=None):
    print(f"\n{Fore.CYAN}Running test: {name}{Style.RESET_ALL}")
    
    ensure_directory("bin")
    with open("bin/test.c2b", "wb") as f:
        f.write(bytes(program))
    
    # Run VM
    result = subprocess.run(["../bin/c2vm.exe", "bin/test.c2b"], 
                          capture_output=True, text=True)
    
    # Parse output
    r0 = int(result.stdout.split("R0: ")[1].split("\n")[0])
    r1 = int(result.stdout.split("R1: ")[1].split("\n")[0]) if "R1: " in result.stdout else None
    
    # Check results
    success = r0 == expected_r0 and (expected_r1 is None or r1 == expected_r1)
    if success:
        print(f"{Fore.GREEN}✓ Test passed{Style.RESET_ALL}")
    else:
        print(f"{Fore.RED}✗ Test failed{Style.RESET_ALL}")
        print(f"Expected: R0={expected_r0}" + 
              (f", R1={expected_r1}" if expected_r1 is not None else ""))
        print(f"Got:      R0={r0}" + (f", R1={r1}" if r1 is not None else ""))
    
    return success

if __name__ == "__main__":
    tests = [
        ("Addition", [
            0x01, 0x00, 5,    # LOAD R0, 5
            0x01, 0x01, 3,    # LOAD R1, 3
            0x03, 0x00, 0x01, # ADD R0, R1
            0x00              # HALT
        ], 8, 3),
        
        ("Multiplication", [
            0x01, 0x00, 4,    # LOAD R0, 4
            0x01, 0x01, 5,    # LOAD R1, 5
            0x08, 0x00, 0x01, # MUL R0, R1
            0x00              # HALT
        ], 20, 5),
        
        ("Division", [
            0x01, 0x00, 10,   # LOAD R0, 10
            0x01, 0x01, 2,    # LOAD R1, 2
            0x09, 0x00, 0x01, # DIV R0, R1
            0x00              # HALT
        ], 5, 2)
    ]
    
    failed = 0
    for name, program, expected_r0, expected_r1 in tests:
        if not run_test(name, program, expected_r0, expected_r1):
            failed += 1
    
    print(f"\n{Fore.CYAN}Test Summary:{Style.RESET_ALL}")
    print(f"Total: {len(tests)}")
    print(f"{Fore.GREEN}Passed: {len(tests) - failed}{Style.RESET_ALL}")
    if failed:
        print(f"{Fore.RED}Failed: {failed}{Style.RESET_ALL}")
        sys.exit(1)
