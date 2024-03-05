# Maintainer: Eric Woudstra <ericwouds AT gmail DOT com>

pkgname=rk3288-uboot
#last tested 2022.07, does not build anymore...
_pkgver=2023.01
pkgver=${_pkgver/"-"/"."}
pkgrel=1
pkgdesc='U-Boot for RK3288 Boards'
arch=('armv7h')
url='https://github.com/u-boot/u-boot'
license=(GPL3)
depends=('build-rk-arch-utils-git')
makedepends=('wget' 'dtc' 'git' 'swig' 'bc' 'python3' 'python-setuptools')
_binsite="https://github.com/rockchip-linux/rkbin/blob"
_bincommit="c02d94aba0d42212bf343785f080f1fc1085f947"
source=(
  "https://github.com/u-boot/u-boot/archive/refs/tags/v${_pkgver}.tar.gz"
  '040-limit_mode_to_config_max.patch'
  '050-rk-vop-no-reset.patch'
)
sha256sums=(SKIP SKIP SKIP)

build() {
  cd "${srcdir}/u-boot-${_pkgver}"
  if [ -f configs/miqi-rk3288_defconfig ]; then
    cp -vf ./configs/miqi-rk3288_defconfig configs/openhour-rk3288_defconfig
    sed -i 's/rk3288-miqi/rk3288-openhour/g' configs/openhour-rk3288_defconfig
  fi
  patch -p1 -N -r - < "${srcdir}/040-limit_mode_to_config_max.patch"
  patch -p1 -N -r - < "${srcdir}/050-rk-vop-no-reset.patch"
  #for rkdev in firefly miqi openhour phycore popmetal rock-pi-n8 tinker tinker-s vyasa; do
  #for rkdev_conf in configs/miqi-rk3288_defconfig; do
  for rkdev_conf in configs/*-rk3288_defconfig; do
    rkdev=$(basename $rkdev_conf)
    rkdev=${rkdev/"-rk3288_defconfig"/""}
    echo ^^^ BUILDING $rkdev ^^^
    cp -vf ./configs/$rkdev-rk3288_defconfig configs/rk3288_my_defconfig
    cat <<-EOT | tee -a configs/rk3288_my_defconfig
	CONFIG_VIDEO_ROCKCHIP_MAX_XRES=1920
	CONFIG_VIDEO_ROCKCHIP_MAX_YRES=1080
	CONFIG_SYS_CONSOLE_ENV_OVERWRITE=y
	CONFIG_PREBOOT="usb start;setenv stdin usbkbd"
	CONFIG_DM_KEYBOARD=y
	CONFIG_CMD_USB_MASS_STORAGE=y
	CONFIG_USB_FUNCTION_MASS_STORAGE=y
	#CONFIG_OF_LIBFDT_OVERLAY=y             ### u-boot hangs
	CONFIG_EFI_ECPT=n
	EOT
    unset CFLAGS CXXFLAGS CPPFLAGS LDFLAGS

    BINMAN_ALLOW_MISSING=1 ARCH=arm make rk3288_my_defconfig
    BINMAN_ALLOW_MISSING=1 ARCH=arm make
    if [ "$rkdev" == "miqi" ]; then # Miqi always gets processed before Openhour
      _DTS="arch/arm/dts"
      cp -vf $_DTS/rk3288-miqi.dtb $_DTS/rk3288-openhour.dtb
#   For older u-boot version use this instead:
#      fdtput -v $_DTS/rk3288-openhour.dtb /dwmmc@ff0f0000 bus-width 4
#      fdtput -v $_DTS/rk3288-openhour.dtb /dwmmc@ff0f0000 cap-sd-highspeed
#      fdtput -v $_DTS/rk3288-openhour.dtb /dwmmc@ff0f0000 card-detect-delay 200
#      fdtput -v $_DTS/rk3288-openhour.dtb /dwmmc@ff0f0000 non-removable -d
      fdtput -v $_DTS/rk3288-openhour.dtb /mmc@ff0f0000 bus-width 4
      fdtput -v $_DTS/rk3288-openhour.dtb /mmc@ff0f0000 cap-sd-highspeed
      fdtput -v $_DTS/rk3288-openhour.dtb /mmc@ff0f0000 card-detect-delay 200
      fdtput -v $_DTS/rk3288-openhour.dtb /mmc@ff0f0000 non-removable -d
    fi
    _out="u-boot-with-spl-rk3288-$rkdev.bin"
    if grep --quiet CONFIG_SPL_ROCKCHIP_BACK_TO_BROM=y ./.config; then
      ./tools/mkimage -n rk3288 -T rksd -d spl/u-boot-spl-dtb.bin $_out
      cat u-boot-dtb.bin >> $_out
    else
      dd if=idbloader.img  of=$_out
      dd if=u-boot-dtb.img of=$_out seek=$((16384 - 64))
    fi
    xz --keep --force --verbose $_out
  done
}

package() {
  cd "${srcdir}/u-boot-${_pkgver}"
  install -vDt "$pkgdir/boot/uboot" -m644 u-boot-with-spl-rk3288-*.bin.xz
}
