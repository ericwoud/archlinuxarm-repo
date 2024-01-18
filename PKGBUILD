# PeekPoke
# Maintainer: Eric Woudstra <ericwouds AT gmail DOT com>

_pkgname=peekpoke
pkgname=${_pkgname}-git
pkgver=r14.913be80
pkgrel=1
pkgdesc="Linux userland tool to read and write arbitrary memory locations"
arch=('any')
url="https://github.com/apritzel/peekpoke/tree/master"
license=('GPL')
makedepends=('git')
provides=("${pkgname}")
conflicts=("${pkgname}")
source=('peekpoke::git+https://github.com/apritzel/peekpoke.git')
md5sums=('SKIP')

pkgver() {
	cd "$srcdir/${_pkgname}"
	printf "r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}

build() {
	cd "$srcdir/${_pkgname}"
	make
}

package() {
	cd "$srcdir/${_pkgname}"
	install -m755 -vDt "$pkgdir/usr/bin" peekpoke
}
