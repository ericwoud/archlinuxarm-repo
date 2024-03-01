# Maintainer: Eric Woudstra <ericwouds@gmail.com>

pkgname=("hostapd-launch")
url="https://github.com/ericwoud/archlinuxarm-repo/tree/hostapd-launch"
license=('GPL')
arch=('any')
pkgver=1.0
pkgrel=1
source=('hostapd-launch'
	'override.conf')
sha256sums=(SKIP SKIP)
install=${pkgname}.install
depends=('systemd')

package() {
  cd "${startdir}"
  install -m644 -vDt $pkgdir/etc/systemd/system/hostapd.service.d override.conf
  install -m755 -vDt $pkgdir/usr/bin hostapd-launch
}
