import os
import machine, psoc6


# Try to mount the filesystem, and format the flash if it doesn't exist.
# TODO: Check "Note: the flash requires the programming size to be aligned to 256 bytes." and progSize parameter below !!
bdev = psoc6.Flash()

try:
    vfs = os.VfsLfs2(bdev, progsize=256)
except:
    os.VfsLfs2.mkfs(bdev, progsize=256)
    vfs = os.VfsLfs2(bdev, progsize=256)

os.mount(vfs, "/")

print("LFS2 filesystem mounted\n")

del machine, os, psoc6, bdev, vfs
