# Maintainer: Eric Woudstra <ericwouds@gmail.com>

pkgname=bash-static-git
pkgver=5.3.0.c868696bb
pkgrel=1

pkgdesc="Static build of bash from git"
arch=('x86_64' 'aarch64')
url='https://www.gnu.org/software/bash/bash.html'
license=('GPL2')
depends=()
makedepends=('git' 'musl' 'kernel-headers-musl')
options=('!emptydirs' '!lto')

source=(git+https://git.savannah.gnu.org/git/bash.git
        git+https://git.savannah.gnu.org/git/readline.git
        git+https://github.com/ThomasDickey/ncurses-snapshots.git)
sha512sums=('SKIP' 'SKIP' 'SKIP')

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
  cd "${srcdir}/bash"
  _ver="$(grep -m1 '@set VERSION' doc/version.texi | cut -d ' ' -f3 | tr - .)"  _rev=$(git rev-list --count $_tag..HEAD)
  _hash=$( git                                  rev-parse --short HEAD | tail -c 4)
  _hash+=$(git -C "${srcdir}/ncurses-snapshots" rev-parse --short HEAD | tail -c 4)
  _hash+=$(git -C "${srcdir}/readline"          rev-parse --short HEAD | tail -c 4)
  printf "%s.%s.%s" "$_ver" "$_rev" "$_hash" | sed 's/^v//;s/_/./g'
}

build() {
  export PKG_CONFIG_PATH="${srcdir}"/temp/usr/lib/pkgconfig
  export PATH="${srcdir}/temp/usr/bin:${PATH}"

  cd "${srcdir}"/"readline"
  ./configure \
      --prefix="${srcdir}"/temp/usr \
      --enable-static --disable-shared
  make install

cd "${srcdir}"/"ncurses-snapshots"
  CFLAGS="$CFLAGS -ffat-lto-objects" \
  CXXFLAGS="$CXXFLAGS -ffat-lto-objects" \
  ./configure \
    --prefix="${srcdir}"/temp/usr \
    --enable-static --disable-shared \
    --mandir="/usr/share/man" \
    --enable-pc-files \
    --enable-widec \
    --with-versioned-syms \
    --with-xterm-kbs=del \
    --without-ada \
    --without-debug \
    --with-normal --without-cxx-binding
  make install

  cd "${srcdir}/bash"
  _staticlibs="-Wl,-rpath=${srcdir}/temp/usr/lib -L${srcdir}/temp/usr/lib -I${srcdir}/temp/usr/include"
  export   LDFLAGS+="  -pie $_staticlibs"
  export    CFLAGS+=" -fPIE $_staticlibs"
  export CXXDFLAGS+=" -fPIE $_staticlibs"
  _bashconfig=(-DDEFAULT_PATH_VALUE=\'\"/usr/local/sbin:/usr/local/bin:/usr/bin\"\'
               -DSTANDARD_UTILS_PATH=\'\"/usr/bin\"\'
               -DSYS_BASHRC=\'\"/etc/bash.bashrc\"\'
               -DSYS_BASH_LOGOUT=\'\"/etc/bash.bash_logout\"\'
               -DNON_INTERACTIVE_LOGIN_SHELLS)
  export CFLAGS="${CFLAGS} ${_bashconfig[@]}"
  ./configure \
    --prefix=/usr \
    --with-curses \
    --enable-readline \
    --without-bash-malloc \
    --with-installed-readline \
  --enable-static --disable-shared --enable-static-link
   make
}

package() {
  cd "${srcdir}/bash"
  install -m755 -vDT ./bash $pkgdir/usr/bin/bash-static
}
