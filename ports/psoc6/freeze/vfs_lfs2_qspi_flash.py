import os
import machine, psoc6


# Try to mount the filesystem, and format the flash if it doesn't exist.
# TODO: Check "Note: the flash requires the programming size to be aligned to 256 bytes." and progSize parameter below !!
bdev = psoc6.QSPI_Flash()
block_size = 0x100

try:
    vfs = os.VfsLfs2(bdev, progsize=block_size, readsize=block_size)
    os.mount(vfs, "/qspi_flash")
except:
    os.VfsLfs2.mkfs(bdev, progsize=block_size, readsize=block_size)
    vfs = os.VfsLfs2(bdev, progsize=block_size, readsize=block_size)
    os.mount(vfs, "/qspi_flash")

print("LFS2 filesystem mounted at /qspi_flash\n")

del machine, os, psoc6, bdev, vfs, block_size
