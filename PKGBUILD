# Maintainer: Eric Woudstra <ericwouds@gmail.com>

pkgname=("build-r64-arch-utils-git")
url="https://github.com/ericwoud/buildR64arch"
license=('GPL')
arch=('any')
pkgver=r1
pkgrel=1
source=("git+https://github.com/ericwoud/buildR64arch.git"
	'95-bpir-toolbox-fip.hook'
	'95-bpir-toolbox-atf.hook'
	'95-bpir-toolbox-uboot-inst.hook'
	'95-bpir-toolbox-uboot-rem.hook')
sha256sums=(SKIP SKIP SKIP SKIP SKIP)
install=${pkgname}.install

pkgver() {
  cd "${srcdir}/buildR64arch"
  printf "r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}

package() {
  cd "${startdir}"
  install -m644 -vDt "${pkgdir}/usr/share/libalpm/hooks/" 95-bpir-toolbox-fip.hook
  install -m644 -vDt "${pkgdir}/usr/share/libalpm/hooks/" 95-bpir-toolbox-atf.hook
  install -m644 -vDt "${pkgdir}/usr/share/libalpm/hooks/" 95-bpir-toolbox-uboot-inst.hook
  install -m644 -vDt "${pkgdir}/usr/share/libalpm/hooks/" 95-bpir-toolbox-uboot-rem.hook

  cd "${srcdir}/buildR64arch/rootfs/bin"
  install -m755 -vDt $pkgdir/usr/bin bpir-toolbox

  cd "${srcdir}/buildR64arch/rootfs/services"
  install -m644 -vDt $pkgdir/etc/systemd/system setmac.service

  cd "${srcdir}/buildR64arch/rootfs"
  install -vDd ${pkgdir}/usr/share/buildR64arch
  cp -vr boot    ${pkgdir}/usr/share/buildR64arch/
  cp -vr etc     ${pkgdir}/usr/share/buildR64arch/
  cp -vr network ${pkgdir}/usr/share/buildR64arch/
  cp -vr hostapd ${pkgdir}/usr/share/buildR64arch/
}
