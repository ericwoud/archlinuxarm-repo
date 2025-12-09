# Maintainer: Eric Woudstra <ericwouds@gmail.com>

pkgname=("ssh-fix-reboot")
url="https://github.com/ericwoud/archlinuxarm-repo/tree/ssh-fix-reboot"
license=('GPL')
arch=('any')
pkgdesc='Fix running reboot from ssh session'
_pkgver=1
pkgver=1.4.de689d1
pkgrel=1
source=('ssh-fix-reboot.service')
sha256sums=(SKIP)
install=${pkgname}.install
depends=('systemd')

pkgver() {
  cd ""${startdir}""
  printf "${_pkgver}.%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short=7 HEAD)"
}

package() {
  cd "${startdir}"
  install -m644 -vDt $pkgdir/etc/systemd/system ssh-fix-reboot.service
}
