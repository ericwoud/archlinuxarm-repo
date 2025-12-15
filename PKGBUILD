# Maintainer: Eric Woudstra <ericwouds@gmail.com>

pkgname=("makedeb")
url="https://github.com/ericwoud/archlinuxarm-repo/tree/$pkgname"
license=('GPL')
arch=('any')
pkgdesc='Build .deb for my bpi router packages from pkg directory'
pkgver=1.a47b774
pkgrel=1
source=('makedeb')
sha256sums=(SKIP)
depends=()

pkgver() {
  cd ""${startdir}""
  printf "%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short=7 HEAD)"
}

package() {
  cd "${startdir}"
  install -m755 -vDt $pkgdir/usr/bin makedeb
}
