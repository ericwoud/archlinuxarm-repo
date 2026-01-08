# Maintainer: Eric Woudstra <ericwouds@gmail.com>

pkgname=i2csfp-static-git
pkgver=1.95df5e6
pkgrel=1
pkgdesc="Static build of Utility for debugging i2c sfp modules"
arch=('x86_64' 'aarch64')
url="https://github.com/ericwoud/i2csfp"
license=('GPL2')
depends=()
makedepends=('git' 'musl' 'kernel-headers-musl' 'linux-api-headers')
options=('!lto')
source=("git+https://github.com/ericwoud/i2csfp.git")
sha512sums=('SKIP')

# to enable func64 interface in musl for 64-bit file system functions
# disable exceptions for musl build
export CFLAGS+=' -Os -static -D_LARGEFILE64_SOURCE -fno-exceptions'
export CC=musl-gcc

pkgver() {
  cd "${srcdir}/i2csfp"
  _rev=$(git rev-list --count $_tag..HEAD)
  _rev=$(( _rev + $(git -C "${startdir}" rev-list --count HEAD) ))
  _hash=$(git rev-parse --short HEAD)
  printf "%s.%s" "$_rev" "$_hash" | sed 's/^v//;s/_/./g'
}

build() {
  cd "${srcdir}/i2csfp"
  # musl's linux/gpio.h is too old
  sed -i 's@<linux/gpio.h>@"/usr/include/linux/gpio.h"@g' i2csfp.c
  $CC $CFLAGS -o i2csfp i2csfp.c
}

package() {
  cd "${srcdir}/i2csfp"
  install -m755 -vDt $pkgdir/usr/bin i2csfp
}
