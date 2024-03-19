# Maintainer: Eric Woudstra <ericwouds AT gmail DOT com>

#_openatf="true"      # Uncomment this line to build opensource atf

pkgname=rk3588-uboot
#last tested 2022.07, does not build anymore...
_pkgver=2024.04-rc3
pkgver=${_pkgver/"-"/"."}
pkgrel=1
pkgdesc='U-Boot for RK3588 Boards'
arch=('aarch64' 'x86_64')
url='https://github.com/u-boot/u-boot'
license=(GPL3)
depends=('build-rk-arch-utils-git')
makedepends=('wget' 'dtc' 'git' 'swig' 'bc' 'python3' 'python-setuptools' 'python-pyelftools')
_binsite="https://github.com/rockchip-linux/rkbin/raw"
_bincommit="f02d10e468d8c783c45137d230ff33d42ca670b4"
source=(
  "https://github.com/u-boot/u-boot/archive/refs/tags/v${_pkgver}.tar.gz"
  "src/rk3588_ddr.bin::$_binsite/$_bincommit/bin/rk35/rk3588_ddr_lp4_2112MHz_lp5_2736MHz_eyescan_v1.11.bin"
  "src/rk3588-bpi-m7.dts::https://raw.githubusercontent.com/ericwoud/archlinuxarm-repo/linux-aarch64-rk-rc/rk3588-bpi-m7.dts"
  "rk3588-bpi-m7-u-boot.dtsi"
)
if [[ "$_openatf" == "true" ]]; then
  # From: https://review.trustedfirmware.org/c/TF-A/trusted-firmware-a/+/21840
  source+=("atf.tar.gz::https://review.trustedfirmware.org/changes/TF-A%2Ftrusted-firmware-a~21840/revisions/7/archive?format=tgz")
  noextract+=("atf.tar.gz")
else
  source+=("src/rk3588_bl31.elf::$_binsite/$_bincommit/bin/rk35/rk3588_bl31_v1.45.elf")
fi
sha256sums=(SKIP SKIP SKIP SKIP SKIP)

export CARCH=aarch64
if [[ "$(uname -m)" != "aarch64" ]]; then
  makedepends+=(aarch64-linux-gnu-gcc)
  pacman -Qt "${makedepends[@]}" >/dev/null
  [[ $? != 0 ]] && exit # Manually check for makedepends only
  export NODEPS=1
  export _crossc="CROSS_COMPILE=aarch64-linux-gnu-"
fi

prepare() {
  if [[ "$_openatf" == "true" ]]; then
    mkdir -p "${srcdir}/atf"
    bsdtar -x -f "${startdir}/atf.tar.gz" -C "${srcdir}/atf"
  fi
}

build() {
  if [[ "$_openatf" == "true" ]]; then
    cd "${srcdir}/atf"
    touch plat/rockchip/rk3588/platform.mk
    unset CXXFLAGS CPPFLAGS LDFLAGS
    export CFLAGS=-Wno-error
    make $_crossc PLAT=rk3588
    _bl31="${srcdir}/atf/build/rk3588/release/bl31/bl31.elf"
  else
    _bl31="${srcdir}/rk3588_bl31.elf"
  fi

  cd "${srcdir}/u-boot-${_pkgver}"
#  patch -p1 -N -r - < "${srcdir}/040-blabla.patch"

  if [ -z "$(grep "rk3588-bpi-m7.dtb" arch/arm/dts/Makefile)" ]; then
    sed -i 's/dtb-$(CONFIG_ROCKCHIP_RK3588) +=.*/dtb-$(CONFIG_ROCKCHIP_RK3588) += \\\n\trk3588-bpi-m7.dtb \\/' \
               arch/arm/dts/Makefile
  fi	
  cp -vf ../rk3588-bpi-m7.dts arch/arm/dts
  cp -vf ../rk3588-bpi-m7-u-boot.dtsi arch/arm/dts

  cp -vf configs/rock5b-rk3588_defconfig configs/bpi-m7-rk3588_defconfig
  sed -i 's/CONFIG_TARGET_ROCK5B_RK3588=y/CONFIG_TARGET_EVB_RK3588=y/g' \
             configs/bpi-m7-rk3588_defconfig
  sed -i 's/rock-5b/bpi-m7/g' \
             configs/bpi-m7-rk3588_defconfig

  #for rkdev in firefly miqi openhour phycore popmetal rock-pi-n8 tinker tinker-s vyasa; do
  #for rkdev_conf in configs/rock5b-rk3588_defconfig; do
  for rkdev_conf in configs/bpi-m7-rk3588_defconfig configs/rock5b-rk3588_defconfig; do
  #for rkdev_conf in configs/*-rk3588_defconfig; do
    rkdev=$(basename $rkdev_conf)
    rkdev=${rkdev/"-rk3588_defconfig"/""}
    echo ^^^ BUILDING $rkdev ^^^
    cp -vf ./configs/$rkdev-rk3588_defconfig configs/rk3588_my_defconfig
    cat <<-EOF | tee -a configs/rk3588_my_defconfig
	#CONFIG_DISABLE_CONSOLE=n
	#CONFIG_VIDEO_ROCKCHIP_MAX_XRES=1920
	#CONFIG_VIDEO_ROCKCHIP_MAX_YRES=1080
	EOF
    unset CFLAGS CXXFLAGS CPPFLAGS LDFLAGS
    export KCFLAGS='-Wno-error=address'
    export ARCH=aarch64
    make $_crossc rk3588_my_defconfig
    make $_crossc ROCKCHIP_TPL="${srcdir}/rk3588_ddr.bin" BL31="$_bl31"
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
