import os
import subprocess

# Пути для создания ISO
iso_output_path = "SimpleOS.iso"
kernel_binary_path = "kernel/arch/i386/kernel.bin"
iso_root_path = "iso_root"
grub_cfg_path = os.path.join(iso_root_path, "boot/grub/grub.cfg")
eltorito_img_path = "eltorito.img"  # Путь для eltorito.img

def create_eltorito_img():
    print("Создание eltorito.img...")

    # Убедиться, что папка iso_root/boot/grub существует
    os.makedirs(os.path.join(iso_root_path, "boot/grub"), exist_ok=True)

    # Создание загрузочного образа eltorito.img
    try:
        command = [
            "xorriso",
            "-as", "mkisofs",
            "-o", eltorito_img_path,  # Путь для eltorito.img
            "-b", "boot/grub/eltorito.img",  # Путь к файлу boot/grub/eltorito.img внутри ISO
            "-no-emul-boot",
            "-boot-load-size", "4",
            "-boot-info-table",
            iso_root_path  # Указываем директорию, где будет размещена структура для создания образа
        ]
        subprocess.run(command, check=True)
        print(f"Файл eltorito.img создан: {eltorito_img_path}")
    except subprocess.CalledProcessError as e:
        print(f"Ошибка при создании eltorito.img: {e}")
        return False
    return True

def create_iso():
    print("Создание ISO...")

    # Убедиться, что папка iso_root/boot/grub существует
    os.makedirs(os.path.join(iso_root_path, "boot/grub"), exist_ok=True)

    # Скопировать ядро в папку ISO
    kernel_dest_path = os.path.join(iso_root_path, "boot/kernel.bin")
    if os.path.exists(kernel_binary_path):
        os.replace(kernel_binary_path, kernel_dest_path)
    else:
        print(f"Ошибка: Файл ядра не найден: {kernel_binary_path}")
        return

    # Создать grub.cfg
    with open(grub_cfg_path, "w") as grub_cfg:
        grub_cfg.write("""
set timeout=0
set default=0

menuentry "SimpleOS" {
    multiboot /boot/kernel.bin
    boot
}
""")

    # Теперь создаем ISO-образ
    try:
        command = [
            "xorriso",
            "-as", "mkisofs",
            "-o", iso_output_path,
            "-b", "boot/grub/eltorito.img",  # Путь к eltorito.img
            "-no-emul-boot",
            "-boot-load-size", "4",
            "-boot-info-table",
            iso_root_path  # Путь к директории iso_root для ISO
        ]
        subprocess.run(command, check=True)
        print(f"ISO образ создан: {iso_output_path}")
    except subprocess.CalledProcessError as e:
        print(f"Ошибка при создании ISO: {e}")

if __name__ == "__main__":
    # Сначала создаем eltorito.img, затем создаем ISO
    if create_eltorito_img():
        create_iso()
