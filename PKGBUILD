# Maintainer: Eric Woudstra <ericwouds AT gmail DOT com>

pkgname=rk3588-uboot
#last tested 2022.07, does not build anymore...
_pkgver=2024.04-rc3
pkgver=${_pkgver/"-"/"."}
pkgrel=1
pkgdesc='U-Boot for RK3588 Boards'
arch=('aarch64' 'x86_64')
url='https://github.com/u-boot/u-boot'
license=(GPL3)
#depends=('build-rk-arch-utils-git')
makedepends=('wget' 'dtc' 'git' 'swig' 'bc' 'python3' 'python-setuptools' 'python-pyelftools')
_binsite="https://github.com/rockchip-linux/rkbin/raw"
_bincommit="f02d10e468d8c783c45137d230ff33d42ca670b4"
#_binsite="https://github.com/radxa/rkbin/raw"
#_bincommit="9840e87723eef7c41235b89af8c049c1bcd3d133"
source=(
  "https://github.com/u-boot/u-boot/archive/refs/tags/v${_pkgver}.tar.gz"
  "src/rk3588_ddr.bin::$_binsite/$_bincommit/bin/rk35/rk3588_ddr_lp4_2112MHz_lp5_2736MHz_eyescan_v1.11.bin"
  "src/rk3588_bl31.elf::$_binsite/$_bincommit/bin/rk35/rk3588_bl31_v1.45.elf"
)
sha256sums=(SKIP SKIP SKIP)

export CARCH=aarch64
if [[ "$(uname -m)" != "aarch64" ]]; then
  makedepends+=(aarch64-linux-gnu-gcc)
  export _crossc="CROSS_COMPILE=aarch64-linux-gnu-"
fi

build() {
  cd "${srcdir}/u-boot-${_pkgver}"
#  patch -p1 -N -r - < "${srcdir}/040-limit_mode_to_config_max.patch"
#  patch -p1 -N -r - < "${srcdir}/050-rk-vop-no-reset.patch"
  #for rkdev in firefly miqi openhour phycore popmetal rock-pi-n8 tinker tinker-s vyasa; do
  for rkdev_conf in configs/rock5b-rk3588_defconfig; do
  #for rkdev_conf in configs/*-rk3588_defconfig; do
    rkdev=$(basename $rkdev_conf)
    rkdev=${rkdev/"-rk3588_defconfig"/""}
    echo ^^^ BUILDING $rkdev ^^^
    cp -vf ./configs/$rkdev-rk3588_defconfig configs/rk3588_my_defconfig
    cat <<EOT | tee -a configs/rk3588_my_defconfig
#CONFIG_DISABLE_CONSOLE=n
#CONFIG_VIDEO_ROCKCHIP_MAX_XRES=1920
#CONFIG_VIDEO_ROCKCHIP_MAX_YRES=1080
#CONFIG_SYS_CONSOLE_ENV_OVERWRITE=y
#CONFIG_PREBOOT="usb start;setenv stdin usbkbd"
#CONFIG_DM_KEYBOARD=y
#CONFIG_CMD_USB_MASS_STORAGE=y
#CONFIG_USB_FUNCTION_MASS_STORAGE=y
##CONFIG_OF_LIBFDT_OVERLAY=y             ### u-boot hangs
#CONFIG_EFI_ECPT=n
EOT
    unset CFLAGS CXXFLAGS CPPFLAGS LDFLAGS
    export KCFLAGS='-Wno-error=address'
    export ARCH=aarch64

    make $_crossc rk3588_my_defconfig
    make $_crossc ROCKCHIP_TPL="${srcdir}/rk3588_ddr.bin" BL31="${srcdir}/rk3588_bl31.elf"
    _out="u-boot-with-spl-rk3588-$rkdev.bin"
    dd if=idbloader.img of=$_out
    dd if=u-boot.itb    of=$_out seek=$((16384 - 64))
    xz --keep --force --verbose $_out
  done
}

package() {
  cd "${srcdir}/u-boot-${_pkgver}"
  install -vDt "$pkgdir/boot/uboot" -m644 u-boot-with-spl-rk3588-*.bin.xz
}
