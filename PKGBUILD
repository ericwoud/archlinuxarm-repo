# Maintainer: Eric Woudstra <ericwouds AT gmail DOT com>

#NOEXTRACT="1"

#mediatek distroboot see: include/configs/mt7623.h
# include/configs/<CONFIG_SYS_CONFIG_NAME>.h
# arch/<CONFIG_SYS_ARCH>/cpu/<CONFIG_SYS_CPU>
# board/<CONFIG_SYS_VENDOR>/<CONFIG_SYS_BOARD>

pkgbase=bpir-uboot-git
_pkgver=2026.01
pkgver=2026.01r80.103050.127a42c7257
pkgrel=1
pkgdesc='U-Boot for BPI Router Boards'
arch=('aarch64' 'x86_64')
url='https://github.com/u-boot/u-boot'
_openwrtver=v24.10.5
_openwrturl="https://github.com/openwrt/openwrt/raw/refs/tags/${_openwrtver}/package/boot/uboot-mediatek/patches"
license=(GPL3)
depends=()
makedepends=('git' 'bc')
source=(
  "git+https://github.com/u-boot/u-boot.git#tag=v${_pkgver}"
  "src/snand.patch::${_openwrturl}/100-02-drivers-mtd-add-support-for-MediaTek-SPI-NAND-flash-.patch"
  "src/mt7622.patch::${_openwrturl}/100-19-board-mt7622-use-new-spi-nand-driver.patch"
  "src/r64dts1.patch::${_openwrturl}/402-update-bananapi-bpi-r64-device-tree.patch"
  "src/r64dts2.patch::${_openwrturl}/403-add-bananapi_bpi-r64-snand.patch"
  "src/mt7986a-bpi-r3-mini.dts::https://github.com/frank-w/u-boot/raw/refs/heads/${_pkgver/./-}-bpi/arch/arm/dts/mt7986a-bpi-r3-mini.dts"
  'mt7xxx.h'
  'append_defconfig'
)
sha256sums=(SKIP SKIP SKIP SKIP SKIP SKIP SKIP SKIP)
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
  printf "%s.%sr%s.%s.%s" "$_year" "$_month" \
                       "$(git -C "${startdir}" rev-list --count HEAD)" \
                       "$(git rev-list --count HEAD)" \
                       "$(git rev-parse --short HEAD)"
}

