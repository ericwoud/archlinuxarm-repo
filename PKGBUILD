# Maintainer: Eric Woudstra <ericwouds AT gmail DOT com>

#NOEXTRACT="1"

#mediatek distroboot see: include/configs/mt7623.h
# include/configs/<CONFIG_SYS_CONFIG_NAME>.h
# arch/<CONFIG_SYS_ARCH>/cpu/<CONFIG_SYS_CPU>
# board/<CONFIG_SYS_VENDOR>/<CONFIG_SYS_BOARD>

pkgname=bpir-uboot-git
_pkgver=2025.10
pkgver=2025.10r101557.e50b1e87150
pkgrel=1
pkgdesc='U-Boot for BPI Router Boards'
arch=('aarch64' 'x86_64')
url='https://github.com/u-boot/u-boot'
license=(GPL3)
depends=()
makedepends=('git' 'bc')
source=(
  "git+https://github.com/u-boot/u-boot.git#tag=v${_pkgver}"
  'mt7xxx.h'
  'append_defconfig'
)
sha256sums=(SKIP SKIP SKIP)
for p in $(shopt -s nullglob; echo *.patch) ; do
  source+=($p)
  sha256sums+=(SKIP)
done

export CARCH=aarch64
if [[ "$(uname -m)" != "aarch64" ]]; then
  makedepends+=(aarch64-linux-gnu-gcc)
  export CROSS_COMPILE=aarch64-linux-gnu-
fi

pkgver() {
  cd "${srcdir}/u-boot"
  _year=$(grep '^VERSION = ' Makefile | cut -b 11-)
  _month=$(grep '^PATCHLEVEL = ' Makefile | cut -b 14-)
  printf "%s.%sr%s.%s" "$_year" "$_month" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}

prepare() {
  cd "${srcdir}/u-boot"
  git apply --verbose ../../*.patch
  cp -vf "${srcdir}/mt7xxx.h" include/configs/
}

_buildimage() {
  _target=$1; _def=$2; _devtree=$3
  echo ^^^ BUILDING $_target ^^^
  cp -vf ./configs/$_def configs/bpir_my_defconfig
  cat <<-EOF | tee -a configs/bpir_my_defconfig
CONFIG_DEFAULT_DEVICE_TREE="${_devtree}"
CONFIG_DEFAULT_FDT_FILE="${_devtree}"
EOF
  cat "${srcdir}/append_defconfig" >> configs/bpir_my_defconfig
  unset CFLAGS CXXFLAGS CPPFLAGS LDFLAGS
  ARCH=arm64 make bpir_my_defconfig
#  export KCFLAGS='-Wno-error=address'
  ARCH=arm64 make u-boot.bin
  cp u-boot.bin u-boot-${_target}.bin
  mv .config config-${_target}.txt
}

build() {
  cd "${srcdir}/u-boot"
  rm -f u-boot-bpir*.bin
  _buildimage bpir64       mt7622_rfb_defconfig          mt7622-bananapi-bpi-r64
  _buildimage bpir3-emmc   mt7986a_bpir3_emmc_defconfig  mt7986a-bpi-r3-emmc
  _buildimage bpir3-sdmmc  mt7986a_bpir3_sd_defconfig    mt7986a-bpi-r3-sd
  _buildimage bpir4-emmc   mt7988_rfb_defconfig          mt7988-rfb
  _buildimage bpir4-sdmmc  mt7988_sd_rfb_defconfig       mt7988-sd-rfb
}

package() {
  cd "${srcdir}/u-boot"
  _pd="$pkgdir/usr/share/bpir-uboot"
  install -vDt "${_pd}/" -m644 u-boot-bpir*.bin
  ln -srf "${_pd}/u-boot-bpir3-emmc.bin" "${_pd}/u-boot-bpir3.bin"
  ln -srf "${_pd}/u-boot-bpir4-emmc.bin" "${_pd}/u-boot-bpir4.bin"
}
