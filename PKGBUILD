# Maintainer: Eric Woudstra <ericwouds@gmail.com>

pkgbase="build-r64-arch-utils-git"
pkgname=("$pkgbase")
url="https://github.com/ericwoud/buildR64arch"
license=('GPL')
arch=('aarch64')
pkgver=59.447.69b6dad
pkgrel=1
source=("git+https://github.com/ericwoud/buildR64arch.git"
        'kernel-preinst'
        'kernel-postinst'
        'kernel-prerm')
sha256sums=(SKIP SKIP SKIP SKIP)

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
           hostapd wireless-regdb iptables-nft iproute2 nftables f2fs-tools dosfstools
           btrfs-progs patch sudo evtest parted binutils cpio mtd-utils diffutils
           nano screen i2c-tools ethtool iperf3 curl wget debootstrap usbutils
           linux-firmware-other linux-firmware-mediatek ericwoud-keyring
           openssh dtc mmc-utils-git uboot-tools base dbus-broker-units inetutils)
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
  cp -vr boot            ${pkgdir}/usr/share/buildR64arch/
  cp -vr etc             ${pkgdir}/usr/share/buildR64arch/
  cp -vr network         ${pkgdir}/usr/share/buildR64arch/
  cp -vr hostapd         ${pkgdir}/usr/share/buildR64arch/
  cp -vr keyring         ${pkgdir}/usr/share/buildR64arch/
  cp -vr skeleton-apt    ${pkgdir}/usr/share/buildR64arch/
  cp -vr skeleton-pacman ${pkgdir}/usr/share/buildR64arch/

  cd "${startdir}"
  install -m755 -vTD ./kernel-preinst  $pkgdir/etc/kernel/preinst.d/70-$pkgbase
  install -m755 -vTD ./kernel-postinst $pkgdir/etc/kernel/postinst.d/70-$pkgbase
  install -m755 -vTD ./kernel-prerm    $pkgdir/etc/kernel/prerm.d/70-$pkgbase
}

_package() {
  pkgdesc="BuildR64arch $1 packages"
  depends=(build-r64-arch-utils-git linux-$1-git $1-atf-git $1-uboot-git)
  install=packages-bpir.install
}

for _target in bpir64 bpir3 bpir3m bpir4; do
  eval "package_packages-${_target}() { _package ${_target} ; }"
  pkgname+=(packages-${_target})
done
