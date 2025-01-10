# Maintainer: Eric Woudstra <ericwouds AT gmail DOT com>

#_openatf="true"      # Uncomment this line to build opensource atf

pkgname=rk3588-uboot
_pkgver=2025.01
pkgver=${_pkgver/"-"/"."}
pkgrel=1
pkgdesc='U-Boot for RK3588 Boards'
arch=('aarch64' 'x86_64')
url='https://github.com/u-boot/u-boot'
license=(GPL3)
depends=('build-rk-arch-utils-git')
makedepends=('wget' 'dtc' 'git' 'swig' 'bc' 'python3' 'python-setuptools' 'python-pyelftools')
_binsite="https://github.com/rockchip-linux/rkbin/raw"
_bincommit="b9183559cabebed120ad431a614b291fee04c498"
source=(
  "https://github.com/u-boot/u-boot/archive/refs/tags/v${_pkgver}.tar.gz"
  "src/rk3588_ddr.bin::$_binsite/$_bincommit/bin/rk35/rk3588_ddr_lp4_2112MHz_lp5_2400MHz_v1.18.bin"
)
if [[ "$_openatf" == "true" ]]; then
  # From: https://review.trustedfirmware.org/c/TF-A/trusted-firmware-a/+/21840
  source+=("atf.tar.gz::https://github.com/ARM-software/arm-trusted-firmware/archive/9244331f354af870a2f38775aaaddb47bbec7b39.tar.gz")
  noextract+=("atf.tar.gz")
else
  source+=("src/rk3588_bl31.elf::$_binsite/$_bincommit/bin/rk35/rk3588_bl31_v1.47.elf")
fi
sha256sums=(SKIP SKIP SKIP)
for p in $(shopt -s nullglob; echo *.patch) ; do
  source+=($p)
  sha256sums+=(SKIP)
done

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

  cd "${srcdir}/u-boot-${_pkgver}"
  for p in $(shopt -s nullglob; echo ../../*-uboot-*.patch) ; do
    patch -p1 -N -r - < "$p"
  done
}

build() {
  if [[ "$_openatf" == "true" ]]; then
    cd "${srcdir}/atf/"*
    touch plat/rockchip/rk3588/platform.mk
    unset CXXFLAGS CPPFLAGS LDFLAGS
    export CFLAGS=-Wno-error
    make $_crossc PLAT=rk3588
    _bl31="${srcdir}/atf/build/rk3588/release/bl31/bl31.elf"
  else
    _bl31="${srcdir}/rk3588_bl31.elf"
  fi
  _tpl="${srcdir}/rk3588_ddr.bin"

  cd "${srcdir}/u-boot-${_pkgver}"

  rm -vf *.bin.xz

  #for rkdev_conf in configs/rock5b-rk3588_defconfig; do
  for rkdev_conf in configs/sige7-rk3588_defconfig configs/rock5b-rk3588_defconfig; do
  #for rkdev_conf in configs/*-rk3588_defconfig; do
    rkdev=$(basename $rkdev_conf)
    rkdev=${rkdev/"-rk3588_defconfig"/""}
    [[ ${rkdev} == sige* ]] && rkdev="armsom-"${rkdev}
    echo ^^^ BUILDING $rkdev ^^^
    cp -vf ${rkdev_conf} configs/rk3588_my_defconfig
    cat <<-EOF | tee -a configs/rk3588_my_defconfig
	#CONFIG_DISABLE_CONSOLE=n
	#CONFIG_VIDEO_ROCKCHIP_MAX_XRES=1920
	#CONFIG_VIDEO_ROCKCHIP_MAX_YRES=1080
	EOF
    unset CFLAGS CXXFLAGS CPPFLAGS LDFLAGS
    export KCFLAGS='-Wno-error=address'
    export ARCH=aarch64
    make $_crossc rk3588_my_defconfig
    make $_crossc ROCKCHIP_TPL="$_tpl" BL31="$_bl31"
    if [ -f "idbloader.img" ] && [ -f "u-boot.itb" ]; then
      _out="u-boot-with-spl-rk3588-$rkdev.bin"
      dd if=idbloader.img of=$_out
      dd if=u-boot.itb    of=$_out seek=$((16384 - 64))
      xz --keep --force --verbose $_out
      rm -vf "idbloader.img" "u-boot.itb"
    fi
    if [ -f "u-boot-rockchip.bin" ]; then
      _out="u-boot-rockchip-rk3588-$rkdev.bin"
      cp -vf u-boot-rockchip.bin $_out
      xz --keep --force --verbose $_out
      rm -vf "u-boot-rockchip.bin"
    fi
    if [ -f "u-boot-rockchip-spi.bin" ]; then
      _out="u-boot-rockchip-spi-rk3588-$rkdev.bin"
      cp -vf u-boot-rockchip-spi.bin $_out
      xz --keep --force --verbose $_out
      rm -vf "u-boot-rockchip-spi.bin"
    fi
  done
}

package() {
  cd "${srcdir}/u-boot-${_pkgver}"
  install -vDt "$pkgdir/boot/uboot" -m644 u-boot-with-spl-rk3588-*.bin.xz
  install -vDt "$pkgdir/boot/uboot" -m644 u-boot-rockchip-rk3588-*.bin.xz
  install -vDt "$pkgdir/boot/uboot" -m644 u-boot-rockchip-spi-rk3588-*.bin.xz
}
