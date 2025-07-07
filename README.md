[中文说明请见 README_CN.md](./README_CN.md)

# Winx Operating System

## Project Introduction
Winx is a teaching operating system based on the x86 architecture, designed to help learn OS principles, kernel development, and low-level programming. The project is implemented in C and Assembly, including kernel, file system, process management, device drivers, and more.

## Main Features
- Simple multitasking and thread management
- Basic file system support
- Process and memory management
- Terminal and keyboard I/O
- Common device drivers (timer, IDE, CMOS, etc.)
- System calls and user program support

## Directory Structure
```
Winx/
├── bochsrc                # Bochs config
├── disk.sh                # Virtual disk script
├── lib/                   # Header files
│   ├── device/            # Device headers
│   ├── fs/                # FS headers
│   ├── kernel/            # Kernel headers
│   ├── shell/             # Shell headers
│   ├── thread/            # Thread headers
│   ├── user/              # User headers
│   └── userprog/          # User program headers
├── src/                   # Source code
│   ├── boot/              # Boot
│   ├── device/            # Device drivers
│   ├── fs/                # File system
│   ├── impl/              # Lib implementation
│   ├── kernel/            # Kernel
│   ├── shell/             # Shell
│   ├── thread/            # Thread
│   ├── user/              # User
│   └── userprog/          # User program
├── Makefile               # Build script
```

## Build & Run
### Dependencies
- Linux
- GCC toolchain
- NASM assembler
- Bochs

### Build
```bash
make build
```

### Run (Bochs example)
```bash
make bochs
```
Or run manually:
```bash
bochs -f bochsrc
```

### Clean
```bash
make clean
```

## Naming
Winx is not Unix

## Contributing
Feel free to submit issues or pull requests. Recommended workflow:
1. Fork this repo
2. Create a new branch for your work
3. Submit a PR with a description of your changes

## License
This project is licensed under the GPLv3 License. See LICENSE for details. 