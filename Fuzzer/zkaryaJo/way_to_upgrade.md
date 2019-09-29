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

우선 샘플 이미지에 어떤 것들이 있는지 확인해 보자. 

```

```
EXT4 부터 살펴보면, 하단의 이미지에서 lost+found는 ext4에서 기본적으로 손상되거나 알수없는 파일들의 정보가 보관되는 곳이라고 한다. 이건 파일시스템 별로 있을 수 있으므로 상관이 없을 것 같다.

![bitmap](./img/way_to_upgrade/ext4_1.png)

그럼 foo 디렉토리를 살펴보자. 다음과 같이 6개의 파일들이 존재한다.
![bitmap](./img/way_to_upgrade/ext4_2.png)


파일의 형식은 다음의 이미지에 있는 사진과 같다. 
![bitmap](./img/way_to_upgrade/ext4_3.png)


여기서 추가하거나 바꿀 수 있는 부분이 있을까 생각해보며 자료를 찾다 보니 다음과 같은 자료가 나왔다.

https://bugzilla.kernel.org/show_bug.cgi?id=202485

다음의 링크를 들어가서 구글번역기로 좀 더 편하게 보면 
![bitmap](./img/way_to_upgrade/seed_upg_1.png)
![bitmap](./img/way_to_upgrade/seed_upg_3.png)


이에 대한 답변으로
![bitmap](./img/way_to_upgrade/seed_upg_4.png)

다음과 같이 달렸다.

초기 seed이미지에 특수파일들을 넣는다면 crash가 나는 부분들도 기존 JANUS와는 다른 방식으로 나오지 않을까 생각하는데...ㅠ

3. 퍼저 스케쥴링 변경?
