#!/usr/bin/env python3
import struct
IMG = "/home/user/ZeroOS/disk.img"
SECTOR_SIZE = 512
SIZE_MB = 10
SECTORS = SIZE_MB * 1024 * 1024 // SECTOR_SIZE

boot = bytearray(512)
boot[0:3] = b'\xEB\x58\x90'
boot[3:11] = b'ZEROOS  '
struct.pack_into('<H', boot, 11, SECTOR_SIZE)
boot[13] = 8
struct.pack_into('<H', boot, 14, 32)
boot[16] = 2
struct.pack_into('<H', boot, 22, 0)
struct.pack_into('<H', boot, 24, 32)
struct.pack_into('<H', boot, 26, 64)
struct.pack_into('<I', boot, 28, 0)
struct.pack_into('<I', boot, 32, SECTORS)
struct.pack_into('<I', boot, 36, 128)
struct.pack_into('<I', boot, 44, 2)
struct.pack_into('<H', boot, 48, 1)
struct.pack_into('<H', boot, 50, 6)
boot[64] = 0x80
boot[66] = 0x29
struct.pack_into('<I', boot, 67, 0x12345678)
boot[71:82] = b'ZEROOS     '
boot[82:90] = b'FAT32   '
boot[510] = 0x55
boot[511] = 0xAA

fsinfo = bytearray(512)
struct.pack_into('<I', fsinfo, 0, 0x41615252)
struct.pack_into('<I', fsinfo, 484, 0x61417272)
struct.pack_into('<I', fsinfo, 488, 0)
struct.pack_into('<I', fsinfo, 492, 3)
fsinfo[510]=0x55
fsinfo[511]=0xAA

def make_fat():
    fat = bytearray(128*512)
    struct.pack_into('<I', fat, 0, 0x0FFFFFF8)
    struct.pack_into('<I', fat, 4, 0x0FFFFFFF)
    struct.pack_into('<I', fat, 8, 0x0FFFFFFF)
    struct.pack_into('<I', fat, 12, 4)
    struct.pack_into('<I', fat, 16, 0x0FFFFFFF)
    return fat
fat = make_fat()

def create_dir_entry(name, ext, attr, cluster, size):
    entry = bytearray(32)
    name = name.upper().ljust(8)[:8]
    ext = ext.upper().ljust(3)[:3]
    entry[0:8] = name.encode()
    entry[8:11] = ext.encode()
    entry[11] = attr
    entry[26:28] = struct.pack('<H', cluster & 0xFFFF)
    entry[20:22] = struct.pack('<H', (cluster >> 16) & 0xFFFF)
    struct.pack_into('<I', entry, 28, size)
    return entry

root_dir = bytearray(4096)
entry1 = create_dir_entry("README", "TXT", 0x20, 3, 95)
entry2 = create_dir_entry("ZERO", "TXT", 0x20, 4, 68)
entry3 = create_dir_entry("DOCS", "", 0x10, 0, 0)
root_dir[0:32]=entry1
root_dir[32:64]=entry2
root_dir[64:96]=entry3

cluster3 = bytearray(4096)
cluster3[0:95]=b"Welcome to Zero OS v6.0 FAT32 Real Disk!\nZero Bloat. Zero Tracking.\n\nThis file is on real FAT32 disk image disk.img\n"
cluster4 = bytearray(4096)
cluster4[0:68]=b"Zero OS v6.0 64K kernel, real drivers, USB-HID, FAT32, NET, SMP, AI\n"

with open(IMG, 'r+b') as f:
    f.seek(0)
    f.write(boot)
    f.seek(512)
    f.write(fsinfo)
    f.seek(512*6)
    f.write(boot)
    f.seek(512*32)
    f.write(fat)
    f.seek(512*(32+128))
    f.write(fat)
    f.seek(512*288)
    f.write(root_dir)
    f.seek(512*296)
    f.write(cluster3)
    f.seek(512*304)
    f.write(cluster4)

print(f"FAT32 disk image created at {IMG} 10MB with 2 files at clusters 3,4")
