디버깅용 ubuntu base rootfs
======================
# 1. ubuntu base rootfs

기존에 사용하던 syzkaller로 만든 rootfs의 경우 rootfs자체적인 오류 때문에 지속적인 
디버깅 메시지가 뜨는 바람에 마운트를 시킨후에도 crash에 의한 메시지인지 원래 뜨는 메시지인지 의심스러워
기존에 사용하던 syzkaller rootfs를 버리고 ubuntu base의 이미지를 만들어 사용하고자 한다.

debootstrap을 이용하여 ubuntu base의 이미지를 만들려고 한다. 

방법은 간단하다 정말로...
```C
#/bin/bash

IMG=qemu-image.img
DIR=mount-point.dir
qemu-img create $IMG 5g
mkfs.ext2 $IMG
mkdir $DIR
sudo mount -o loop $IMG $DIR
sudo debootstrap --arch amd64 xenial $DIR
sudo umount $DIR
rmdir $DIR
```
위에있는 스크립트 하나만 실행하면 자동으로 이미지를 만들어준다!
mkfs.ext2 $IMG <= 여기서 mkfs 뒤의 내용을 원하는 파일시스템으로 변경도 가능하다.

sudo debootstrap --arch amd64 xenial $DIR 해당 줄의 --arch 뒤의 내용을 변경하면 원하는 아키텍쳐에 맞춰 파일시스템을 생성할 수 있다.

rootfs이미지가 생성된 후 해당 이미지를 마운트하고 root 계정의 암호를 설정한다.
```
mkdir ./mnt
mount ./qemu-image.img ./mnt 
cd ./mnt
chroot .
passwd root //root 계정 암호 설정
cd ..
umount ./mnt
```

qemu-system-x86_64 -kernel ../bzImage.1 -append "root=/dev/sda rw console=ttyS0 nokaslr" -m 512 -hda qemu-image.img -nographic > logsss

qemu를 통한 실행 명령어
-kernel bzImage : 빌드한 커널 이미지, linux/~~~ 에 빌드된걸 실행하는 디렉토리로 가져왔음. 
-append
    root=/dev/sda rw : ubuntu가 사용하고 있는 /dev/sda의 공간을 사용함, rw가 없으면 쓰기가 안되서 root여도 시스템 파일 수정이 안됨
    console=ttyS0 : 지금 사용하고 있는 터미널로 콘솔이 떨어짐 물론 ssh로 접속중이라면 pty 번호가 들어가면 똑같이 됨
    nokalsr : 커널 aslr disable
-m 512 : 메모리 할당
-hda qemu-image.img : 2번에서 만들었던 rootfs 이미지
-net user,hostfwd=tcp::2222=:22 : 네트워크 되면 ssh를 연결해서 접속해려고 포트포워딩을 해줌. 안됨. 필요없음.
--nographic : 위에 -append "console=ttyS0"랑 연관되서 그래픽 표시를 안하고 콘솔만 사용하겠다는 뜻
-s -S : -s는 기본 1234번 포트로 디버거 접속을 대기하겠다는 뜻. -S 는 커널 시작 코드에서 멈춘 후 대기하겠다는 뜻

출처: https://satanel001.tistory.com/183 [oxqo's Hacklab : 개발.해킹.시험.피곤.사망]