import os
import machine, psoc6


# Try to mount the filesystem, and format the flash if it doesn't exist.
# TODO: Check "Note: the flash requires the programming size to be aligned to 256 bytes." and progSize parameter below !!
bdev = psoc6.QSPI_Flash()
read_write_size = 0x20 # multiples of this minimum number will be read and written to in the fs.

try:
    vfs = os.VfsLfs2(bdev, progsize=read_write_size, readsize=read_write_size)
    os.mount(vfs, "/qspi_flash")
except:
    os.VfsLfs2.mkfs(bdev, progsize=read_write_size, readsize=read_write_size)
    vfs = os.VfsLfs2(bdev, progsize=read_write_size, readsize=read_write_size)
    os.mount(vfs, "/qspi_flash")

print("LFS2 filesystem mounted at /qspi_flash\n")

del machine, os, psoc6, bdev, vfs, read_write_size
