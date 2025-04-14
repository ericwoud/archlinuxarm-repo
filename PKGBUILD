# Maintainer: Eric Woudstra <ericwouds@gmail.com>

pkgname=("bpir-initrd")
url="https://github.com/ericwoud/archlinuxarm-repo/tree/$pkgname"
license=('GPL')
arch=('any')
pkgver=1.0
pkgrel=1
source=('bpir-initrd')
sha256sums=(SKIP)
depends=('cpio')
provides=('initramfs')

package() {
  cd "${startdir}"
  install -m700 -vDt $pkgdir/usr/bin bpir-initrd
}
