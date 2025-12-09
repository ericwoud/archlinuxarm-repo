# Maintainer: Eric Woudstra ericwouds AT gmail DOT com

#NOEXTRACT="1"

_gitname="arm-trusted-firmware"
_gitroot=https://github.com/ericwoud/${_gitname}
#_gitroot=https://github.com/mtk-openwrt/${_gitname}
#_gitbranch="wip"
_gitbranch="bpir"
#_gitbranch="master"
#_gitbranch="mtksoc"
pkgbase=bpir-atf-git
pkgname=("$pkgbase")
epoch=2
pkgver=v2.12r1.16266.09661111e
pkgrel=1
_ubootpkgver=2023.01
url='https://github.com/mtk-openwrt/arm-trusted-firmware.git'
arch=(aarch64 x86_64)
depends=()
makedepends=(git dtc)
license=(GPL)
source=("git+${_gitroot}.git#branch=${_gitbranch}"
        "uboot${_ubootpkgver}::https://github.com/u-boot/u-boot/archive/refs/tags/v${_ubootpkgver}.tar.gz"
        'mtkimage-gpt-expand.patch'
)
sha256sums=(SKIP SKIP SKIP)

export CARCH=aarch64
if [[ "$(uname -m)" == "aarch64" ]]; then
  pkgname+=("$pkgbase-fiptool")
  export CC="gcc"
else
  makedepends+=(aarch64-linux-gnu-gcc)
  export _crossc="CROSS_COMPILE=aarch64-linux-gnu-"
  export CC="aarch64-linux-gnu-gcc"
fi
 
pkgver() {
  cd "${srcdir}/${_gitname}"
  printf "v%s.%sr%s.%s.%s" \
      $(grep '^VERSION_MAJOR' Makefile | cut -b 19-) \
      $(grep '^VERSION_MINOR' Makefile | cut -b 19-) \
      "$(git -C "${startdir}" rev-list --count HEAD)"
      "$(git rev-list --count HEAD)" \
      "$(git rev-parse --short HEAD)"
}

prepare() {
  cd "${srcdir}/${_gitname}"
#  git config --global user.email "you@example.com"
#  git config --global user.name "Your Name"
#  rm -rf ${srcdir}/${_gitname}/.git/rebase-apply  
#  echo -n -e "\n\nCOMMIT DATE:"
#  git log -1 --format="%at" | xargs -I{} date -d @{} +%Y/%m/%d-%H:%M:%S
#  echo -e "\n\n"
#  git am --ignore-space-change --ignore-whitespace "${startdir}/"*.patch
}

_buildmkimage() {
  cd "${srcdir}/u-boot-${_ubootpkgver}"
  cp -vf configs/mt7622_rfb_defconfig configs/my_defconfig
  # No libcrypto (openssl)
  echo "CONFIG_TOOLS_LIBCRYPTO=n" | tee -a configs/my_defconfig
  # kwbimage needs libcrypto, so remove it from build
  sed -i '/kwbimage.o \\/d' ./tools/Makefile
  # reduce objects being build, as they are not linked anyway
  sed -i 's/FIT_OBJS-y :=.*/FIT_OBJS-y := fit_common.o boot\/image-fit.o/' ./tools/Makefile
  ARCH=arm64 make clean
  ARCH=arm64 make my_defconfig
  ARCH=arm64 make tools-only
  mv -vf tools/mkimage nostretch-mkimage
  patch -p1 -N -r - < "${srcdir}/mtkimage-gpt-expand.patch"
  ARCH=arm64 make tools-only
  mv -vf tools/mkimage stretch-mkimage
}

_buildfiptool() {
  cd "${srcdir}/${_gitname}/tools/fiptool"
  sed -i '/-Werror/d' ./Makefile
  make HOSTCCFLAGS+="-D'SHA256(x,y,z)=nop'" LDOPTS="-static"
}

_use_extended_partlabel() {
  _file="plat/mediatek/apsoc_common/bl2/bl2_boot_mmc.c"
  [ -f "$_file" ] || _file="plat/mediatek/${_plat}/bl2_boot_mmc.c"
  _match='.*dev_handle = fill_io_block_spec_gpt.*mmc_dev_fip_spec.*fip.*'
   _new='\t*dev_handle = fill_io_block_spec_gpt(\&mmc_dev_fip_spec, "'${_bpir}'-'${_atfdev}'-fip");'
  sed -i 's/'"${_match}"'/'"${_new}"'/g' $_file
  _match='.*entry = get_partition_entry.*fip.*'
    _new='\tentry = get_partition_entry("'${_bpir}'-'${_atfdev}'-fip");'
  sed -i 's/'"${_match}"'/'"${_new}"'/g' $_file
  _match='.*entry = get_partition_entry.*boot.*'
    _new='\tentry = get_partition_entry("'${_bpir}'-'${_atfdev}'-boot");'
  sed -i 's/'"${_match}"'/'"${_new}"'/g' $_file
}

