# Maintainer: Eric Woudstra <ericwouds@gmail.com>

pkgname=("bpir-initrd")
url="https://github.com/ericwoud/archlinuxarm-repo/tree/$pkgname"
license=('GPL')
arch=('any')
pkgver=1.0
pkgrel=1
source=('bpir-initrd'
        'git+https://github.com/ericwoud/daft-dhcp-client.git'
        'pkgdetails.c::https://salsa.debian.org/installer-team/base-installer/-/raw/master/pkgdetails.c'
        )
sha256sums=(SKIP SKIP SKIP)
depends=('cpio' 'which')
provides=('initramfs')

build() {
  cd "$srcdir/daft-dhcp-client"
  export CFLAGS+=" -w"
  make
}

package() {
  cd "${startdir}"
  install -m700 -vDt $pkgdir/usr/bin bpir-initrd

  cd "$srcdir/daft-dhcp-client"
  install -m755 -vT ./daft-dhcp-client $pkgdir/usr/bin/dhcpc
}
