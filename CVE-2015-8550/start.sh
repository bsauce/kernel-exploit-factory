#!/bin/sh
qemu-system-x86_64 \
   -m 256M \
   -kernel bzImage \
   -initrd rootfs.cpio \
   -append "loglevel=3 console=ttyS0 oops=panic panic=1 kaslr" \
   -nographic -net user -net nic \
   -device e1000 \
   -smp cores=2,threads=2 -cpu kvm64,+smep \
   -monitor /dev/null 2>/dev/null \
   -s
