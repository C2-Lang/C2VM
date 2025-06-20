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

def run_test(name, program, expected_output):
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
    # Define syscall numbers
    SYS_EXIT = 0x00
    SYS_PUTCHAR = 0x01
    SYS_OPEN = 0x07
    SYS_CLOSE = 0x08
    SYS_WRITE = 0x0A
    
    tests = [
        ("Memory Out of Bounds", [
            0x01, 0x00, 0xFF,   # LOAD R0, 255
            0x01, 0x01, 0x41,   # LOAD R1, 'A'
            0x02, 0x01, 0xFF,   # STORE R1, [255] (should work)
            0x02, 0x01, 0xFF,   # Store beyond memory bounds
            0x02, 0x01, 0xFF,   # Another invalid store
            0x00                # HALT
        ], "Invalid memory access (address:"),
        
        ("Memory Store and Load", [
            0x01, 0x00, 0x01,   # LOAD R0, SYS_PUTCHAR
            0x01, 0x01, 0x41,   # LOAD R1, 'A'
            0x02, 0x01, 0x20,   # STORE R1, [32]
            0x01, 0x02, 0x20,   # LOAD R2, [32]
            0x01, 0x01, 0x41,   # LOAD R1, 'A'
            0x19,               # SYSCALL
            0x00                # HALT
        ], "A"),
        
        ("Stack Memory Test", [
            0x01, 0x00, 0x01,   # LOAD R0, SYS_PUTCHAR
            0x01, 0x01, 0x42,   # LOAD R1, 'B'
            0x05, 0x01,         # PUSH R1
            0x06, 0x01,         # POP R1
            0x19,               # SYSCALL
            0x00                # HALT
        ], "B"),
        
        ("File Write Test", [
            # Write "Hello, VM!" to test.txt
            # Store filename "test.txt" at memory address 0x20
            0x01, 0x01, 0x74,   # LOAD R1, 't'
            0x02, 0x01, 0x20,   # STORE R1, [0x20]
            0x01, 0x01, 0x65,   # LOAD R1, 'e'
            0x02, 0x01, 0x21,   # STORE R1, [0x21]
            0x01, 0x01, 0x73,   # LOAD R1, 's'
            0x02, 0x01, 0x22,   # STORE R1, [0x22]
            0x02, 0x01, 0x23,   # STORE R1, [0x23]
            0x01, 0x01, 0x2E,   # LOAD R1, '.'
            0x02, 0x01, 0x24,   # STORE R1, [0x24]
            0x01, 0x01, 0x74,   # LOAD R1, 't'
            0x02, 0x01, 0x25,   # STORE R1, [0x25]
            0x01, 0x01, 0x78,   # LOAD R1, 'x'
            0x02, 0x01, 0x26,   # STORE R1, [0x26]
            0x01, 0x01, 0x74,   # LOAD R1, 't'
            0x02, 0x01, 0x27,   # STORE R1, [0x27]
            0x01, 0x01, 0x00,   # LOAD R1, null terminator
            0x02, 0x01, 0x28,   # STORE R1, [0x28]

            # Open file for writing (mode = 1)
            0x01, 0x00, 0x07,   # LOAD R0, SYS_OPEN
            0x01, 0x01, 0x20,   # LOAD R1, filename address
            0x01, 0x02, 0x01,   # LOAD R2, write mode
            0x19,               # SYSCALL
            
            # Store file descriptor
            0x02, 0x00, 0x29,   # STORE R0, [0x29]
            
            # Store "Hello, VM!" at address 0x30
            0x01, 0x01, 0x48,   # LOAD R1, 'H'
            0x02, 0x01, 0x30,   # STORE R1, [0x30]
            0x01, 0x01, 0x65,   # LOAD R1, 'e'
            0x02, 0x01, 0x31,   # STORE R1, [0x31]
            0x01, 0x01, 0x6C,   # LOAD R1, 'l'
            0x02, 0x01, 0x32,   # STORE R1, [0x32]
            0x02, 0x01, 0x33,   # STORE R1, [0x33]
            0x01, 0x01, 0x6F,   # LOAD R1, 'o'
            0x02, 0x01, 0x34,   # STORE R1, [0x34]
            0x01, 0x01, 0x2C,   # LOAD R1, ','
            0x02, 0x01, 0x35,   # STORE R1, [0x35]
            0x01, 0x01, 0x20,   # LOAD R1, ' '
            0x02, 0x01, 0x36,   # STORE R1, [0x36]
            0x01, 0x01, 0x56,   # LOAD R1, 'V'
            0x02, 0x01, 0x37,   # STORE R1, [0x37]
            0x01, 0x01, 0x4D,   # LOAD R1, 'M'
            0x02, 0x01, 0x38,   # STORE R1, [0x38]
            0x01, 0x01, 0x21,   # LOAD R1, '!'
            0x02, 0x01, 0x39,   # STORE R1, [0x39]
            
            # Write to file
            0x01, 0x00, 0x0A,   # LOAD R0, SYS_WRITE
            0x01, 0x01, 0x29,   # LOAD R1, fd
            0x01, 0x02, 0x30,   # LOAD R2, buffer address
            0x01, 0x03, 0x0A,   # LOAD R3, length (10 bytes)
            0x19,               # SYSCALL
            
            # Close file
            0x01, 0x00, 0x08,   # LOAD R0, SYS_CLOSE
            0x01, 0x01, 0x29,   # LOAD R1, fd
            0x19,               # SYSCALL
            
            0x00                # HALT
        ], ""),
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
