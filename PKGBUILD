# Maintainer: Eric Woudstra <ericwouds@gmail.com>

pkgname=("linux-firmware-brcm43752")
url="https://forum.banana-pi.org/t/bpi-m7-archlinuxarm-build-script/17794/21"
license=('GPL')
arch=('any')
pkgver=r217.dc2d3b1
pkgrel=1
source=("git+https://github.com/armbian/firmware.git")
sha256sums=(SKIP)

pkgver() {
  cd "${srcdir}/firmware"
  printf "r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}

package() {
  cd "${srcdir}/firmware/ap6275p"
  _bcrm="$pkgdir/usr/lib/firmware/brcm"
  install -m755 -vDT fw_bcm43752a2_pcie_ag.bin   $_bcrm/brcmfmac43752-pcie.bin
  install -m755 -vDT clm_bcm43752a2_pcie_ag.blob $_bcrm/brcmfmac43752-pcie.clm_blob
  install -m755 -vDT nvram_AP6275P.txt           $_bcrm/brcmfmac43752-pcie.txt
}
