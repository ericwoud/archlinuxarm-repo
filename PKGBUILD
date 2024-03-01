# Maintainer: Eric Woudstra <ericwouds@gmail.com>

pkgname=("build-rk-arch-utils-git")
url="https://github.com/ericwoud/buildRKarch"
license=('GPL')
arch=('any')
pkgver=r53.9e25a90
pkgrel=1
source=("git+https://github.com/ericwoud/buildRKarch.git"
	'95-rockchip-toolbox.hook')
sha256sums=(SKIP SKIP)
install=${pkgname}.install

pkgver() {
  cd "${srcdir}/buildRKarch"
  printf "r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}

package() {
  cd "${srcdir}/buildRKarch/rootfs/bin"
  install -m755 -vDt $pkgdir/usr/bin rockchip-toolbox
  install -m755 -vDt $pkgdir/usr/bin rockchip-postinstall

  cd "${srcdir}/buildRKarch/rootfs"
  install -vDd ${pkgdir}/share/buildRKarch
  cp -vr boot  ${pkgdir}/share/buildRKarch/

  cd "${startdir}"
  install -m644 -vDt "${pkgdir}/usr/share/libalpm/hooks/" 95-rockchip-toolbox.hook
}
