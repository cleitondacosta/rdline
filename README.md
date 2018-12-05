# rdline
rdline is a command to read line(s) of file(s). It supports read range of
lines.

### INSTALLATION
```bash
git clone https://github.com/cleitondacosta/rdline
cd rdline
make
```
The rdline software will be at `bin/rdline`.

### SYNOPSIS
#### rdline FILE... \[-l lnum] \[-t] \[--show-name] \[--show-indice] \[--help] \[--version]

#### -l, --line lnum
```
    Read the specific line(s) of file(s). lnum is a number, but
    not a common number. You can parse one or more numbers with
    it. It can be numbers separated by comma, or a range of
    numbers separated by colon. Or both.
    Some examples:
        10 - parses the number 10
        1,2,3,4,5 - parses the numbers 1, 2, 3, 4 and 5.
        500:1000 - parses the numbers 500 to 1000 (both inclusive)
        1000:500 - parses the numbers 1000 to 500 (both inclusive)
        1,4,5,10:20,49 - parses the numbers 1, 4, 5, 10 to 20 and 49.
    Note that, a lnum must be a non-zero natural number. And 18 digits max.
``` 
#### -t, --total
```
    Show the total lines of given file(s). But wc -l is faster.
```
        
#### --show-name
```
    Show the file's name. (works with -l and -t)
```
    
#### --show-indice
```
    Show the number of line (works only with -l).
```
 
#### --version
```
    Show the version.
```

#### --help
```
    Show this message.
```
        
### RETURN VALUE
* Returns 0 upon success.
* Returns a non-zero value upon failure.
