# 🔒 Secure Kernel Module Signing & Verification on Raspberry Pi  
**Building and Signing LKMs for Embedded Security**  
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)  


---

## 📜 Abstract  
This project demonstrates cryptographic signing/verification of Linux Kernel Modules (LKMs) to ensure code authenticity on embedded systems like the Raspberry Pi. By enforcing signature checks during module loading, unauthorized code execution is prevented. Includes cross-compilation workflows, kernel configuration, and real-world implementation challenges.

---

## 🎯 Project Goals  
1. Generate cryptographic keys/certificates using OpenSSL.  
2. Configure Linux kernel to enforce module signature verification.  
3. Cross-compile signed kernels/modules for Raspberry Pi (ARM).  
4. Validate security by comparing signed/unsigned module behavior.  

---

## 🛠️ Implementation Steps  

1. Key & Certificate Generation  
```bash
# Generate RSA key pair (PEM format)
openssl req -new -newkey rsa:2048 -days 3650 -nodes -x509 \
  -keyout private_key.pem -out public_key.pem

# Convert public key to DER format
openssl x509 -outform der -in public_key.pem -out public_key.der

# Create combined certificate
cat private_key.pem certificate.pem > combined.pem
```

### 2. Kernel Configuration
```bash
Edit .config or use make menuconfig:

CONFIG_MODULE_SIG=y                 # Enable module signing
CONFIG_MODULE_SIG_FORCE=y           # Enforce signed modules
CONFIG_MODULE_SIG_KEY="certs/combined.pem"  # Certificate path
```

### 3. Cross-Compilation for Raspberry Pi
```bash
# Set up cross-compilation tools
sudo apt-get install gcc-arm-linux-gnueabihf

# Build kernel and modules
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- bcmrpi_defconfig
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j$(nproc) zImage modules dtbs
```

### 4. Transfer Files to Raspberry Pi
```bash

# Kernel image
scp arch/arm/boot/zImage pi@raspberrypi:/boot/kernel7.img

# Device Tree Blobs
scp arch/arm/boot/dts/*.dtb pi@raspberrypi:/boot/

# Signed modules
scp -r modules/lib/modules/$(make kernelversion) pi@raspberrypi:/lib/modules/
```

## 🔍 Testing Workflow
Load Signed Module
```bash
sudo insmod TestDriver.ko    # Insert module
dmesg | tail -n 5           # Verify logs: "Hello, World!"
lsmod | grep TestDriver      # Confirm module loaded
```

Unload Module
```bash
Copy
sudo rmmod TestDriver        # Remove module
dmesg | tail -n 5           # Verify logs: "Goodbye, World!"
```

Attempt Unsigned Module Load (Security Check)
```bash
sudo insmod unsigned.ko      # Expected error:
# [  12.456] module: unsigned: loading of unsigned module is rejected
```

## 🧩 Challenges Faced
Issue	Resolution
```
8-hour kernel build time	Used -j$(nproc) for parallel compilation
Emulator failures	Switched to physical Raspberry Pi hardware
Bootloader misconfiguration	Edited /boot/config.txt manually
Signature verification overhead	Optimized SHA256 hashing
```

## 📂 File Structure
```
.
├── certs/                   # Keys & certificates
│   ├── private_key.pem
│   ├── public_key.pem
│   └── combined.pem
├── modules/                 # LKM source code
│   ├── TestDriver.c
│   └── Makefile
    └── signature.bin

```

## 🔮 Future Enhancements
```
TPM Integration: Hardware-backed key storage

CI/CD Pipeline: Automated cross-compilation

Kernel Live Patching: Secure runtime updates

Performance Metrics: Quantify verification overhead
```

## 📝 License
MIT License - See LICENSE for details.
