import os
import glob

# Пути к файлам для очистки
paths_to_clean = [
    "kernel/kernel/*.o",
    "kernel/arch/i386/*.o",
    "kernel/arch/i386/drv/*.o",
    "kernel/arch/i386/sys/*.o",
    "libc/**/*.o",
    "libc.a",
    "kernel/arch/i386/kernel.bin",
    "SimpleOS.iso",
    "iso_root/boot/kernel.bin"
]

def clean():
    print("Очистка проекта...")
    for pattern in paths_to_clean:
        for file in glob.glob(pattern, recursive=True):
            try:
                os.remove(file)
                print(f"Удалено: {file}")
            except OSError as e:
                print(f"Ошибка при удалении {file}: {e}")

if __name__ == "__main__":
    clean()
