# Maintainer: Eric Woudstra <ericwouds@gmail.com>

pkgname=ethtool-static-git
pkgver=v6.15.26.c615ea54
pkgrel=1

pkgdesc="Static build of Utility for controlling network drivers and hardware"
arch=('x86_64' 'aarch64')
url="https://www.kernel.org/pub/software/network/ethtool/"
license=('GPL2')
depends=()
makedepends=('git' 'musl' 'kernel-headers-musl')
provides=("ethtool=$pkgver")
conflicts=('ethtool')
options=('!emptydirs' '!lto')

source=("git+https://git.kernel.org/pub/scm/network/ethtool/ethtool.git"
        "git://git.netfilter.org/libmnl.git"
)
sha512sums=('SKIP' 'SKIP')

# to enable func64 interface in musl for 64-bit file system functions
# disable exceptions for musl build
export CFLAGS+=' -D_LARGEFILE64_SOURCE -fno-exceptions'
export CXXFLAGS+=' -D_LARGEFILE64_SOURCE -fno-exceptions'
export LDFLAGS="$LDFLAGS -static"
export CC=musl-gcc
export CXX=musl-gcc

prepare() {
  echo
}

pkgver() {
  cd "${srcdir}/ethtool"
  _tag=$(git tag -l --sort -v:refname | head -n1)
  _rev=$(git rev-list --count $_tag..HEAD)
  _rev=$(( _rev + $(git -C "${startdir}" rev-list --count HEAD) ))
  _hash=$( git                       rev-parse --short HEAD | tail -c 5)
  _hash+=$(git -C "${srcdir}/libmnl" rev-parse --short HEAD | tail -c 5)
  printf "%s.%s.%s" "$_tag" "$_rev" "$_hash" | sed 's/^hostap_//;s/_/./g'
}

build() {
  export PKG_CONFIG_PATH="${srcdir}"/temp/usr/lib/pkgconfig
  export PATH="${srcdir}/temp/usr/bin:${PATH}"

  cd "${srcdir}"/"libmnl"
  ./autogen.sh
  ./configure \
      --prefix="${srcdir}"/temp/usr \
      --enable-static --disable-shared
  make install

  rm "${srcdir}"/temp/usr/lib/lib*.la

  cd "${srcdir}/ethtool"
  _staticlibs="-Wl,-rpath=${srcdir}/temp/usr/lib -L${srcdir}/temp/usr/lib -I${srcdir}/temp/usr/include"
  export   LDFLAGS+="  -pie $_staticlibs"
  export    CFLAGS+=" -fPIE $_staticlibs"
  export CXXDFLAGS+=" -fPIE $_staticlibs"

  ./autogen.sh
  ./configure \
    --prefix="/usr" \
    --sbindir="/usr/bin" \
    --enable-static --disable-shared
  make
}

package() {
  cd "${srcdir}/ethtool"
  make DESTDIR="$pkgdir" install
}
