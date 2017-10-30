Following guidelines from [this](http://arjunsreedharan.org/post/82710718100/kernel-101-lets-write-a-kernel) guide to make a simple kernel

## Setup Instructions

1. Clone the repository `git clone https://github.com/Viktree/Simple-Linux-Kernal.git`
2. Enter into the repsitory `cd Simple-Linux-Kernal`
3. Create the kernal with `make`
4. Copy the kernel to the boot directory: `mv kernel /boot/kernel-01`. Note that this renames the kernel program to kernel-01
5. Change the permissions so that we can edit the grub. `chmod u+xw /boot/grub/grub.cfg`.
6. Add the following section to the grub configuration. The grub configuration can be edited by running `sudo vim /boot/grub/grub.cfg`. Add the following lines to the grub:

```bash
menuentry 'kernel 701' {
	set root='hd0,msdos1'
	multiboot /boot/kernel-701 ro
}
```