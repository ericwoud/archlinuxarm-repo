# Maintainer: Eric Woudstra <ericwouds@gmail.com>

pkgbase="build-r64-arch-utils-git"
pkgname=("$pkgbase")
url="https://github.com/ericwoud/buildR64arch"
license=('GPL')
arch=('aarch64')
pkgver=54.434.6d7c634
pkgrel=1
source=("git+https://github.com/ericwoud/buildR64arch.git")
sha256sums=(SKIP)
export CARCH=aarch64

pkgver() {
  cd "${srcdir}/buildR64arch"
  printf "%s.%s.%s" "$(git -C "${startdir}" rev-list --count HEAD)" \
                    "$(git rev-list --count HEAD)" \
                    "$(git rev-parse --short HEAD)"
}

package_build-r64-arch-utils-git() {
  pkgdesc='Utilities for BPI Router boards installed with buildR64arch'
  depends=(bpir-atf-git-fiptool initramfs sudo
           hostapd wireless-regdb iproute2 nftables f2fs-tools dosfstools
           btrfs-progs patch sudo evtest parted binutils cpio mtd-utils diffutils
           nano screen i2c-tools ethtool iperf3 curl wget debootstrap usbutils
           linux-firmware-other linux-firmware-mediatek
           openssh dtc mmc-utils-git uboot-tools base dbus-broker-units iptables-nft)
  depends+=(${depends_alarm[*]})
  install=${pkgname}.install
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

_package() {
  pkgdesc="BuildR64arch $1 packages"
  depends=(build-r64-arch-utils-git linux-$1-git $1-atf-git $1-uboot-git)
}

for _target in bpir64 bpir3 bpir3m bpir4; do
  eval "package_packages-${_target}() { _package ${_target} ; }"
  pkgname+=(packages-${_target})
done
