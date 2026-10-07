# MiniShell

A custom Unix-like command-line shell implemented in C from scratch.

MiniShell is designed to demonstrate fundamental operating-system and systems-programming concepts such as process creation, signal handling, file descriptors, pipes, I/O redirection, command parsing, background processes, and file-system operations.

## Features

### Custom Commands

MiniShell implements its own versions of the following commands:

- `ls` — list directory contents
- `pwd` — display the current working directory
- `cd` — change the current directory
- `cat` — display file contents or read from standard input
- `touch` — create a file
- `mkdir` — create a directory
- `rm` — remove files
- `echo` — print text
- `clear` — clear the terminal
- `head` — display the first 10 lines
- `tail` — display the last 10 lines
- `wc` — count lines, words, and bytes
- `cp` — copy a file
- `mv` — move/rename a file
- `history` — display previously entered commands
- `exit` — terminate MiniShell

MiniShell does not use `execvp()` as a fallback for unsupported commands. Unsupported commands are reported as not implemented.

## Shell Features

### Command Parsing

MiniShell parses user input into command arguments using whitespace-based tokenization.

### Input and Output Redirection

Supported operators:

```text
>
>>
<
```

Examples:

```bash
echo hello > output.txt
echo second >> output.txt
cat < output.txt
```

### Pipelines

Multiple commands can be connected using `|`.

Examples:

```bash
echo hello | cat
cat file.txt | head
cat file.txt | head | wc
```

MiniShell creates the required pipes and processes to connect command input and output.

### Background Processes

Commands can be executed in the background using `&`.

Example:

```bash
pwd &
echo hello &
```

MiniShell does not wait for these processes to finish immediately.

### Signal Handling

MiniShell handles important terminal signals including:

- `SIGINT` (`Ctrl+C`)
- `SIGTSTP` (`Ctrl+Z`)
- `SIGCHLD`

Foreground commands that require their own process are executed separately so that signals such as `Ctrl+C` do not terminate the shell itself.

### Command History

MiniShell maintains an in-memory history of commands entered during the current session.

Example:

```text
minishell> pwd
minishell> echo hello
minishell> history

1  pwd
2  echo hello
3  history
```

The history is session-based and is not currently persisted to disk.

## Project Structure

```text
MiniShell/
│
├── include/
│   ├── background.h
│   ├── cat.h
│   ├── cd.h
│   ├── clear.h
│   ├── cp.h
│   ├── echo.h
│   ├── exit.h
│   ├── foreground.h
│   ├── head.h
│   ├── history.h
│   ├── ls.h
│   ├── mkdir.h
│   ├── mv.h
│   ├── parser.h
│   ├── pipeline.h
│   ├── pwd.h
│   ├── redirection.h
│   ├── rm.h
│   ├── signals.h
│   ├── tail.h
│   ├── touch.h
│   └── wc.h
│
├── src/
│   ├── main.c
│   ├── background.c
│   ├── cat.c
│   ├── cd.c
│   ├── clear.c
│   ├── cp.c
│   ├── echo.c
│   ├── exit.c
│   ├── foreground.c
│   ├── head.c
│   ├── history.c
│   ├── ls.c
│   ├── mkdir.c
│   ├── mv.c
│   ├── parser.c
│   ├── pipeline.c
│   ├── pwd.c
│   ├── redirection.c
│   ├── rm.c
│   ├── signals.c
│   ├── tail.c
│   ├── touch.c
│   └── wc.c
│
├── tests/
├── Makefile
├── README.md
├── LICENSE
└── .gitignore
```

## Architecture

The shell follows a modular architecture.

```text
                    ┌──────────────┐
                    │   main.c     │
                    │ Shell Loop   │
                    └──────┬───────┘
                           │
                           ▼
                    ┌──────────────┐
                    │   parser.c   │
                    │ Input Parser │
                    └──────┬───────┘
                           │
             ┌─────────────┼─────────────┐
             │             │             │
             ▼             ▼             ▼
        Background     Pipeline     Redirection
        Processes      Engine       Handler
             │             │             │
             └─────────────┼─────────────┘
                           │
                           ▼
                  Custom Commands
                           │
       ┌──────┬──────┬────┼────┬──────┬──────┐
       ▼      ▼      ▼    ▼    ▼      ▼      ▼
      ls     cat    pwd  cd   echo    wc    ...
```

Each command has its own implementation and header file, making the project easier to maintain and extend.

## Technologies

- C
- Linux / Unix system calls
- GCC
- Make
- Git
- WSL / Ubuntu for development

## Important System Calls and APIs

MiniShell uses several Linux/POSIX APIs to implement its functionality:

- `fork()` — create child processes
- `waitpid()` — wait for child processes
- `pipe()` — create communication channels between processes
- `dup()` / `dup2()` — manage file descriptors
- `open()` — open and create files
- `read()` / `write()` — perform file I/O
- `close()` — close file descriptors
- `chdir()` — change the working directory
- `getcwd()` — obtain the current working directory
- `opendir()` / `readdir()` / `closedir()` — directory operations
- `mkdir()` — create directories
- `unlink()` — remove files
- `rename()` — move or rename files
- `signal()` — configure signal handling

## Building the Project

### Prerequisites

A Linux environment with:

- GCC
- GNU Make

For Windows development, WSL with Ubuntu can be used.

### Build

Clone the repository and enter the project directory:

```bash
cd MiniShell
```

Build using:

```bash
make
```

This creates the `minishell` executable.

### Run

```bash
./minishell
```

Or build and run directly with:

```bash
make run
```

### Clean

Remove the compiled executable:

```bash
make clean
```

## Example Session

```text
minishell> pwd
/home/user/MiniShell

minishell> echo Hello MiniShell
Hello MiniShell

minishell> echo hello | cat
hello

minishell> cat test.txt | head | wc
3 6 29

minishell> history
1  pwd
2  echo Hello MiniShell
3  echo hello | cat
4  cat test.txt | head | wc
5  history

minishell> exit
```

## Design Goals

The project was designed with the following goals:

1. Implement shell functionality instead of relying on external command execution.
2. Understand Linux processes and system calls.
3. Understand how pipes and file descriptors enable inter-process communication.
4. Implement input/output redirection.
5. Handle background processes.
6. Practice signal handling.
7. Maintain a clean and modular C architecture.
8. Build a project that demonstrates practical systems-programming knowledge.

## Current Limitations

MiniShell is an educational Unix-like shell and does not attempt to reproduce the complete behavior of Bash.

Current limitations include:

- Command parsing is whitespace-based.
- Full shell quoting and escaping are not implemented.
- Shell variables and environment expansion are not implemented.
- Wildcard expansion is not implemented.
- Advanced job control and process groups are not implemented.
- Background job management commands are not implemented.
- `rm` currently handles files rather than recursive directory deletion.
- Command history is stored only in memory.
- The shell supports a focused set of custom commands rather than every Unix command.
- Complex shell syntax is intentionally outside the project's scope.

## Learning Outcomes

Through MiniShell, the project demonstrates practical understanding of:

- Processes and process creation
- Parent and child processes
- File descriptors
- Standard input/output
- Inter-process communication
- Pipes
- I/O redirection
- Signals
- File-system operations
- Dynamic memory management
- Modular C programming
- Make-based compilation
- Git-based project development

## License

This project is licensed under the terms of the license included in this repository.