
# ⚙️ C² Virtual Machine (C2VM)

A lightweight, 64-bit register-based virtual machine designed for the C² programming language.
C2VM provides a minimal yet powerful instruction set ideal for educational use, scripting, or embedding custom logic.
<p align="center">
  <img src= "https://cdn.discordapp.com/attachments/1153973442824118323/1384274565898829945/c2vmLogo.png?ex=6851d5a7&is=68508427&hm=d2f0e2956099bd0984ea46aff3aa7684a42460e1c4a3ef603de35090523e6049&"/>
</p>

## 🚀 Features

* **64-bit Register-Based Architecture**
* **Basic Arithmetic Instructions**: `ADD`, `SUB`, `MUL`, `DIV`
* **Logical Operations**: `AND`, `OR`, `XOR`, `NOT`
* **Comparison Operators**: `EQ`, `LT`, `GT`
* **Stack Operations**: `PUSH`, `POP`
* **Control Flow**: `JMP`, `JZ`, `JNZ`, `JGT`, `JLT`
* **Function Call Support**: `CALL`, `RET`
* **Interrupt Handling System**
* \*\*Debug Mode with Execution Tracing\`

---

## 🧠 Registers

| Name  | Purpose               |
| ----- | --------------------- |
| R0–R3 | General-purpose       |
| IP    | Instruction Pointer   |
| SP    | Stack Pointer         |
| BP    | Base Pointer          |
| FLAGS | Status/Flags register |

---

## 🧱 Memory Model

* **Total Memory**: 1MB addressable space
* **Stack**: Grows **downward** from top of memory
* **Overflow Protection**: Built-in to prevent crashes

---

## 🧾 Instruction Set

| Mnemonic | Hex    | Description              |
| -------- | ------ | ------------------------ |
| `HALT`   | `0x00` | Halt the VM              |
| `LOAD`   | `0x01` | Load value into register |
| `STORE`  | `0x02` | Store register to memory |
| `ADD`    | `0x03` | Add registers            |
| `SUB`    | `0x04` | Subtract registers       |
| `PUSH`   | `0x05` | Push register to stack   |
| `POP`    | `0x06` | Pop value to register    |
| `JMP`    | `0x07` | Unconditional jump       |
| `MUL`    | `0x08` | Multiply registers       |
| `DIV`    | `0x09` | Divide registers         |
| `AND`    | `0x0A` | Bitwise AND              |
| `OR`     | `0x0B` | Bitwise OR               |
| `XOR`    | `0x0C` | Bitwise XOR              |
| `NOT`    | `0x0D` | Bitwise NOT              |
| `EQ`     | `0x0E` | Compare: Equal           |
| `LT`     | `0x0F` | Compare: Less Than       |
| `GT`     | `0x10` | Compare: Greater Than    |

---

## 🛠 Building

```bash
mkdir build
cd build
cmake ..
make
```

---

## 🐞 Debug Mode

Enable debug logging to trace execution step-by-step:

```bash
cmake -DC2VM_DEBUG=ON ..
make
```

This will print each instruction as it's executed, along with register/memory state (if implemented).

---

## 🛆 Usage

```bash
./c2vm <bytecode_file>
```

---

## 🧪 Example

Generate test bytecode and run:

```bash
# Generate sample bytecode
python tools/gen_examples.py

# Run the sample program
./c2vm bin/examples/arithmetic.c2b
```


## 📄 License

This project is licensed under the **MIT License**.
Feel free to use, modify, and distribute as needed.
