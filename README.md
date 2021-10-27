# Kat
![version](https://img.shields.io/badge/version-0.8.1-green?style=flat&logo=GNU)
![GitHub Workflow Status](https://img.shields.io/github/workflow/status/alphapanda01/kat/C%20CI)

> My rip-off of GNU cat

```
kat git/main
❯ kat -h
Usage: kat [OPTION]... [FILE]...
Print contents of file(s) to stdout

When no input or '-' is provided, prints from stdin
OPTIONS:
        -t              Print tabs
        -e              Print line ends
        -n              Print line numbers
        -A              similar to -e -t
        -v              Prints versiona
        -h              Prints this help

```

## Install(CMake)

### From Source
#### 1. GUI
1. Set the "Source Directory" to the current repo\'s root
2. Set the "Binary directory" to where ever you want the CMake build files to be placed(`out/`)
3. Click on "Configure" and then "Generate" (This will generate the make file)

##### OR

#### 1. CLI
1. Create a new directory(`out/`) where you want to place the cmake files
2. Create the CMake build files and Makefile
```
cmake -G "Unix Makefiles" -S REPO_ROOT_DIR -B out/  
```

#### 2. Build
1. Move into the `out/` dir
2. Build kat
```
make
```
3. Run tests (Optional)
```
ctest
```

#### 3. Install
In the same directory type
```
sudo make install
```

### From artifacts 
`Note: Why are you doing this?`
1. Download the latest artifact 
2. Unzip the archive
```
sudo ./kat-VERSION-OS.sh --exclude-subdir --prefix=/usr
```
Replace VERSION, OS and prefix(optional)

### Usage
![Example](https://img.shields.io/badge/Example-asciinema-blue?style=flat&logo=asciinema)

[![demo](https://asciinema.org/a/B7W9XaZdVzjL4NRZfktgCWl4Y.svg)](https://asciinema.org/a/B7W9XaZdVzjL4NRZfktgCWl4Y?autoplay=1)



