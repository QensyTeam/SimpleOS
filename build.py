import os
import subprocess
import glob

# Пути к исходным файлам и заголовочным файлам
kernel_src_path = "kernel/kernel"
kernel_include_path = "kernel/include/kernel"
libc_include_path = "libc/include"
arch_src_path = "kernel/arch/i386"
libc_src_path = "libc"


def build_kernel():
    print("\nСборка ядра...")

    # Скомпилировать все C-файлы из директории kernel/kernel
    kernel_files = glob.glob(os.path.join(kernel_src_path, "*.c"))
    for file in kernel_files:
        subprocess.run([
            "i686-elf-gcc", "-ffreestanding", "-c", file,
            "-I", os.path.join(kernel_include_path, "sys"),
            "-I", kernel_include_path,
            "-I", libc_include_path,
            "-o", file.replace(".c", ".o")
        ])

    # Скомпилировать все C-файлы из директории kernel/arch/i386
    arch_files = glob.glob(os.path.join(arch_src_path, "**", "*.c"), recursive=True)
    for file in arch_files:
        subprocess.run([
            "i686-elf-gcc", "-ffreestanding", "-c", file,
            "-I", os.path.join(kernel_include_path, "sys"),
            "-I", kernel_include_path,
            "-I", libc_include_path,
            "-o", file.replace(".c", ".o")
        ])

    # Скомпилировать все ассемблерные файлы
    asm_files = glob.glob(os.path.join(arch_src_path, "**", "*.S"), recursive=True)
    for file in asm_files:
        subprocess.run([
            "i686-elf-as", file,
            "-o", file.replace(".S", ".o")
        ])

    # Собрать список объектных файлов
    object_files = (
        glob.glob(os.path.join(kernel_src_path, "*.o")) +
        glob.glob(os.path.join(arch_src_path, "**", "*.o"), recursive=True)
    )

    # Добавляем объектные файлы библиотеки libc
    libc_object_files = glob.glob(os.path.join(libc_src_path, "**", "*.o"), recursive=True)

    # Линковка объектных файлов в итоговое ядро
    subprocess.run([
        "i686-elf-ld", "-T", os.path.join(arch_src_path, "linker.ld")
    ] + object_files + libc_object_files + ["-o", os.path.join(arch_src_path, "kernel.bin")])


def build_libc():
    print("\nСборка libc...")

    # Скомпилировать все C-файлы из директории libc
    libc_files = glob.glob(os.path.join(libc_src_path, "**", "*.c"), recursive=True)
    for file in libc_files:
        subprocess.run([
            "i686-elf-gcc", "-ffreestanding", "-c", file,
            "-I", libc_include_path,
            "-o", file.replace(".c", ".o")
        ])

    # Создание библиотеки libc.a
    libc_object_files = glob.glob(os.path.join(libc_src_path, "**", "*.o"), recursive=True)
    subprocess.run([
        "i686-elf-ar", "rcs", "libc.a"] + libc_object_files)


if __name__ == "__main__":
    build_libc()  # Сначала строим библиотеку libc
    build_kernel()  # Затем ядро
