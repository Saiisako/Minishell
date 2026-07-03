# Minishell

A simplified UNIX shell written in C, reimplementing the core behavior of `bash` from the ground up: parsing, environment handling, built-ins, pipes and redirections.

## Features

- **Parsing**: tokenizes and interprets quotes (`'`, `"`), environment variable expansion (`$VAR`, `$?`), and operator precedence, close to bash's own lexer rules.
- **Built-ins**: `cd`, `echo`, `pwd`, `export`, `unset`, `env`, `exit`, each implemented rather than shelled out to.
- **Execution**: resolves and runs external binaries via `PATH`, forking one process per command in a pipeline.
- **Pipes & redirections**: `|`, `<`, `>`, `>>`, and heredoc (`<<`) support, correctly chained across arbitrarily long pipelines.
- **Signal handling**: `Ctrl-C`, `Ctrl-D` and `Ctrl-\` behave like in an interactive bash session (prompt reset, EOF exit, ignored in child processes as appropriate).
- Exit status propagation (`$?`) matching bash semantics.

## Build & usage

```bash
make
./minishell
```

## Skills demonstrated

Recursive-descent style parsing, process orchestration (`fork`/`execve`/`waitpid`), file descriptor manipulation for pipes and redirections, POSIX signal handling, and building a robust interactive REPL.

*42 School — Common Core project, built in collaboration with [Chloé Montaigut](https://github.com/cmontaig).*
