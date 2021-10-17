# tinysh

A tiny shell made by D1N0

## Get Started

1. git clone or download all sources in tinysh directory
2. type `make` on your terminal
3. execute shell by `./tinysh`
4. type `exit` or press `Ctrl-D` to exit shell

## Commands

### cd

`Usage: cd DIRECTORY`

change the current working directory to DIRECTORY

### ls

`Usage: ls [OPTION]... [FILE]...`

List information about the FILEs (the current directory by default).

options:
* `-a` : do not ignore entries starting with .
* `-l` : use a long listing format
* `--help` : display this help and exit

### mv

`Usage: mv SOURCE DEST`

`or: mv SOURCE DIRECTORY`

Rename SOURCE to DEST, or move SOURCE(s) to DIRECTORY.

### exit

`Usage: exit`

Exit the shell

### help

`Usage: help`

Print help message
