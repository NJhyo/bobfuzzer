1. 관련 apt 설치
```sudo apt install mtd-utils```


#h1 nandsim -> MTD -> ubi -> ubifs

ubifs -> *ubi 모듈을 사용
*ubi -> **MTD 모듈을 사용
**MTD -> nandsim 을 사용

*** 
UBIFS 가 MTD라는 장치가 중간에 있어서 이미지 만들고 다시 재 마운트 하는데 꽤나 애를 먹었따!!!

1. 이미지를 만드는 방법.
```
sudo modprobe nandsim 		                                                //nandsim 모듈 로드
cat /proc/mtd							                                               //mtd 확인
sudo modprobe ubi mtd=0						                                       //mtd0에 ubi모듈 추가
sudo ubimkvol /dev/ubi0 -N rootfs -s 10MiB			                          //volume 생성
sudo mkdir ~/ubifs						                                           //마운트할 디렉토리 생성
sudo mount -t ubifs ubi0:rootfs ~/ubifs			                             //마운트 시작
df 								                                                       //마운트 확인
mkfs.ubifs -q -v -r ~/ubifs/ -m 512 -e 15872 -c 2047 -o ~/ubifs.img -F   //ubifs 파일시스템 img로 만들기.

```

2. 이미지 마운트 하는 방법

```
sudo apt install mtd-utils                  //마운트할때 mtd-utils 
sudo modprobe nandsim 		                  //nandsim 모듈 로드
cat /proc/mtd							                  //mtd 확인
sudo modprobe ubi mtd=0				              //mtd0에 ubi모듈 추가
sudo ubimkvol /dev/ubi0 -N rootfs -s 10MiB	//volume 생성
sudo mkdir ~/ubifs						              //마운트할 디렉토리 생성
sudo ubiupdatevol /dev/ubi0_0 ~/ubifs.img   //이미지를 ubi0_0에 업데이트!
sudo mount -t ubifs ubi0:rootfs ~/ubifs			//마운트 시작
df 								                          //마운트 확인

```

***

마운트 해제
```
sudo umount ~/ubifs
sudo rmmod ubifs ubi nandsim
```
