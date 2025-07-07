# Winx 操作系统

## 项目简介
Winx 是一个基于 x86 架构的教学操作系统，旨在帮助学习操作系统原理、内核开发与底层编程。项目采用 C 语言和汇编实现，包含了内核、文件系统、进程管理、设备驱动等模块。

## 主要特性
- 简单的多任务调度与线程管理
- 基本的文件系统支持
- 进程与内存管理
- 终端与键盘输入输出
- 常用设备驱动（时钟、IDE、CMOS等）
- 系统调用与用户程序支持

## 目录结构
```
Winx/
├── bochsrc                # Bochs 配置文件
├── disk.sh                # 虚拟磁盘脚本
├── lib/                   # 头文件目录
│   ├── device/            # 设备相关头文件
│   ├── fs/                # 文件系统相关头文件
│   ├── kernel/            # 内核相关头文件
│   ├── shell/             # shell 相关头文件
│   ├── thread/            # 线程相关头文件
│   ├── user/              # 用户态头文件
│   └── userprog/          # 用户程序相关头文件
├── src/                   # 源码目录
│   ├── boot/              # 启动相关
│   ├── device/            # 设备驱动实现
│   ├── fs/                # 文件系统实现
│   ├── impl/              # 基础库实现
│   ├── kernel/            # 内核实现
│   ├── shell/             # shell 实现
│   ├── thread/            # 线程实现
│   ├── user/              # 用户态实现
│   └── userprog/          # 用户程序实现
├── Makefile               # 构建脚本
```

## 编译与运行
### 依赖环境
- Linux 环境
- GCC 工具链
- NASM 汇编器
- Bochs

### 编译
```bash
make build
```

### 运行（以 Bochs 为例）
```bash
make bochs
```
或手动运行：
```bash
bochs -f bochsrc
```

### 清理
```bash
make clean
```

## 贡献方式
欢迎提交 issue 或 pull request 参与开发。建议遵循如下流程：
1. Fork 本仓库
2. 新建分支进行开发
3. 提交 PR 并描述修改内容

## 许可证
本项目采用 MIT 许可证，详见 LICENSE 文件。 