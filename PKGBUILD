# Maintainer: Eric Woudstra <ericwouds@gmail.com>

pkgname=("hostapd-launch")
url="https://github.com/ericwoud/archlinuxarm-repo/tree/hostapd-launch"
license=('GPL')
arch=('any')
pkgdesc='Launch helper for hostapd'
pkgver=18.2758cdd
pkgrel=1
source=('hostapd-launch'
	'override.conf'
	'overrideat.conf')
sha256sums=(SKIP SKIP SKIP)
install=${pkgname}.install
depends=('systemd')

pkgver() {
  cd ""${startdir}""
  printf "%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short=7 HEAD)"
}

package() {
  cd "${startdir}"
  install -m644 -vDt $pkgdir/etc/systemd/system/hostapd.service.d override.conf
  install -m644 -vDt $pkgdir/etc/systemd/system/hostapd@.service.d overrideat.conf
  install -m755 -vDt $pkgdir/usr/bin hostapd-launch
}
