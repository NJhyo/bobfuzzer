# Android Kernel

android kernel : android-4.9-p

linux kernel : linux-4.9.118


## Comparison(working)

[+] crypto\\sepck.c in android kernel



## File system(sdcard FS)
VFS - sdcardfs - file system(eg. ext4 ... )

fs\\sdcardfs\\*

- ##### about stackable file system
https://www.linuxjournal.com/article/6485

#### linux file system
![alt text](https://myaut.github.io/dtrace-stap-book/images/linux/vfs.png)
<sup>https://myaut.github.io/dtrace-stap-book/images/linux/vfs.png<sup>

#### data structures
```cpp
struct sdcardfs_file_info {
	struct file *lower_file;
	const struct vm_operations_struct *lower_vm_ops;
};

```

```cpp
int wrapfs_unlink(struct inode *dir,
                  struct dentry *dentry)
{
  int err = 0;
  struct inode *lower_dir;
  struct dentry *lower_dentry;
  lower_dir = get_lower_inode(dir);
  lower_dentry = get_lower_dentry(dentry);
  /* pre-call code can go here */
  err = lower_dir->i_op->unlink(lower_dir,
                                lower_dentry);
  /* post-call code can go here */
  return err;
}
```


## supported file system
- SD - FAT -2GB
- SDHC - FAT32 -32GB
- SDXC - exFAT - 2TB
- F2FS replaced by sdcardfs