prepare() {
  cd "${srcdir}/u-boot"
  git clean -d -f
  cp -vf arch/arm/dts/mt7988-rfb.dts arch/arm/dts/mt7988-rfb-pro.dts
  cp -vf arch/arm/dts/mt7988-sd-rfb.dts arch/arm/dts/mt7988-sd-rfb-pro.dts
  git apply --verbose ../*.patch
  cp -vf "${srcdir}/mt7xxx.h" include/configs/
  cp -vf "${srcdir}/"*".dts" arch/arm/dts/
}

_buildimage() {
  _target=$1; _def=$2; _devtree=$3
  _maintarget=$(echo ${_target} | cut -d- -f1)
  echo ^^^ BUILDING $_target ^^^
  cp -vf ./configs/$_def configs/bpir_my_defconfig
  (
    echo 'CONFIG_DEFAULT_DEVICE_TREE="'${_devtree}'"'
    echo 'CONFIG_DEFAULT_FDT_FILE="'${_devtree}'"'
    cat "${srcdir}/append_defconfig"
    if [[ ${_target} == "bpir64" ]]; then
      echo 'CONFIG_MTD_SPI_NAND=n'
      echo 'CONFIG_MTK_SPI_NAND=y'
      echo 'CONFIG_MTK_SPI_NAND_MTD=y'
    else
      echo 'CONFIG_MTD_SPI_NAND=y'
      echo 'CONFIG_MTK_SPI_NAND=n'
      echo 'CONFIG_MTK_SPI_NAND_MTD=n'
    fi
  ) | tee -a configs/bpir_my_defconfig
  unset CFLAGS CXXFLAGS CPPFLAGS LDFLAGS
  ARCH=arm64 make bpir_my_defconfig
#  export KCFLAGS='-Wno-error=address'
  ARCH=arm64 make u-boot.bin
  mkdir -p ${_maintarget}
  cp -vf u-boot.bin ${_maintarget}/u-boot-${_target}.bin
  mv -vf .config config-${_target}.txt
}

build() {
  cd "${srcdir}/u-boot"
  rm -f bpir*/u-boot-bpir*.bin
  _buildimage bpir64         mt7622_rfb_defconfig          mt7622-bananapi-bpi-r64
  _buildimage bpir3-emmc     mt7986a_bpir3_emmc_defconfig  mt7986a-bpi-r3-emmc
  _buildimage bpir3-sdmmc    mt7986a_bpir3_sd_defconfig    mt7986a-bpi-r3-sd
  _buildimage bpir3m         mt7986a_bpir3_emmc_defconfig  mt7986a-bpi-r3-mini
  _buildimage bpir4m         mt7987_emmc_rfb_defconfig     mt7987a-emmc-rfb
  _buildimage bpir4l-emmc    mt7987_emmc_rfb_defconfig     mt7987a-emmc-rfb
  _buildimage bpir4l-sdmmc   mt7987_sd_rfb_defconfig       mt7987a-sd-rfb
  _buildimage bpir4-emmc     mt7988_rfb_defconfig          mt7988-rfb
  _buildimage bpir4-sdmmc    mt7988_sd_rfb_defconfig       mt7988-sd-rfb
  _buildimage bpir4p4e-emmc  mt7988_rfb_defconfig          mt7988-rfb-pro
  _buildimage bpir4p4e-sdmmc mt7988_sd_rfb_defconfig       mt7988-sd-rfb-pro
  _buildimage bpir4p8x-emmc  mt7988_rfb_defconfig          mt7988-rfb-pro
  _buildimage bpir4p8x-sdmmc mt7988_sd_rfb_defconfig       mt7988-sd-rfb-pro
  #               Filename                           Linkname
  ln -srf "bpir64/u-boot-bpir64.bin"         "bpir64/u-boot-bpir64-emmc.bin"
  ln -srf "bpir64/u-boot-bpir64.bin"         "bpir64/u-boot-bpir64-sdmmc.bin"
  ln -srf "bpir3/u-boot-bpir3-emmc.bin"      "bpir3/u-boot-bpir3.bin"
  ln -srf "bpir3m/u-boot-bpir3m.bin"         "bpir3m/u-boot-bpir3m-emmc.bin"
  ln -srf "bpir4m/u-boot-bpir4m.bin"         "bpir3m/u-boot-bpir4m-emmc.bin"
  ln -srf "bpir4l/u-boot-bpir4l-emmc.bin"    "bpir4l/u-boot-bpir4l.bin"
  ln -srf "bpir4/u-boot-bpir4-emmc.bin"      "bpir4/u-boot-bpir4.bin"
  ln -srf "bpir4/u-boot-bpir4p4e-emmc.bin"   "bpir4/u-boot-bpir4p4e.bin"
  ln -srf "bpir4/u-boot-bpir4p8x-emmc.bin"   "bpir4/u-boot-bpir4p8x.bin"
}

_package() {
  pkgdesc="U-Boot $1 images"
  replaces=('bpir-uboot-git')
  cd "${srcdir}/u-boot/$1"
  _pd="$pkgdir/usr/share/bpir-uboot"
  mkdir -p "${_pd}"
  cp -vdf *.bin "${_pd}"
}

for _target in bpir64 bpir3 bpir3m bpir4l bpir4 bpir4p4e bpir4p8x; do
  eval "package_${_target}-uboot-git() { _package ${_target} ; }"
  _packages+=(${_target}-uboot-git)
  pkgname+=(${_target}-uboot-git)
done

