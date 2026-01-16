pkgname=ericwoud-keyring
pkgver=20260101
pkgrel=1
pkgdesc='Arch Linux ericwoud PGP keyring'
arch=('any')
url='http://github.com/ericwoud'
license=('GPL')
install="${pkgname}.install"
depends=('pacman')
_REPOKEY="DD73724DCA27796790D33E98798137154FE1474C"
source=("ericwoud.gpg::https://keyserver.ubuntu.com/pks/lookup?op=get&search=0x${_REPOKEY,,}")
sha256sums=('b16d570a9b165ab83b6b971e5e3ed481b82b109f61b4364fe006df5112ca38cf')

build() {
  cd "${srcdir}"
  echo "${_REPOKEY^^}:4:" >ericwoud-trusted
  touch ericwoud-revoked
  mkdir -p "${srcdir}/deb"
  cat ericwoud.gpg | grep -v -e '-----' -e ':' -e '^$' | base64 -di > deb/ericwoud.gpg 2>/dev/null || true
  [[ "$(cksum deb/ericwoud.gpg | cut -d' ' -f1)" == "2324760174" ]] && echo jaaa || return 1
}
package() {
  cd "${srcdir}"
  install -D -m0644 -t "${pkgdir}"/usr/share/pacman/keyrings/ 'ericwoud'{.gpg,-trusted,-revoked}
  cd "${srcdir}/deb"
  install -D -m0644 -t "${pkgdir}"/usr/share/keyrings/ 'ericwoud.gpg'
}
