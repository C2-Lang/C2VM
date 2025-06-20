import os
from bytecode_gen import *
from vm_constants import *


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
            (OpCode.LOAD, 0x00, 10),    
            (OpCode.LOAD, 0x01, 5),     
            (OpCode.ADD,  0x00, 0x01),  
            (OpCode.MUL,  0x00, 0x01), 
            (OpCode.MUL,  0x00, 0x01),
            OpCode.HALT
        ],
        
        "stack": [
            (OpCode.LOAD, 0x00, 42),    
            (OpCode.PUSH, 0x00),        
            (OpCode.LOAD, 0x00, 24),    
            (OpCode.POP,  0x01),        
            OpCode.HALT
        ],
        
        "logic": [
            (OpCode.LOAD, 0x00, 0xFF),  
            (OpCode.LOAD, 0x01, 0x0F),  
            (OpCode.AND,  0x00, 0x01),  
            (OpCode.NOT,  0x00),        
            OpCode.HALT
        ],
        
        "compare": [
            (OpCode.LOAD, 0x00, 100),   
            (OpCode.LOAD, 0x01, 50),    
            (OpCode.GT,   0x00, 0x01),  
            (OpCode.EQ,   0x00, 0x01),  
            OpCode.HALT
        ],
        
        "div_by_zero": [
            (OpCode.LOAD, 0x00, 10),    
            (OpCode.LOAD, 0x01, 0),     
            (OpCode.DIV,  0x00, 0x01),  
            OpCode.HALT
        ],

        "syscall_hello": [
            
            (OpCode.LOAD_TYPE, 0x00, ValueType.UINT64),
            (OpCode.LOAD, 0x00, 0x01),      
            (OpCode.LOAD_TYPE, 0x01, ValueType.INT64),
            (OpCode.LOAD, 0x01, ord('H')),
            (OpCode.SYSCALL,),
            (OpCode.LOAD_TYPE, 0x01, ValueType.INT64),
            (OpCode.LOAD, 0x01, ord('e')),
            (OpCode.SYSCALL,),
            (OpCode.LOAD_TYPE, 0x01, ValueType.INT64),
            (OpCode.LOAD, 0x01, ord('l')),
            (OpCode.SYSCALL,),
            (OpCode.LOAD_TYPE, 0x01, ValueType.INT64),
            (OpCode.LOAD, 0x01, ord('l')),
            (OpCode.SYSCALL,),
            (OpCode.LOAD_TYPE, 0x01, ValueType.INT64),
            (OpCode.LOAD, 0x01, ord('o')),
            (OpCode.SYSCALL,),
            (OpCode.LOAD_TYPE, 0x01, ValueType.INT64),
            (OpCode.LOAD, 0x01, ord('\n')),
            (OpCode.SYSCALL,),
            OpCode.HALT
        ],

        "syscall_echo": [
            
            (OpCode.LOAD_TYPE, 0x00, ValueType.UINT64),
            (OpCode.LOAD, 0x00, 0x02),      
            (OpCode.SYSCALL,),
            (OpCode.LOAD_TYPE, 0x02, ValueType.INT64),
            (OpCode.LOAD, 0x02, ord('\n')),
            (OpCode.TYPE_EQ, 0x00, 0x02),
            (OpCode.JNZ, 12),
            (OpCode.LOAD_TYPE, 0x00, ValueType.UINT64),
            (OpCode.LOAD, 0x00, 0x01),
            (OpCode.SYSCALL,),
            (OpCode.JMP, 0),
            OpCode.HALT
        ],

        "syscall_exit": [
            
            (OpCode.LOAD, 0x00, 0x00),      
            (OpCode.LOAD, 0x01, 42),        
            (OpCode.SYSCALL,),
            OpCode.HALT                      
        ],

        "file_write": [
            
            (OpCode.LOAD_TYPE, 0x01, ValueType.INT64),
            (OpCode.LOAD, 0x01, ord('t')),
            (OpCode.STORE, 0x01, 0),
            (OpCode.LOAD, 0x01, ord('e')),
            (OpCode.STORE, 0x01, 1),
            (OpCode.LOAD, 0x01, ord('s')),
            (OpCode.STORE, 0x01, 2),
            (OpCode.LOAD, 0x01, ord('t')),
            (OpCode.STORE, 0x01, 3),
            (OpCode.LOAD, 0x01, ord('.')),
            (OpCode.STORE, 0x01, 4),
            (OpCode.LOAD, 0x01, ord('t')),
            (OpCode.STORE, 0x01, 5),
            (OpCode.LOAD, 0x01, ord('x')),
            (OpCode.STORE, 0x01, 6),
            (OpCode.LOAD, 0x01, ord('t')),
            (OpCode.STORE, 0x01, 7),
            (OpCode.LOAD, 0x01, 0),
            (OpCode.STORE, 0x01, 8),

            
            (OpCode.LOAD_TYPE, 0x00, ValueType.UINT64),
            (OpCode.LOAD, 0x00, SYS_OPEN),
            (OpCode.LOAD_TYPE, 0x01, ValueType.UINT64),
            (OpCode.LOAD, 0x01, 0),           
            (OpCode.LOAD_TYPE, 0x02, ValueType.UINT64),
            (OpCode.LOAD, 0x02, 1),           
            (OpCode.SYSCALL,),

            
            (OpCode.LOAD_TYPE, 0x01, ValueType.INT64),
            (OpCode.LOAD, 0x01, ord('H')),
            (OpCode.STORE, 0x01, 32),
            (OpCode.LOAD, 0x01, ord('i')),
            (OpCode.STORE, 0x01, 33),
            (OpCode.LOAD, 0x01, ord('!')),
            (OpCode.STORE, 0x01, 34),

            
            (OpCode.LOAD_TYPE, 0x00, ValueType.UINT64),
            (OpCode.LOAD, 0x00, SYS_WRITE),
            (OpCode.LOAD_TYPE, 0x01, ValueType.INT64),
            (OpCode.ADD, 0x01, 0x00),        
            (OpCode.LOAD_TYPE, 0x02, ValueType.UINT64),
            (OpCode.LOAD, 0x02, 32),         
            (OpCode.LOAD_TYPE, 0x03, ValueType.UINT64),
            (OpCode.LOAD, 0x03, 3),          
            (OpCode.SYSCALL,),

            
            (OpCode.LOAD_TYPE, 0x00, ValueType.UINT64),
            (OpCode.LOAD, 0x00, SYS_CLOSE),
            (OpCode.LOAD_TYPE, 0x01, ValueType.INT64),
            (OpCode.ADD, 0x01, 0x00),
            (OpCode.SYSCALL,),

            OpCode.HALT
        ],

        "data_types": [
            # Still haven't added the data sections yet
        ],

        "if_statement": [
            (OpCode.LOAD_TYPE, 0x00, ValueType.UINT64),  
            (OpCode.LOAD, 0x00, 2),                      
            (OpCode.LOAD_TYPE, 0x01, ValueType.UINT64),  
            (OpCode.LOAD, 0x01, 10),                      
            (OpCode.GT, 0x00, 0x01),                     
            (OpCode.JZ, 15),                             
            
            
            (OpCode.LOAD_TYPE, 0x00, ValueType.UINT64),  
            (OpCode.LOAD, 0x00, 0x01),                   
            (OpCode.LOAD_TYPE, 0x01, ValueType.INT64),   
            (OpCode.LOAD, 0x01, ord('A')),               
            (OpCode.SYSCALL,),                           
            (OpCode.JMP, 15),                            
            
            
            (OpCode.LOAD_TYPE, 0x00, ValueType.UINT64),  
            (OpCode.LOAD, 0x00, 0x01),                   
            (OpCode.LOAD_TYPE, 0x01, ValueType.INT64),   
            (OpCode.LOAD, 0x01, ord('B')),               
            (OpCode.SYSCALL,),                           
            
            
            (OpCode.LOAD_TYPE, 0x00, ValueType.UINT64),  
            (OpCode.LOAD, 0x00, 0x01),                   
            (OpCode.LOAD_TYPE, 0x01, ValueType.INT64),   
            (OpCode.LOAD, 0x01, ord('\n')),              
            (OpCode.SYSCALL,),                           
            OpCode.HALT                                   
        ],

        "function_call": [
            (OpCode.LOAD, 0x00, 7),
            (OpCode.LOAD, 0x01, 5),
            (OpCode.LOAD_TYPE, 0x00, ValueType.UINT64),
            (OpCode.LOAD_TYPE, 0x01, ValueType.UINT64),
            (OpCode.CALL, 0x16, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),
            (OpCode.HALT,),
            (OpCode.ADD, 0x00, 0x01),
            (OpCode.RET,)
        ],
    }
    
    for name, program in examples.items():
        save_bytecode(name, program)

if __name__ == "__main__":
    generate_examples()
    print("All examples generated in bin/examples/")