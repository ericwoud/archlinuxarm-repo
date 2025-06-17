# Maintainer: Eric Woudstra <ericwouds AT gmail DOT com>

#NOEXTRACT="1"

#mediatek distroboot see: include/configs/mt7623.h
# include/configs/<CONFIG_SYS_CONFIG_NAME>.h
# arch/<CONFIG_SYS_ARCH>/cpu/<CONFIG_SYS_CPU>
# board/<CONFIG_SYS_VENDOR>/<CONFIG_SYS_BOARD>

pkgname=bpir-uboot-git
#_pkgver=2021.10 # last build of u-boot tested for R64
#_pkgver=2023.04 # last build of u-boot tested for R3
_pkgver=2025.04
pkgver=2025.04r98055.34820924edb
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
  'mt7xxx.patch'
  'mt798x-pcie.patch'
  'clk-uclass-log_ret.patch'
)
sha256sums=(SKIP SKIP SKIP SKIP SKIP)

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
  if [[ "$_pkgver" == "2023.04" ]]; then
    patch -p1 -N -r - < "${srcdir}/clk-uclass-log_ret.patch"
  fi
  git apply "${srcdir}/mt7xxx.patch"
  git apply "${srcdir}/mt798x-pcie.patch"
  cp -vf "${srcdir}/mt7xxx.h" include/configs/
}

_buildimage() {
  _target=$1; _def=$2; _devtree=$3
  echo ^^^ BUILDING $_target ^^^
  cp -vf ./configs/$_def configs/bpir_my_defconfig
  cat <<EOT | tee -a configs/bpir_my_defconfig
CONFIG_AUTOBOOT=y
CONFIG_BOOTDELAY=1
CONFIG_DEFAULT_DEVICE_TREE="${_devtree}"
CONFIG_DEFAULT_FDT_FILE="${_devtree}"
CONFIG_CMD_EXT4=y
CONFIG_CMD_SETEXPR=y
CONFIG_HUSH_PARSER=y
CONFIG_EFI_PARTITION=y
CONFIG_ENV_IS_IN_MMC=n
CONFIG_DISTRO_DEFAULTS=y
CONFIG_CMD_PWM=y
CONFIG_DM_PWM=y
CONFIG_PWM_MTK=y
CONFIG_CMD_MTD=y
CONFIG_MTD=y
CONFIG_DM_MTD=y
CONFIG_MTD_UBI=y
CONFIG_CMD_UBI=y
CONFIG_CMD_UBIFS=y
CONFIG_MTD_SPI_NAND=y
CONFIG_DM_SPI_FLASH=y
CONFIG_SPI_FLASH_SFDP_SUPPORT=y
CONFIG_SPI_FLASH_EON=y
CONFIG_SPI_FLASH_GIGADEVICE=y
CONFIG_SPI_FLASH_ISSI=y
CONFIG_SPI_FLASH_MACRONIX=y
CONFIG_SPI_FLASH_SPANSION=y
CONFIG_SPI_FLASH_STMICRO=y
CONFIG_SPI_FLASH_WINBOND=y
CONFIG_SPI_FLASH_XMC=y
CONFIG_SPI_FLASH_XTX=y
CONFIG_SPI_FLASH_MTD=y
CONFIG_SPI=y
CONFIG_DM_SPI=y
CONFIG_MTK_SPIM=y
CONFIG_PCI=y
CONFIG_PCIE_MEDIATEK_GEN3=y
CONFIG_CMD_PCI=y
CONFIG_NVME_PCI=y
CONFIG_NVME=y
CONFIG_BOOTCOMMAND="mmc rescan; run distro_bootcmd;"
EOT
  unset CFLAGS CXXFLAGS CPPFLAGS LDFLAGS
  ARCH=arm64 make bpir_my_defconfig
#  export KCFLAGS='-Wno-error=address'
  ARCH=arm64 make u-boot.bin
  cp u-boot.bin u-boot-${_target}.bin
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
