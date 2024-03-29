# Imitation of UNIX cat&grep utilities

Implementations of Bash text utilities: cat, grep.

### Implemented cat options

| No. | Options | Description |
| ------ | ------ | ------ |
| 1 | -b (GNU: --number-nonblank) | numbers only non-empty lines |
| 2 | -e implies -v (GNU only: -E the same, but without implying -v) | but also display end-of-line characters as $  |
| 3 | -n (GNU: --number) | number all output lines |
| 4 | -s (GNU: --squeeze-blank) | squeeze multiple adjacent blank lines |
| 5 | -t implies -v (GNU: -T the same, but without implying -v) | but also display tabs as ^I  |

### Implemented grep options

| No. | Options | Description |
| ------ | ------ | ------ |
| 1 | -e | pattern |
| 2 | -i | Ignore uppercase vs. lowercase.  |
| 3 | -v | Invert match. |
| 4 | -c | Output count of matching lines only. |
| 5 | -l | Output matching files only.  |
| 6 | -n | Precede each matching line with a line number. |
| 7 | -h | Output matching lines without preceding them by file names. |
| 8 | -s | Suppress error messages about nonexistent or unreadable files. |
| 9 | -f file | Take regexes from a file. |


## Build 

Build utilities using Makefiles' commands

```
make imit_cat
```

grep imitation uses prce library for regex, so you need to download it

Use commands for Ubuntu/Debian
```
sudo apt-get update
sudo apt-get install libpcre3 libpcre3-dev
```

```
make imit_grep
```
## Run

```
 ./imit_cat [OPTIONS] [FILES]
 ./imit_grep [REGEX] [OPTIONS] [FILES]

 ```

## Test
Integration tests compare the output of system calls of cat&grep with developed ones. To run tests do command from test directory

```
 ./test-script.sh

 ```