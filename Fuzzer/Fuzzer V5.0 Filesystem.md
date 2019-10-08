# Fuzzer 5.0 Filesystem 버전 업그레이드

지금 서버에 올라와있는 ext4-combined image를 마운트 시키면 아래와 같이 Btrfs Loaded라고 뜬다.<br>

![error](https://github.com/Kimdong0219/image/blob/master/error.png?raw=true)
<br>


----
## 수정 시작


Hydra 5.0 ext4 Filesystem을 구동시켜 보았다.<br>
```c
./run.py -t ext4 -c 1 -l 11 -g 1
```
<br>

![hydra](https://github.com/Kimdong0219/image/blob/master/hydra.png?raw=true)<br>


기존의 LKL V5.0과 Hydra에서 사용하는 LKL V 5.0을 비교해보았다. <br>

![diff](https://github.com/Kimdong0219/image/blob/master/diff.png?raw=true)
<br>
50가지 정도의 다른 파일들이 발견되었다.<br>

현재 서버에 올라와져 있는 LKL과 `hydra LKL`도 비교해보았다.<br>

![diff4](https://github.com/Kimdong0219/image/blob/master/diff4.png?raw=true)
<br>

```c
<서버 LKL>
#progs-$(LKL_HOST_CONFIG_FUSE) += lklfuse
#LDLIBS_lklfuse-y := -lfuse

<Hydra LKL>
progs-$(LKL_HOST_CONFIG_FUSE) += lklfuse
LDLIBS_lklfuse-y := -lfuse

<서버 LKL>
progs-y += fsfuzz
LDLIBS_fsfuzz-y := -larchive
LDLIBS_fsfuzz-$(LKL_HOST_CONFIG_NEEDS_LARGP) += -largp

<Hydra LKL>
# progs-y += fsfuzz
# LDLIBS_fsfuzz-y := -larchive
# LDLIBS_fsfuzz-$(LKL_HOST_CONFIG_NEEDS_LARGP) += -largp

<서버 LKL>
---

<Hydra LKL>
progs-y += combined-consistency
LDLIBS_combined-consistency-y := -larchive -lz
LDLIBS_combined-consistency-$(LKL_HOST_CONFIG_NEEDS_LARGP) += -largp

<서버 LKL>
$progs-$(LKL_HOST_CONFIG_ARCHIVE) += cptofs
$LDLIBS_cptofs-y := -larchive
$LDLIBS_cptofs-$(LKL_HOST_CONFIG_NEEDS_LARGP) += -largp

<Hydra LKL>
# progs-$(LKL_HOST_CONFIG_ARCHIVE) += cptofs
# LDLIBS_cptofs-y := -larchive
# LDLIBS_cptofs-$(LKL_HOST_CONFIG_NEEDS_LARGP) += -largp

```
<br>

서버의 ext4를 compile을 해보았다.<br>

```c
sudo ./compile -t ext4 -c
make: Entering directory '/home/fuzzer/janus/lkl_ext4/tools/lkl'
  CLEAN    objects
  CLEAN    headers
  CLEAN    liblkl.a
  CLEAN    targets
make: Leaving directory '/home/fuzzer/janus/lkl_ext4/tools/lkl'
cp -f ext4-config arch/lkl/defconfig
make -C tools/lkl  CC="/home/fuzzer/janus/lkl_ext4/../ff-gcc/ff-gcc fs/ext4"
make: Entering directory '/home/fuzzer/janus/lkl_ext4/tools/lkl'
scripts/kconfig/conf  --defconfig Kconfig

```
<br>

ext4를 compile 했으나 `btrfs의 object파일`이 들어가는 것을 볼수있다.<br>


```c
  CC      fs/btrfs/super.o
  CC      fs/btrfs/ctree.o
  CC      fs/btrfs/extent-tree.o
  CC      fs/btrfs/print-tree.o
  CC      fs/btrfs/root-tree.o
  CC      fs/btrfs/dir-item.o
  CC      fs/btrfs/file-item.o
  CC      fs/btrfs/inode-item.o
  CC      fs/btrfs/inode-map.o
  CC      fs/btrfs/disk-io.o
  CC      fs/btrfs/transaction.o
  CC      fs/btrfs/inode.o
  CC      fs/btrfs/file.o
  CC      fs/btrfs/tree-defrag.o
  CC      fs/btrfs/extent_map.o
  CC      fs/btrfs/sysfs.o
  CC      fs/btrfs/struct-funcs.o
  CC      fs/btrfs/xattr.o
  CC      fs/btrfs/ordered-data.o
  CC      fs/btrfs/extent_io.o
  CC      fs/btrfs/volumes.o
  CC      fs/btrfs/async-thread.o
  CC      fs/btrfs/ioctl.o
  CC      fs/btrfs/locking.o
  CC      fs/btrfs/orphan.o
  CC      fs/btrfs/export.o
```
<br>

이제 어느 부분에서 btrfs가 잘못들어가는지 확인해보자.<br>

일단 예상되는 부분을 작성해 보았다.<br>

* `ext4-config`<br>

* `compile` 코드 또는 `Makefile` 코드<br>

* `LKL V 5.0`으로 Porting하다가 오류 발생<br>


```c
<btrfs>

fuzzer@fuzzer-PowerEdge-R720:~/janus/lklJo/tools/lkl/include/lkl$ pwd

/home/fuzzer/janus/lklJo/tools/lkl/include/lkl/autoconf.h

#define LKL_CONFIG_BTRFS_FS 1
#define LKL_CONFIG_BTRFS_FS 1
#define LKL_CONFIG_RAID6_PQ 1
#define LKL_CONFIG_RAID6_PQ_BENCHMARK 1

<ext4>
#define LKL_CONFIG_BTRFS_FS_POSIX_ACL 1
#define LKL_CONFIG_BTRFS_FS 1
#define LKL_CONFIG_RAID6_PQ 1
#define LKL_CONFIG_RAID6_PQ_BENCHMARK 1


```
<br>

예상에는 hydra에서 btrfs로 compile한후의 `lkl source`가 그대로 남아 있는것 같다.<br>

`janus/lkl/tools/lkl/include/lkl` 경로의 `autoconf.h`등 여러 헤더파일들이 문제가 된것 같다.<br>


따라서, hydra에서 ext4로 compile한후 다시 수정해야 할것 같다.<br>

----

## ext4 compile

ext4로 compile한후에 diff를 확인해보고 수정을 해보았다.<br>

최종적으로 `18 file` 외에 전부 수정을 하였다.<br>

![enter image description here](https://github.com/Kimdong0219/image/blob/master/ext4_change_diff.png?raw=true)
<br>

현재에는 btrfs가 아닌 `fs/ext4` 가 들어가긴 하는데 몇가지 권한 문제가 발생했다.<br>


```c
sh: 1: ./scripts/gcc-version.sh: Permission denied
sh: 1: ./scripts/gcc-version.sh: Permission denied

sh: 1: ./scripts/clang-version.sh: Permission denied
sh: 1: ./scripts/gcc-plugin.sh: Permission denied

make[1]: execvp: ./arch/lkl/scripts/headers_install.py

clang: warning: optimization flag '-fmerge-constants' is not supported

```
<br>

권한을 주도록 하자. <br>


수정하고 `ext4-combined`를 돌려보았다.<br>


```c

fuzzer@fuzzer-PowerEdge-R720:~/janus/lkl_ext4/tools/lkl$ sudo ./ext4-combined -t ext4 -i ../../../tmp.img -p ../../../tmpJo.c.raw -v


[    0.000000] Linux version 5.0.0 (fuzzer@fuzzer-PowerEdge-R720) (gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.11)) #2 Tue Oct 8 15:52:51 KST 2019
[    0.000000] memblock address range: 0x7f9a18000000 - 0x7f9a1ffff000
[    0.000000] Built 1 zonelists, mobility grouping on.  Total pages: 32319
[    0.000000] Kernel command line: mem=128M virtio_mmio.device=316@0x1000000:1
[    0.000000] Dentry cache hash table entries: 16384 (order: 5, 131072 bytes)
[    0.000000] Inode-cache hash table entries: 8192 (order: 4, 65536 bytes)
[    0.000000] Memory available: 129044k/131068k RAM
[    0.000000] SLUB: HWalign=32, Order=0-3, MinObjects=0, CPUs=1, Nodes=1
[    0.000000] NR_IRQS: 4096
[    0.000000] lkl: irqs initialized
[    0.000000] clocksource: lkl: mask: 0xffffffffffffffff max_cycles: 0x1cd42e4dffb, max_idle_ns: 881590591483 ns
[    0.000000] lkl: time and timers initialized (irq2)
[    0.000004] pid_max: default: 4096 minimum: 301
[    0.000043] Mount-cache hash table entries: 512 (order: 0, 4096 bytes)
[    0.000047] Mountpoint-cache hash table entries: 512 (order: 0, 4096 bytes)
[    0.001797] printk: console [lkl_console0] enabled
[    0.001812] clocksource: jiffies: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 19112604462750000 ns
[    0.002762] clocksource: Switched to clocksource lkl
[    0.002910] virtio-mmio: Registering device virtio-mmio.0 at 0x1000000-0x100013b, IRQ 1.
[    0.003144] workingset: timestamp_bits=62 max_order=15 bucket_order=0
[    0.006764] virtio-mmio virtio-mmio.0: Failed to enable 64-bit or 32-bit DMA.  Trying to continue, but this might not work.
[    0.006901] virtio_blk virtio0: [vda] 262144 512-byte logical blocks (134 MB/128 MiB)
[    0.007239] random: get_random_bytes called from .LC87+0xd/0x18 with crng_init=0
[    0.007434] Warning: unable to open an initial console.
[    0.007454] This architecture does not have kernel memory protection.
[    0.007459] Run /init as init process
[    0.007839] EXT4-fs (vda): VFS: Can't find ext4 filesystem
can't mount disk: Invalid argument
[    0.007895] reboot: Restarting system

```

compile시 EXT4가 경로에서 들어갔으나 VFS에서 Filesystem을 읽어오지 못한다.<br>

좀더 수정을 해야 할것 같다.<br>

밥을 먹고 와서 기존에 janus에서 제공해준 image로 다시 테스트를 하니까 Filesystem이
인식되었다.<br>

```c
fuzzer@fuzzer-PowerEdge-R720:~/janus$ sudo ./core/afl-image-syscall/afl-fuzz -b
 ext4 -s fs/ext4/ext4_wrapper.so -e ./samples/evaluation/ext4-00.image
 -S ext4 -y prog -i input_test -o output_test -m none -u 3 --
 ./lkl_ext4/tools/lkl/ext4-combined -t ext4 -p @@
```
<br>

![enter image description here](https://github.com/Kimdong0219/image/blob/master/ext4_fuzzer.png?raw=true)
<br>

**일단은 EXT4 Fuzzer 구동에 성공하였다.**<br>

테스트로 10분정도 돌린결과 crash가 3개정도 발견되었다.<br>


```c
root@fuzzer-PowerEdge-R720:~/janus/output_ext4_k/ext4/crashes# ls
id:000000,sig:11,src:000349,op:havoc,rep:128
id:000001,sig:11,src:000387,op:fs-havoc-mutate,rep:2
id:000002,sig:11,src:000460,op:havoc,rep:128
README.txt
```
<br>

조금 더 손을 본뒤에 EXT4 Fuzzer 구동을 시키면 될것 같다.<br>
