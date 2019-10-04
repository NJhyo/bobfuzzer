1. 관련 apt 설치
```sudo apt install mtd-utils```


#h1 nandsim -> MTD -> ubi -> ubifs

ubifs -> *ubi 모듈을 사용
*ubi -> **MTD 모듈을 사용
**MTD -> nandsim 을 사용

***
```
sudo modprobe nandsim total_size=32768			//nandsim 모듈 로드
cat /proc/mtd							//mtd 확인
sudo modprobe ubi mtd=0						//mtd0에 ubi모듈 추가
sudo ubimkvol /dev/ubi0 -N rootfs -s 10MiB			//volume 생성
sudo mkdir /mnt/ubifs						//마운트할 디렉토리 생성
sudo mount -t ubifs ubi0:rootfs /mnt/ubifs			//마운트 시작
df 								//마운트 확인
mkfs.ubifs -q -r /mnt/ubifs/ -m 1 -e 129024 -c 2047 -o ubifs.img//ubifs 파일시스템 img로 만들기.
touch ubinize.cfg						//ubinize config 파일 생성
vim ubinize.cfg							//수정

[ubifs]
mode=ubi
image=ubifs.img
vol_id=0
vol_size=200MiB
vol_type=dynamic
vol_name=rootfs
vol_flags=autoresize

sudo ubinize -o ubi.img -m 2048 -p 128KiB -s 512 ubinize.cfg		//ubi.img 생성
```
***

마운트 해제
sudo umount ~/ubifs
sudo rmmod ubifs ubi nandsim