_buildimage() {
  _plat=$1; _bpir=$2; _atfdev=$3; _stretch=$4; _ddrsize=$5; _options="${@:6}"
  cd "${srcdir}/${_gitname}"
  [ -n "${EXTENDED_PARTLABEL}" ] && _use_extended_partlabel
  touch plat/mediatek/${_plat}/platform.mk
  unset CXXFLAGS CPPFLAGS LDFLAGS
  export CFLAGS=-Wno-error
  [[ "$_ddrsize" != "-" ]] && _extra="-${_ddrsize}gb" || _extra=""
  make $_crossc PLAT=${_plat} BOOT_DEVICE=$_atfdev LOG_LEVEL=40 USE_MKIMAGE=1 \
       BUILD_STRING="Alarm ATF ${_bpir^^} ${_atfdev^^} ${pkgver}" \
       MKIMAGE="${srcdir}/u-boot-${_ubootpkgver}/${_stretch}-mkimage" ${_options} all # MTK_BL33_IS_64BIT=1
  if [[ "${_atfdev}" == "ram" ]]; then
    dd of=build/${_plat}/release/${_bpir}-atf-${_atfdev}-atf${_extra}.bin                   if=build/${_plat}/release/bl2.bin
  elif [[ "${_stretch}" == "stretch" ]]; then
    dd of=build/${_plat}/release/${_bpir}-atf-${_atfdev}-header${_extra}.bin bs=1 count=440 if=build/${_plat}/release/bl2.img
    dd of=build/${_plat}/release/${_bpir}-atf-${_atfdev}-atf${_extra}.bin          skip=34  if=build/${_plat}/release/bl2.img
  else
    dd of=build/${_plat}/release/${_bpir}-atf-${_atfdev}-atf${_extra}.bin                   if=build/${_plat}/release/bl2.img
  fi
  if [ -z "$(cat bl2/bl2.mk | grep bl31.bin.o)" ]; then # bl31.bin is not being build in, so add it
    dd of=build/${_plat}/release/${_bpir}-atf-${_atfdev}-bl31${_extra}.bin                  if=build/${_plat}/release/bl31.bin
  fi
}

build() {
  rm -rf "${srcdir}/${_gitname}"/build/*
  echo "$(uname -m)"
  if [ ! -f "${srcdir}/u-boot-${_ubootpkgver}/nostretch-mkimage"  ] || \
     [ ! -f "${srcdir}/u-boot-${_ubootpkgver}/stretch-mkimage" ]; then _buildmkimage
  fi
  [ -z "$_crossc" ] && _buildfiptool
  _buildimage mt7622 bpir64 sdmmc     stretch   - DDR3_FLYBY=1 DEVICE_HEADER_OFFSET=0
  _buildimage mt7622 bpir64 emmc      stretch   - DDR3_FLYBY=1 DEVICE_HEADER_OFFSET=0
  _buildimage mt7622 bpir64 snand     nostretch - DDR3_FLYBY=1 UBI=1 OVERRIDE_UBI_START_ADDR=0x280000
  _buildimage mt7986 bpir64 ram       nostretch - DDR3_FLYBY=1 RAM_BOOT_UART_DL=1
  _buildimage mt7986 bpir3  sdmmc     nostretch - DRAM_USE_DDR4=1
  _buildimage mt7986 bpir3  emmc      nostretch - DRAM_USE_DDR4=1 BROM_HEADER_TYPE=sdmmc
  _buildimage mt7986 bpir3  ram       nostretch - DRAM_USE_DDR4=1 RAM_BOOT_UART_DL=1
  _buildimage mt7986 bpir3  spim-nand nostretch - DRAM_USE_DDR4=1 UBI=1 OVERRIDE_UBI_START_ADDR=0x580000
  _buildimage mt7986 bpir3m emmc      nostretch - DRAM_USE_DDR4=1
  _buildimage mt7986 bpir3m spim-nand nostretch - DRAM_USE_DDR4=1 UBI=1 OVERRIDE_UBI_START_ADDR=0x580000
  _buildimage mt7986 bpir3m ram       nostretch - DRAM_USE_DDR4=1 RAM_BOOT_UART_DL=1
  _buildimage mt7988 bpir4  sdmmc     nostretch - DRAM_USE_COMB=1
  _buildimage mt7988 bpir4  emmc      nostretch - DRAM_USE_COMB=1
  _buildimage mt7988 bpir4  spim-nand nostretch - DRAM_USE_COMB=1 UBI=1 OVERRIDE_UBI_START_ADDR=0x200000
  _buildimage mt7986 bpir4  ram       nostretch - DRAM_USE_COMB=1 RAM_BOOT_UART_DL=1
  _buildimage mt7988 bpir4  sdmmc     nostretch 8 DRAM_USE_COMB=1 DDR4_4BG_MODE=1
  _buildimage mt7988 bpir4  emmc      nostretch 8 DRAM_USE_COMB=1 DDR4_4BG_MODE=1
  _buildimage mt7988 bpir4  spim-nand nostretch 8 DRAM_USE_COMB=1 DDR4_4BG_MODE=1 UBI=1 OVERRIDE_UBI_START_ADDR=0x200000
  _buildimage mt7986 bpir4  ram       nostretch 8 DRAM_USE_COMB=1 DDR4_4BG_MODE=1 RAM_BOOT_UART_DL=1
}
 
package_bpir-atf-git() {
  pkgdesc='ATF BPI-R64/R3/R4 images'
  replaces=(bpir64-mkimage)
  cd "${srcdir}"
  for _folder in "${srcdir}/${_gitname}/build/"*; do
    cd "$_folder/release"
    install -vDt "$pkgdir/usr/share/bpir-atf/" -m644 *-atf-*.bin
  done
}

package_bpir-atf-git-fiptool() {
  pkgdesc='ATF BPI-R64/R3/R4 fiptool'
  cd "${srcdir}/${_gitname}/tools/fiptool"
  install -m755 -vDt "$pkgdir/usr/bin" fiptool
}

