# Maintainer: Eric Woudstra <ericwouds AT gmail DOT com>

pkgname=rk3588-uboot-git
_pkgver=2022.07
pkgver=2017.09r52761.75b12f8295
pkgrel=2
pkgdesc='U-Boot for RK3588 Boards'
arch=('aarch64')
url='https://github.com/radxa/u-boot'
license=(GPL3)
depends=('build-rk-arch-utils-git')
makedepends=('wget' 'dtc' 'git' 'swig' 'bc' 'python3' 'python-setuptools')
#_binsite="https://github.com/rockchip-linux/rkbin/raw"
#_bincommit="6e4ecc820bf15d47f06e67b5ddb2eab224e485b6"
_binsite="https://github.com/radxa/rkbin/raw"
_bincommit="9840e87723eef7c41235b89af8c049c1bcd3d133"
source=(
  "git+https://github.com/radxa/u-boot.git#branch=stable-5.10-rock5"
  "src/rk3588_ddr.bin::$_binsite/$_bincommit/bin/rk35/rk3588_ddr_lp4_2112MHz_lp5_2736MHz_v1.08.bin"
  "src/rk3588_bl31.elf::$_binsite/$_bincommit/bin/rk35/rk3588_bl31_v1.28.elf"
)
sha256sums=(SKIP SKIP SKIP)

pkgver() {
  cd "${srcdir}/u-boot"
  _year=$(grep '^VERSION = ' Makefile | cut -b 11-)
  _month=$(grep '^PATCHLEVEL = ' Makefile | cut -b 14-)
  printf "%s.%sr%s.%s" "$_year" "$_month" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}

build() {
  cd "${srcdir}/u-boot"
  sed -i '1c#!/usr/bin/env python3' arch/arm/mach-rockchip/decode_bl31.py
  #for rkdev in rock-5b; do
  for rkdev_conf in configs/*-rk3588_defconfig; do
    rkdev=$(basename $rkdev_conf)
    rkdev=${rkdev/"-rk3588_defconfig"/""}
    echo ^^^ BUILDING $rkdev ^^^
    cp -vf ./configs/$rkdev-rk3588_defconfig configs/rk3588_my_defconfig
    cat <<EOT | tee -a configs/rk3588_my_defconfig
CONFIG_DISABLE_CONSOLE=n
##CONFIG_SYS_CONSOLE_ENV_OVERWRITE=y
##CONFIG_PREBOOT="usb start;setenv stdin usbkbd"
##CONFIG_DM_KEYBOARD=y
##CONFIG_CMD_USB_MASS_STORAGE=y
##CONFIG_USB_FUNCTION_MASS_STORAGE=y
###CONFIG_OF_LIBFDT_OVERLAY=y
##CONFIG_EFI_ECPT=n
EOT
    unset CFLAGS CXXFLAGS CPPFLAGS LDFLAGS

    ARCH=aarch64 make rk3588_my_defconfig
    export KCFLAGS='-Wno-error=address'
    ARCH=aarch64 make BL31="${srcdir}/rk3588_bl31.elf" spl/u-boot-spl.bin u-boot.dtb u-boot.itb
    tools/mkimage -n rk3588 -T rksd -d ../rk3588_ddr.bin:spl/u-boot-spl.bin idbloader.img
    _out="u-boot-with-spl-rk3588-$rkdev.bin"
    dd if=idbloader.img of=$_out
    dd if=u-boot.itb    of=$_out seek=$((16384 - 64))
    xz --keep --force --verbose $_out
  done
}

package() {
  cd "${srcdir}/u-boot"
  install -vDt "$pkgdir/boot/uboot" -m644 u-boot-with-spl-rk3588-*.bin.xz
}
