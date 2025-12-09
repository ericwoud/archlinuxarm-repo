# Maintainer: Eric Woudstra <ericwouds@gmail.com>

pkgname=("bpir-initrd")
url="https://github.com/ericwoud/archlinuxarm-repo/tree/$pkgname"
license=('GPL')
arch=('aarch64')
pkgdesc='Build customized initrd for BPI Router Boards'
_pkgver=1
pkgver=1.95.db23030
pkgrel=1
source=('bpir-initrd'
        'git+https://github.com/ericwoud/daft-dhcp-client.git'
        'pkgdetails.c::https://salsa.debian.org/installer-team/base-installer/-/raw/master/pkgdetails.c'
        )
sha256sums=(SKIP SKIP SKIP)
depends=('cpio' 'which' curl wget debootstrap)
provides=('initramfs')

pkgver() {
  cd ""${startdir}""
  printf "${_pkgver}.%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short=7 HEAD)"
}

build() {
  export CFLAGS+=" -w"
  cd "$srcdir/daft-dhcp-client"
  make
  cd "$srcdir"
  gcc pkgdetails.c -o pkgdetails --static
}

package() {
  cd "${startdir}"
  install -m700 -vDt $pkgdir/usr/bin bpir-initrd

  cd "$srcdir/daft-dhcp-client"
  install -m755 -vT ./daft-dhcp-client $pkgdir/usr/bin/dhcpc
  cd "$srcdir"
  install -m755 -vDt "$pkgdir/usr/lib/debootstrap" pkgdetails
}
