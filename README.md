# Ressembler

## Overview
A compiler project implemented for a custom programming language I invented. 
It involves conversion of Ressembly source code to C++ and custom Binaries that can be read and executed by a custom interpreter with a GUI. 
The conversion involves Lexical Analysis, Syntax Analysis, Semantic Trees, and Semantic Evaluation & Analysis.
The UI is implemented using the Raylib framework using the C & C++ programming languages.

---
## About The Language - Ressembly
### Architecture
- A memory tape containing 16 cells that can be manipulated through operators to program complex logic.
- Through operators, we can move between memory cells, change their values, output and input values into the cells. We can also define labels to allow looping over operator functions.

### Operators
- **OPR \[signed integer\]** - When argument available -> Increments/decrements the current cell value by the integer amount.
- **MOV \[signed integer\]** - When argument available -> Moves cell pointer forwards/backwards by the integer amount.
- **VOUT** - Prints the ascii character of the current cell value
- **VIN** - Takes user input and stores its ascii value in the current cell
- **<<\[label\]>>** - Defines a label that can be used for jumps
- **JNZ \[label\]** - Conditional jump to the defined label if current cell value does not equal zero
- **JMP \[label\]** - Unconditional jump to the defined label

## The Ressembler UI
![Ressembler UI](https://github.com/user-attachments/assets/45939bf9-e8b9-428c-8393-4608e76f72ba)

## Usage
- **Pressing the C++ file button (labeled 1 above):**
    - Reads the contents of the file named `InputCode.rs` present in the root of the GUI folder.
    - Analyzes the code and converts it to its C++ equivalent.
    - Saves the converted code in a file called `compiled.cpp`.
    - Automatically compiles the generated C++ code (assuming `g++` is installed and accessible in the system path) and saves it as an executable `compiledCPP.exe`, which can be run.
    - Displays the Semantic Tree of the program in the terminal within the GUI.
    - Shows any lexical or syntax errors present in the code on the terminal.

- **Pressing the Binary file button (labeled 2 above):**
    - Reads the contents of the file named `InputCode.rs` present in the root of the GUI folder.
    - Analyzes the code and converts it to a custom binary format that allows the GUI’s interpreter to execute the commands properly.

- **After pressing the Binary file button (labeled 2 above):**
    - You can run the program one command at a time using the orange "next" button.
    - Alternatively, you can press the green play button (labeled 4 above) to run the commands automatically.
    - Control the time interval between each step of the automatic execution by clicking and sliding horizontally on the purple knob circle, which controls the execution speed.
  
- **Additional Features:**
    - The memory cells at the top display how the commands manipulate the memory.
    - The output result is shown on the console on the right.
