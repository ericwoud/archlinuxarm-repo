# Maintainer: Eric Woudstra <ericwouds@gmail.com>

pkgname=("build-r64-arch-utils-git")
url="https://github.com/ericwoud/buildR64arch"
license=('GPL')
arch=('any')
pkgver=r1
pkgrel=1
source=("git+https://github.com/ericwoud/buildR64arch.git"
	'95-bpir-toolbox.hook')
sha256sums=(SKIP SKIP)
install=${pkgname}.install

pkgver() {
  cd "${srcdir}/buildRKarch"
  printf "r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}

package() {
  cd "${startdir}"
  install -m644 -vDt "${pkgdir}/usr/share/libalpm/hooks/" 95-bpir-toolbox.hook

  cd "${srcdir}/buildRKarch/rootfs/bin"
  install -m755 -vDt $pkgdir/usr/bin bpir-toolbox

  cd "${srcdir}/buildR64arch/rootfs"
  install -vDd ${pkgdir}/usr/share/buildR64arch
  cp -vr boot  ${pkgdir}/usr/share/buildR64arch/
  cp -vr etc   ${pkgdir}/usr/share/buildR64arch/
}
