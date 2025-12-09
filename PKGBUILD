# Maintainer: Eric Woudstra <ericwouds@gmail.com>

pkgname=("mkinitcpio-bpir")
url="https://github.com/ericwoud/archlinuxarm-repo/tree/$pkgname"
license=('GPL')
arch=('any')
pkgdesc='Mkinitcpio configuration files for BPI Router Boards'
pkgver=5.d75fd0d
pkgrel=1
source=('mkinitcpio-bpir.conf'
        'mkinitcpio-bpir.hook'
        'mkinitcpio-bpir.build')
sha256sums=(SKIP SKIP SKIP)

pkgver() {
  cd ""${startdir}""
  printf "%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short=7 HEAD)"
}

package() {
  cd "${srcdir}"
  install -m755 -vTD ./mkinitcpio-bpir.conf  ${pkgdir}/etc/mkinitcpio-bpir.conf
  install -m755 -vTD ./mkinitcpio-bpir.hook  ${pkgdir}/etc/initcpio/hooks/bpir
  install -m755 -vTD ./mkinitcpio-bpir.build ${pkgdir}/etc/initcpio/install/bpir
}
