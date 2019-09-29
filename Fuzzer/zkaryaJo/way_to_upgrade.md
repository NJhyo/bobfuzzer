기존 JANUS에서 비틀어 볼 부분 확인.

1. 버전 업으로 인한 syscall 추가.
2. seed 이미지 변경?
3. 퍼저 스케쥴링 변경?
4. trimming 하기?
5. 

1. 버전 업으로 인한 syscall 추가.
linux 4.17 -> 5.0으로 버전 올라가면서 추가된 syscall 함수.

추가된 syscall
```C
sys_io_pgetevents
sys_io_pgetevents_time32
sys_recvmmsg_time32
sys_rseq
ksys_personality

__ARCH_WANT_SYS_UTIME
```

삭제된 syscall
```C
sys_utimes
sys_futimesat
```

2. seed 이미지 변경?

우선 샘플 이미지를 확인해 보자. 

```
sudo mount -t btrfs -o rw ~/janus/samples/evaluation/btrfs-00.image ~/mnt -o loop
```

![bitmap](./img/?.png)


3. 퍼저 스케쥴링 변경?



![bitmap](./img/?.png)
