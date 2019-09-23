ext4 
![ext4_layout_all](./img/ext4_layout_all.png)

![ext4_group_layout](./img/ext4_group_layout.png)

flexable block group 일 때의 ext4는 하단의 그림과 같이 여러 블록 그룹을 논리적으로 하나의 블록 그룹으로 묶어주는 역할을 한다.
블록 그룹 0에 block bitmap, inode bitmap, inode table 들을 가지고 있어 관리가 용이하다.
![ext4_group_layout](./img/flex_ext4.png)

