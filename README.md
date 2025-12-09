# chris-ping
A PING from scratch

sudo apt install gcc-14 gcc-14-multilib libc6-dev-i386

sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-14 100

`sudo strace -f ./ping 2>&1 | grep sendto`