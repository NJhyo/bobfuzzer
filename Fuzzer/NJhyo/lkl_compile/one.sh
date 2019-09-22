#!/bin/sh
if [ $# -eq 0 ]
then
	echo "Usage : one.sh [btrfs|ext4|f2fs]"
	echo "Currently only btrfs is supported."
	exit 0
fi

echo "$#"

types=$1

echo "FileSystem Name = $types"

echo "install dependencies"

sudo apt update
sudo apt upgrade -y
sudo apt install llvm clang libboost-dev libfuse-dev libarchive-dev xfsprogs bison flex zlib1g-dev python-pip

sudo pip install xattr
sudo pip install pathlib2

echo "Download Fuzzer"
wget http://211.193.58.170:9000/janus.tar.gz

echo "Dcompress"
tar -xvzf janus.tar.gz
rm janus.tar.gz


echo "Compile ff-gcc"
make -C ./janus/ff-gcc clean
make -C ./janus/ff-gcc

echo "Compile core"
make -C ./janus/core clean
make -C ./janus/core

echo "Compile Image Parser"
make -C ./janus/fs/$types clean
make -C ./janus/fs/$types


echo "Make Seed Images"
sudo ./janus/istat/istat -i ./janus/samples/evaluation/$types-00.image -t $types -o ./janus/istat/$types.istat 

echo "Create Seed Programs"
mkdir ./janus/prog
sudo ./janus/core/create_corpus ./janus/istat/$types.istat ./janus/prog

echo core >/proc/sys/kernel/core_pattern

