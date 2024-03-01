# Maintainer: Eric Woudstra <ericwouds@gmail.com>

pkgname=("ssh-fix-reboot")
url="https://github.com/ericwoud/archlinuxarm-repo/tree/ssh-fix-reboot"
license=('GPL')
arch=('any')
pkgver=1.0
pkgrel=1
source=('ssh-fix-reboot.service')
sha256sums=(SKIP)
install=${pkgname}.install

package() {
  cd "${startdir}"
  install -m644 -vDt $pkgdir/etc/systemd/system ssh-fix-reboot.service
}
