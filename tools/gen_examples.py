import os
from bytecode_gen import OpCode, generate_bytecode

def ensure_directory(path):
    if not os.path.exists(path):
        os.makedirs(path, exist_ok=True)

def save_bytecode(name, program):
    examples_dir = os.path.join(os.path.dirname(os.path.dirname(__file__)), "examples")
    ensure_directory(examples_dir)
    
    output_file = os.path.join(examples_dir, f"{name}.c2b")
    with open(output_file, "wb") as f:
        f.write(bytes(generate_bytecode(program)))
    print(f"Generated {output_file}")

def generate_examples():
    
    examples = {
        "arithmetic": [
            (OpCode.LOAD, 0x00, 10),    # R0 = 10
            (OpCode.LOAD, 0x01, 5),     # R1 = 5
            (OpCode.ADD,  0x00, 0x01),  # R0 += R1 (15)
            (OpCode.MUL,  0x00, 0x01), 
            (OpCode.MUL,  0x00, 0x01),
            OpCode.HALT
        ],
        
        "stack": [
            (OpCode.LOAD, 0x00, 42),    # R0 = 42
            (OpCode.PUSH, 0x00),        # Push R0
            (OpCode.LOAD, 0x00, 24),    # R0 = 24
            (OpCode.POP,  0x01),        # Pop into R1
            OpCode.HALT
        ],
        
        "logic": [
            (OpCode.LOAD, 0x00, 0xFF),  # R0 = 255
            (OpCode.LOAD, 0x01, 0x0F),  # R1 = 15
            (OpCode.AND,  0x00, 0x01),  # R0 &= R1
            (OpCode.NOT,  0x00),        # R0 = ~R0
            OpCode.HALT
        ],
        
        "compare": [
            (OpCode.LOAD, 0x00, 100),   # R0 = 100
            (OpCode.LOAD, 0x01, 50),    # R1 = 50
            (OpCode.GT,   0x00, 0x01),  # R0 > R1?
            (OpCode.EQ,   0x00, 0x01),  # R0 == R1?
            OpCode.HALT
        ],
        
        "div_by_zero": [
            (OpCode.LOAD, 0x00, 10),    # R0 = 10
            (OpCode.LOAD, 0x01, 0),     # R1 = 0
            (OpCode.DIV,  0x00, 0x01),  # R0 /= R1 (error)
            OpCode.HALT
        ],

        "syscall_hello": [
            # Print "Hello, World!\n"
            (OpCode.LOAD, 0x00, 0x01),      # SYS_PUTCHAR
            (OpCode.LOAD, 0x01, ord('H')),
            (OpCode.SYSCALL,),
            (OpCode.LOAD, 0x01, ord('e')),
            (OpCode.SYSCALL,),
            (OpCode.LOAD, 0x01, ord('l')),
            (OpCode.SYSCALL,),
            (OpCode.LOAD, 0x01, ord('l')),
            (OpCode.SYSCALL,),
            (OpCode.LOAD, 0x01, ord('o')),
            (OpCode.SYSCALL,),
            (OpCode.LOAD, 0x01, ord(',')),
            (OpCode.SYSCALL,),
            (OpCode.LOAD, 0x01, ord(' ')),
            (OpCode.SYSCALL,),
            (OpCode.LOAD, 0x01, ord('W')),
            (OpCode.SYSCALL,),
            (OpCode.LOAD, 0x01, ord('o')),
            (OpCode.SYSCALL,),
            (OpCode.LOAD, 0x01, ord('r')),
            (OpCode.SYSCALL,),
            (OpCode.LOAD, 0x01, ord('l')),
            (OpCode.SYSCALL,),
            (OpCode.LOAD, 0x01, ord('d')),
            (OpCode.SYSCALL,),
            (OpCode.LOAD, 0x01, ord('!')),
            (OpCode.SYSCALL,),
            (OpCode.LOAD, 0x01, ord('\n')),
            (OpCode.SYSCALL,),
            OpCode.HALT
        ],

        "syscall_echo": [
            # Read char and echo it back until newline
            (OpCode.LOAD, 0x00, 0x02),      # SYS_GETCHAR
            (OpCode.SYSCALL,),              # Get char into R0
            (OpCode.LOAD, 0x02, ord('\n')), # Load \n into R2
            (OpCode.EQ, 0x00, 0x02),        # Compare with \n
            (OpCode.JNZ, 12),               # Jump to HALT if newline
            (OpCode.LOAD, 0x00, 0x01),      # SYS_PUTCHAR
            (OpCode.SYSCALL,),              # Print the char
            (OpCode.JMP, 0),                # Loop back to start
            OpCode.HALT
        ],

        "syscall_exit": [
            # Exit with status code 42
            (OpCode.LOAD, 0x00, 0x00),      # SYS_EXIT
            (OpCode.LOAD, 0x01, 42),        # Exit code 42
            (OpCode.SYSCALL,),
            OpCode.HALT                      # Should not reach here
        ]
    }
    
    for name, program in examples.items():
        save_bytecode(name, program)

if __name__ == "__main__":
    generate_examples()
    print("All examples generated in bin/examples/")
