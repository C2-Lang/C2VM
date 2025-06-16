import sys
import os
import subprocess
from colorama import init, Fore, Style

init(autoreset=True)

def get_project_root():
    return os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

def ensure_directory(path):
    if not os.path.exists(path):
        os.makedirs(path)

def run_test(name, program, expected_output, check_registers=True):
    print(f"\n{Fore.CYAN}Running test: {name}{Style.RESET_ALL}")
    
    project_root = get_project_root()
    bin_dir = os.path.join(project_root, "bin")
    ensure_directory(bin_dir)
    
    test_file = os.path.join(bin_dir, "test.c2b")
    with open(test_file, "wb") as f:
        f.write(bytes(program))
    
    vm_path = os.path.join(bin_dir, "c2vm.exe")
    result = subprocess.run([vm_path, test_file], capture_output=True, text=True)
    
    success = expected_output in result.stdout
    if success:
        print(f"{Fore.GREEN}✓ Test passed{Style.RESET_ALL}")
    else:
        print(f"{Fore.RED}✗ Test failed{Style.RESET_ALL}")
        print(f"Expected output: {expected_output}")
        print(f"Got output: {result.stdout}")
    
    return success

if __name__ == "__main__":
    tests = [
        ("Division by Zero", [
            0x01, 0x00, 10,   # LOAD R0, 10
            0x01, 0x01, 0,    # LOAD R1, 0
            0x09, 0x00, 0x01, # DIV R0, R1
            0x00              # HALT
        ], "Division by zero"),
        
        ("Stack Overflow", [
            0x01, 0x00, 1,    # LOAD R0, 1
            0x05, 0x00,       # PUSH R0 (repeat many times)
            0x05, 0x00,
            0x05, 0x00,
            0x00              # HALT
        ], "Stack overflow")
    ]
    
    failed = 0
    for name, program, expected_output in tests:
        if not run_test(name, program, expected_output):
            failed += 1
    
    print(f"\n{Fore.CYAN}Test Summary:{Style.RESET_ALL}")
    print(f"Total: {len(tests)}")
    print(f"{Fore.GREEN}Passed: {len(tests) - failed}{Style.RESET_ALL}")
    if failed:
        print(f"{Fore.RED}Failed: {failed}{Style.RESET_ALL}")
        sys.exit(1)
