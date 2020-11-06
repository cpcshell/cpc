#!/bin/bash

if [ -d jail ]; then 
    exit 0
fi

wget https://raw.githubusercontent.com/tokland/arch-bootstrap/master/arch-bootstrap.sh 
chmod +x arch-bootstrap.sh
mkdir jail 
sudo ./arch-bootstrap.sh jail
chroot ./jail /bin/bash -c "pacman -Syu --noconfirm  git freebasic libzip libx11 libxrandr libxext libxpm libpng"
rm arch-bootstrap.sh
cp -r Sysroot/* /jail/ 
