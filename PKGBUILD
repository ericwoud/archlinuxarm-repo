# Maintainer: Eric Woudstra <ericwouds@gmail.com>

pkgname=("ssh-fix-reboot")
url="https://github.com/ericwoud/archlinuxarm-repo/tree/ssh-fix-reboot"
license=('GPL')
arch=('any')
pkgdesc='Fix running reboot from ssh session'
pkgver=21.90ac03f
pkgrel=1
source=('ssh-fix-reboot.service')
sha256sums=(SKIP)
install=${pkgname}.install
depends=('systemd')

pkgver() {
  cd ""${startdir}""
  printf "%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short=7 HEAD)"
}

package() {
  cd "${startdir}"
  install -m644 -vDt $pkgdir/etc/systemd/system ssh-fix-reboot.service
}
