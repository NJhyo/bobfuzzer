# squashfs


zstd(introduced in 4.4)

used in
 - btrFS
 - squashfs
 - ReiserFS
 https://facebook.github.io/zstd/



 ### mount
```cpp
# mksqushfs ./dir dir.sqsh
# mkdir /mnt/dir
# mount dir.sqush /mnt/dir -t squashfs -o loop

```

### how to create a type file
```cpp
ln src dst// hard link
ln -s src dst//symbolic link
mkfifo fifo

```
./istat -i ../samples/evaluation/btrfs-00.image -t btrfs -o btrfs.istat

##### istat
```
$ ./istat -i test2.sqsh -t squashfs -o squashfs2.istat
tmp image: /tmp/tmpHWdQg6
mount dir: /tmp/tmpR5ecbW
['sudo', 'mount', '-o', 'loop', '-t', 'squashfs', '/tmp/tmpHWdQg6', '/tmp/tmpR5ecbW']
[sudo] password for grapefruit:
chown: changing ownership of '/tmp/tmpR5ecbW/dir/a.c': Read-only file system
chown: changing ownership of '/tmp/tmpR5ecbW/dir/fifo2': Read-only file system
chown: changing ownership of '/tmp/tmpR5ecbW/dir/hard': Read-only file system
chown: changing ownership of '/tmp/tmpR5ecbW/dir/subdir': Read-only file system
chown: changing ownership of '/tmp/tmpR5ecbW/dir/sym': Read-only file system
chown: changing ownership of '/tmp/tmpR5ecbW/dir': Read-only file system
chown: changing ownership of '/tmp/tmpR5ecbW/fifo': Read-only file system
chown: changing ownership of '/tmp/tmpR5ecbW/general': Read-only file system
chown: changing ownership of '/tmp/tmpR5ecbW/hardlink': Read-only file system
chown: changing ownership of '/tmp/tmpR5ecbW/regular': Read-only file system
chown: changing ownership of '/tmp/tmpR5ecbW/symlink': Read-only file system
chown: changing ownership of '/tmp/tmpR5ecbW': Read-only file system
/tmp/tmpR5ecbW
.
/tmp/tmpR5ecbW, True
/tmp/tmpR5ecbW/./fifo, True
/tmp/tmpR5ecbW/./general, True
/tmp/tmpR5ecbW/./hardlink, True
/tmp/tmpR5ecbW/./regular, True
/tmp/tmpR5ecbW/./symlink, True
dir
/tmp/tmpR5ecbW/dir, True
/tmp/tmpR5ecbW/dir/a.c, True
/tmp/tmpR5ecbW/dir/fifo2, True
/tmp/tmpR5ecbW/dir/hard, True
/tmp/tmpR5ecbW/dir/sym, True
dir/subdir
/tmp/tmpR5ecbW/dir/subdir, True
[['.', 2, []], ['./fifo', 4, []], ['./general', 1, []], ['./hardlink', 1, []], ['./regular', 1, []], ['./symlink', 3, []], ['dir', 2, []], ['dir/a.c', 1, []], ['dir/fifo2', 4, []], ['dir/hard', 1, []], ['dir/sym', 3, []], ['dir/subdir', 2, []]]



prob -> chown,
```

```
$ ./istat -i ../samples/evaluation/btrfs-00.image -t btrfs -o btrfs2.istat
tmp image: /tmp/tmp5tVRiN
mount dir: /tmp/tmpdyHM_u
['sudo', 'mount', '-o', 'loop', '-t', 'btrfs', '/tmp/tmp5tVRiN', '/tmp/tmpdyHM_u']
/tmp/tmpdyHM_u
.
/tmp/tmpdyHM_u, True
foo
/tmp/tmpdyHM_u/foo, True
foo/bar
/tmp/tmpdyHM_u/foo/bar, True
/tmp/tmpdyHM_u/foo/bar/baz, True
/tmp/tmpdyHM_u/foo/bar/xattr, True
/tmp/tmpdyHM_u/foo/bar/acl, True
/tmp/tmpdyHM_u/foo/bar/æøå, True
/tmp/tmpdyHM_u/foo/bar/fifo, True
/tmp/tmpdyHM_u/foo/bar/hln, True
/tmp/tmpdyHM_u/foo/bar/sln, False
[['.', 2, []], ['foo', 2, []], ['foo/bar', 2, []], ['foo/bar/baz', 1, []], ['foo/bar/xattr', 1, []], ['foo/bar/acl', 1, []], ['foo/bar/\xc3\xa6\xc3\xb8\xc3\xa5', 1, []], ['foo/bar/fifo', 4, []], ['foo/bar/hln', 1, []], ['foo/bar/sln', 3, []]]

```


## ?
```cpp
//in SyscallMutator.hpp
 Syscall* createTarget(FileObject* fobj) {
   // arg2: flags (very simple now)
                int64_t flags;
                if (fobj->type == I_DIR)
                    flags = O_RDONLY | O_DIRECTORY;
                else
                    flags = O_RDWR;
                syscall->add_arg(new Arg(flags, 0));
//when we read read-only file what will happen?
```

----------------------------------------

### crash test

### mksquash
```
mksquashfs ./ctest ctest4.sqsh
Parallel mksquashfs: Using 1 processor
Creating 4.0 filesystem on ctest4.sqsh, block size 131072.
[===================================================================|] 5/5 100%

Exportable Squashfs 4.0 filesystem, gzip compressed, data block size 131072
	compressed data, compressed metadata, compressed fragments, compressed xattrs
	duplicates are removed
Filesystem size 4.05 Kbytes (0.00 Mbytes)
	22.90% of uncompressed filesystem size (17.71 Kbytes)
Inode table size 162 bytes (0.16 Kbytes)
	42.52% of uncompressed inode table size (381 bytes)
Directory table size 125 bytes (0.12 Kbytes)
	73.96% of uncompressed directory table size (169 bytes)
Number of duplicate files found 3
Number of inodes 12
Number of files 7
Number of fragments 1
Number of symbolic links  1
Number of device nodes 0
Number of fifo nodes 1
Number of socket nodes 0
Number of directories 3
Number of ids (unique uids + gids) 2
Number of uids 2
	root (0)
	grapefruit (1000)
Number of gids 2
	root (0)
	grapefruit (1000)
```

### squashfs format
----------------------
ref. https://dr-emann.github.io/squashfs/#superblock
https://www.kernel.org/doc/Documentation/filesystems/squashfs.txt

#### - superblock

block_size = 2^block_log // checksum 처럼 작용

<sup>If block_size and block_log do not agree, the archive is considered corrupt</sup>

#### - sanity check for super block contents
unsquashfs. unsquash-4.c
```cpp
squashfs_operations *read_filesystem_tables_4()
```

** sanity check 확인 중




##### TODO
- squashfs_fuzzer files
- study fs structures




ref.

https://blog.exploitee.rs/2013/chromecast-exploiting-the-newest-device-by-google/

https://0day.work/a-crash-in-unsquashfs/
