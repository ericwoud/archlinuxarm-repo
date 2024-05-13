# Maintainer: Eric Woudstra <first name last name 256 at gmail dot com>

pkgname=libgpiod-dbus-git
pkgver=r1304.f8dfc32
pkgrel=1
pkgdesc="C library and tools for interacting with the linux GPIO character device"
url="https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git"
arch=('x86_64' 'i686' 'arm' 'armv6h' 'armv7h' 'aarch64')
license=('LGPL2.1')
depends=('linux-api-headers>=5.5')
conflicts=('libgpiod')
provides=('libgpiod')
makedepends=('git' 'autoconf-archive' 'help2man' 'python-setuptools' 'python-build')
source=('libgpiod::git+https://github.com/brgl/libgpiod-private.git#branch=topic/dbus')
sha1sums=('SKIP')

pkgver() {
  cd "libgpiod"
  printf "r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}

build() {
  cd "libgpiod"
  ./autogen.sh \
    --enable-tools=yes \
    --enable-bindings-cxx \
    --enable-bindings-python \
    --enable-dbus \
    --enable-bindings-glib \
    --prefix=/usr
  sed -i -e 's/ -shared / -Wl,-O1,--as-needed\0/g' libtool
  make
}

package() {
  cd "libgpiod"
  sed -i s,--prefix='$(prefix)',--root="$pkgdir", bindings/python/Makefile.am  
  make DESTDIR="$pkgdir/" install
  install -Dm644 "dbus/data/gpio-manager.service" -t "$pkgdir"/usr/lib/systemd/system
}
