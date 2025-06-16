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
        ]
    }
    
    for name, program in examples.items():
        save_bytecode(name, program)

if __name__ == "__main__":
    generate_examples()
    print("All examples generated in bin/examples/")
