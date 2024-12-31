import subprocess

# Путь к ISO-образу
iso_path = "SimpleOS.iso"

def run_qemu():
    print("Запуск QEMU...")
    subprocess.run([
        "qemu-system-i386", "-cdrom", iso_path
    ])

if __name__ == "__main__":
    run_qemu()
