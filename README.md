
# README - Shell Command Executor with Pipelining and Redirection

## Project Overview

This project implements a basic shell-like command interpreter in C that allows the user to execute commands sequentially or in parallel, with support for input/output redirection, pipes, and command history. It can either take commands interactively from the user or from a script file passed as a command-line argument.

### Key Features:

- **Sequential and Parallel Execution**: Commands can be executed one after the other (sequentially) or concurrently using threads (parallel execution).
- **Input/Output Redirection**: Supports both standard redirection (`>`, `>>`) to files.
- **Pipes**: Implements pipelining (`|`) for chaining commands.
- **Command History**: Supports re-executing the last entered command via `!!`.
- **File Input**: Commands can also be read from a file provided as a command-line argument.
- **Error Handling**: Detects and handles common issues such as invalid input and fork failures.
- **Multi-threading**: For parallel execution, commands are executed in separate threads using the POSIX threads library (`pthread`).

## Project Structure

### Key Components:

- **Main Program**:
  The main function runs an interactive loop that accepts commands from the user or reads commands from a file. It determines whether commands should be executed sequentially or in parallel based on user input. It also maintains command history and handles the `exit` command.

- **Command Execution**:
  The core command execution is handled by the `executeSeq()` and `executePar()` functions:
  
  - **`executeSeq()`**: Executes commands sequentially. It can handle multiple commands separated by `;`, perform input/output redirection (`>` or `>>`), and implement basic pipe handling (`|`).
  
  - **`executePar()`**: Executes commands in parallel using threads. Each command is executed in a separate thread.

- **Command Parsing**:
  Functions like `splitStringByRedirect()` and `splitStringByPipe()` are used to break down user input into components when pipes or redirections are detected.

- **Thread Management**:
  The project uses a linked list (`struct thread_node`) to manage threads created for parallel execution. Threads are joined once all the commands have been executed.

### Files:
- **`main.c`**: The primary source file containing the implementation of the shell, command parsing, and execution logic.
- **`Makefile`**: A makefile for compiling the project.

## How to Run the Program

### Prerequisites:
- GCC compiler
- Linux or UNIX-based system (supports POSIX API for multi-threading and process management)

### Compilation:

Compile the program using the `Makefile`:

```bash
make
```

This will generate an executable file.

### Running the Program:

1. **Interactive Mode**:
   Run the program without arguments for an interactive shell:

   ```bash
   ./linux-shell
   ```

   - You can execute any shell command here (e.g., `ls`, `cat`, etc.).
   - Use `exit` to quit the program.
   - The command `!!` will re-execute the last command entered.

2. **File Input Mode**:
   You can provide a file containing shell commands as an argument:

   ```bash
   ./linux-shell commands.txt
   ```

   The program will read and execute the commands from the file sequentially or in parallel, depending on the configuration.

### Switching Execution Mode:

- You can switch between sequential and parallel execution modes during the interactive session:
  
  - To switch to **parallel** execution, type:
    ```bash
    style parallel
    ```
  
  - To switch back to **sequential** execution, type:
    ```bash
    style sequential
    ```

### Special Commands:

- **History**: The command `!!` will re-execute the last command entered.
  
- **Pipes**: Use the `|` symbol to pipe the output of one command into another (e.g., `ls | grep main`).

- **Redirection**: Redirect output to a file using `>` or append output using `>>` (e.g., `ls > output.txt`).

- **Exit**: Type `exit` to terminate the program.

### Example Commands:
- Execute a simple command:
  ```bash
  ls
  ```
- Execute commands in parallel:
  ```bash
  echo "Hello"; sleep 2; echo "World"
  ```
- Use pipes:
  ```bash
  ls | grep ".txt"
  ```
- Redirect output:
  ```bash
  echo "Hello, World!" > hello.txt
  ```
- Re-run the last command:
  ```bash
  !!
  ```

## File Input Example

You can run the shell with a file containing commands. The file could look like:

```
ls;
echo "Parallel execution!";
cat hello.txt;
```

Run it with:

```bash
./linux-shell commands.txt
```

## Known Limitations

- **Pipe Implementation**: The pipe handling is limited to a single `|` symbol between two commands. Chaining multiple commands using pipes (e.g., `cmd1 | cmd2 | cmd3`) is not yet supported.
- **Redirection**: It only handles output redirection (`>` and `>>`). Input redirection (`<`) is not supported.
- **Command Length**: There is a limit on the command length (`MAX_LINE` set to 100 characters).

## Possible Improvements

- Add support for input redirection (`<`).
- Improve the pipe functionality to support chaining multiple commands.
- Add more sophisticated error handling for various edge cases.
- Improve command parsing to handle edge cases like quoted strings or special characters.

## Authors

- Carlos Augusto Peixoto Vasconcelos FIlho


