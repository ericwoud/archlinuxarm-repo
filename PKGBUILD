# Maintainer: Eric Woudstra <ericwouds@gmail.com>

pkgname=("build-r64-arch-utils-git")
url="https://github.com/ericwoud/buildR64arch"
license=('GPL')
arch=('aarch64')
pkgdesc='Utilities for BPI Router boards installed with buildR64arch'
pkgver=37.384.d7e7f5c
pkgrel=1
depends=(bpir-atf-git-fiptool initramfs sudo
         hostapd wireless-regdb iproute2 nftables f2fs-tools dosfstools
         btrfs-progs patch sudo evtest parted binutils cpio mtd-utils diffutils
         nano screen i2c-tools ethtool iperf3 curl wget debootstrap usbutils
         bpir-atf-git bpir-uboot-git ssh-fix-reboot hostapd-launch
         linux-firmware-other linux-firmware-mediatek)
depends_alarm=( openssh        dtc                  mmc-utils-git uboot-tools
                base dbus-broker-units)
depends_debian=(openssh-server device-tree-compiler mmc-utils     u-boot-tools
                libpam-systemd systemd-timesyncd systemd-resolved kmod zstd
                iputils-ping iw file)

source=("git+https://github.com/ericwoud/buildR64arch.git")
sha256sums=(SKIP)
install=${pkgname}.install

pkgver() {
  cd "${srcdir}/buildR64arch"
  printf "%s.%s.%s" "$(git -C "${startdir}" rev-list --count HEAD)" \
                    "$(git rev-list --count HEAD)" \
                    "$(git rev-parse --short HEAD)"
}

package() {
  depends+=(${depends_alarm[*]})
  cd "${srcdir}/buildR64arch/rootfs/bin"
  install -m755 -vDt $pkgdir/usr/bin bpir-rootfs
  install -m755 -vDt $pkgdir/usr/bin bpir-flash2emmc
  install -m755 -vDt $pkgdir/usr/bin bpir-toolbox
  cd "${srcdir}/buildR64arch"
  install -m755 -vT ./build.sh $pkgdir/usr/bin/bpir-build

  cd "${srcdir}/buildR64arch/rootfs/services"
  install -m644 -vDt $pkgdir/etc/systemd/system setmac.service

  cd "${srcdir}/buildR64arch/rootfs"
  install -vDd ${pkgdir}/usr/share/buildR64arch
  cp -vr boot    ${pkgdir}/usr/share/buildR64arch/
  cp -vr etc     ${pkgdir}/usr/share/buildR64arch/
  cp -vr network ${pkgdir}/usr/share/buildR64arch/
  cp -vr hostapd ${pkgdir}/usr/share/buildR64arch/
}
