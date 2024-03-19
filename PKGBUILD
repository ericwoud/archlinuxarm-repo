# AArch64 multi-platform
# Maintainer: Kevin Mihelich <kevin@archlinuxarm.org>

#NOEXTRACT="1"

buildarch=8

pkgbase=linux-aarch64-rk-rc
pkgname=("${pkgbase}" "${pkgbase}-headers")
_kernelname=${pkgbase#linux}
_desc="AArch64 multi-platform"
pkgver=6.8.r20240310
pkgrel=1
_srcname=linux
arch=('aarch64')
url="http://www.kernel.org/"
license=('GPL2')
makedepends=('xmlto' 'docbook-xsl' 'kmod' 'inetutils' 'bc' 'git' 'uboot-tools' 'dtc')
options=('!strip')
source=('0001-net-smsc95xx-Allow-mac-address-to-be-set-as-a-parame.patch'
        '0002-arm64-dts-rockchip-disable-pwm0-on-rk3399-firefly.patch'
        'src/config::https://github.com/armbian/build/raw/main/config/kernel/linux-rockchip-rk3588-edge.config'
        'generate_chromebook_its.sh'
        'kernel.keyblock'
        'kernel_data_key.vbprivk'
        'linux.preset'
        'rk3588-bpi-m7.dts')
md5sums=('7b08a199a97e3e2288e5c03d8e8ded2d'
         'c9d4e392555b77034e24e9f87c5ff0b3'
         SKIP
         '7c97cf141750ad810235b1ad06eb9f75'
         '61c5ff73c136ed07a7aadbf58db3d96a'
         '584777ae88bce2c5659960151b64c7d8'
         SKIP
         SKIP)

_gitroot="git://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git"

export CARCH=aarch64
export LOCALVERSION=""
if [[ "$(uname -m)" == "aarch64" ]]; then
  pkgname+=("${pkgbase}-chromebook")
  makedepends+=('vboot-utils')
else
  makedepends+=('aarch64-linux-gnu-gcc')
  export MAKEFLAGS+=" ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu-"
fi

prepare() {
  _tag=$(git ls-remote --tags --refs --exit-code "${_gitroot}" | \
         sed -e 's/$/-x/' | sort -k2 | tail -n1 | sed 's/..$//' | cut -d$'/' -f3)
  echo "Latest tag: $_tag"
  if [[ ! -d "${srcdir}/${_srcname}/" ]]; then
    cd "${srcdir}/"
    git clone --branch "${_tag}" --depth=1 "${_gitroot}" "${srcdir}/${_srcname}/"
  else
    cd "${srcdir}/${_srcname}/"
    git fetch --depth=1 origin ${_tag}
    git reset --hard FETCH_HEAD
  fi
  cd "${srcdir}/${_srcname}/"

  echo "Setting version..."
  echo "-$pkgrel" > localversion.10-pkgrel
  echo "${pkgbase#linux}" > localversion.20-pkgname

  # ALARM patches
  git apply ../0001-net-smsc95xx-Allow-mac-address-to-be-set-as-a-parame.patch
  git apply ../0002-arm64-dts-rockchip-disable-pwm0-on-rk3399-firefly.patch

  if [ -z "$(grep "rk3588-bpi-m7.dtb" arch/arm64/boot/dts/rockchip/Makefile)" ]; then
    echo -e '\ndtb-$(CONFIG_ARCH_ROCKCHIP) += rk3588-bpi-m7.dtb' \
                >>arch/arm64/boot/dts/rockchip/Makefile
  fi	
  cp -vf ../rk3588-bpi-m7.dts arch/arm64/boot/dts/rockchip

##  cat "${srcdir}/config" > ./.config

#  cp -vf arch/arm64/configs/defconfig   ./arch/arm64/configs/rockchip_defconfig
#  if [ -f "${startdir}/defconfig_merge" ]; then
#    echo -e "\n# After this is added" >>./arch/arm64/configs/rockchip_defconfig
#    cat "${startdir}/defconfig_merge" >>./arch/arm64/configs/rockchip_defconfig
#  fi
#  make ${MAKEFLAGS} rockchip_defconfig
#  rm -vf ./arch/arm64/configs/rockchip_defconfig

  # get kernel version
  make ${MAKEFLAGS} olddefconfig
  make ${MAKEFLAGS} prepare
  make ${MAKEFLAGS} -s kernelrelease > version
  echo "Prepared for $(<version)"
}

pkgver() {
  cd "$srcdir/$_srcname"
  _v1=$(grep '^VERSION = ' Makefile | cut -b 11-)
  _v2=$(grep '^PATCHLEVEL = ' Makefile | cut -b 14-)
  _v3=$(grep '^EXTRAVERSION = ' Makefile | cut -b 16-)
  _date=$(git log -n1 --date=short --pretty=format:'%cd')
  printf "%s.r%s" "${_v1}.${_v2}${_v3/-/_}" "${_date//-/}"
}

build() {
  cd ${_srcname}
  # build!
  unset LDFLAGS
  if [[ -v MENUCONFIG ]]; then
    make ${MAKEFLAGS} $_llvm menuconfig
    make ${MAKEFLAGS} $_llvm savedefconfig
    scripts/diffconfig -m arch/arm64/configs/defconfig defconfig | \
                  sed 's/# //g' | sed 's/ is not set/=n/g' | \
                  tee ${startdir}/defconfig_merge
  fi
  make ${MAKEFLAGS} Image Image.gz modules
  # Generate device tree blobs with symbols to support applying device tree overlays in U-Boot
  make ${MAKEFLAGS} DTC_FLAGS="-@" dtbs
}

_package() {
  pkgdesc="The Linux Kernel and modules - ${_desc}"
  depends=('coreutils' 'linux-firmware' 'kmod' 'mkinitcpio>=0.7')
  optdepends=('wireless-regdb: to set the correct wireless channels of your country')
  backup=("etc/mkinitcpio.d/${pkgbase}.preset")
  install=${pkgname}.install

  cd ${_srcname}
  local kernver="$(<version)"
  local modulesdir="$pkgdir/usr/lib/modules/$kernver"

  # install dtbs
  make INSTALL_DTBS_PATH="${pkgdir}/boot/dtbs/${pkgbase}" dtbs_install

  echo "Installing modules..."
  make INSTALL_MOD_PATH="$pkgdir/usr" INSTALL_MOD_STRIP=1 modules_install

  # copy kernel
  local _dir_module="${pkgdir}/usr/lib/modules/$(<version)"
  install -Dm644 arch/arm64/boot/Image "${_dir_module}/vmlinuz"

  # remove reference to build host
  rm -f "${_dir_module}/"{build,source}

  # used by mkinitcpio to name the kernel
  echo "${pkgbase}" | install -D -m 644 /dev/stdin "${_dir_module}/pkgbase"

  # sed expression for following substitutions
  local _subst="
    s|%PKGBASE%|${pkgbase}|g
    s|%KERNVER%|${kernver}|g
  "

  # install mkinitcpio preset file
  sed "${_subst}" ../linux.preset |
    install -Dm644 /dev/stdin "${pkgdir}/etc/mkinitcpio.d/${pkgbase}.preset"

  # rather than use another hook (90-linux.hook) rely on kmod's 90-mkinitcpio-install.hook
  # which avoids a double run of mkinitcpio that can occur
  install -d "${pkgdir}/usr/lib/firmware/"
  touch "${pkgdir}/usr/lib/firmware/$(<version)"
}

_package-headers() {
  pkgdesc="Header files and scripts for building modules for linux kernel - ${_desc}"

  cd $_srcname
  local builddir="$pkgdir/usr/lib/modules/$(<version)/build"

  echo "Installing build files..."
  install -Dt "$builddir" -m644 .config Makefile Module.symvers System.map \
    localversion.* version vmlinux
  install -Dt "$builddir/kernel" -m644 kernel/Makefile
  install -Dt "$builddir/arch/arm64" -m644 arch/arm64/Makefile
  cp -t "$builddir" -a scripts

  # add xfs and shmem for aufs building
  mkdir -p "$builddir"/{fs/xfs,mm}

  echo "Installing headers..."
  cp -t "$builddir" -a include
  cp -t "$builddir/arch/arm64" -a arch/arm64/include
  install -Dt "$builddir/arch/arm64/kernel" -m644 arch/arm64/kernel/asm-offsets.s
  mkdir -p "$builddir/arch/arm"
  cp -t "$builddir/arch/arm" -a arch/arm/include

  install -Dt "$builddir/drivers/md" -m644 drivers/md/*.h
  install -Dt "$builddir/net/mac80211" -m644 net/mac80211/*.h

  # https://bugs.archlinux.org/task/13146
  install -Dt "$builddir/drivers/media/i2c" -m644 drivers/media/i2c/msp3400-driver.h

  # https://bugs.archlinux.org/task/20402
  install -Dt "$builddir/drivers/media/usb/dvb-usb" -m644 drivers/media/usb/dvb-usb/*.h
  install -Dt "$builddir/drivers/media/dvb-frontends" -m644 drivers/media/dvb-frontends/*.h
  install -Dt "$builddir/drivers/media/tuners" -m644 drivers/media/tuners/*.h

  # https://bugs.archlinux.org/task/71392
  install -Dt "$builddir/drivers/iio/common/hid-sensors" -m644 drivers/iio/common/hid-sensors/*.h

  echo "Installing KConfig files..."
  find . -name 'Kconfig*' -exec install -Dm644 {} "$builddir/{}" \;

  echo "Removing unneeded architectures..."
  local arch
  for arch in "$builddir"/arch/*/; do
    [[ $arch = */arm64/ || $arch == */arm/ ]] && continue
    echo "Removing $(basename "$arch")"
    rm -r "$arch"
  done

  echo "Removing documentation..."
  rm -r "$builddir/Documentation"

  echo "Removing broken symlinks..."
  find -L "$builddir" -type l -printf 'Removing %P\n' -delete

  echo "Removing loose objects..."
  find "$builddir" -type f -name '*.o' -printf 'Removing %P\n' -delete

  echo "Stripping build tools..."
  local file
  while read -rd '' file; do
    case "$(file -bi "$file")" in
      application/x-sharedlib\;*)      # Libraries (.so)
        strip -v $STRIP_SHARED "$file" ;;
      application/x-archive\;*)        # Libraries (.a)
        strip -v $STRIP_STATIC "$file" ;;
      application/x-executable\;*)     # Binaries
        strip -v $STRIP_BINARIES "$file" ;;
      application/x-pie-executable\;*) # Relocatable binaries
        strip -v $STRIP_SHARED "$file" ;;
    esac
  done < <(find "$builddir" -type f -perm -u+x ! -name vmlinux -print0)

  echo "Adding symlink..."
  mkdir -p "$pkgdir/usr/src"
  ln -sr "$builddir" "$pkgdir/usr/src/$pkgbase"
}

_package-chromebook() {
  pkgdesc="The Linux Kernel - ${_desc} - Chromebooks"
  depends=(${pkgbase})
  install=${pkgname}.install

  cd ${_srcname}

  mkdir -p "${pkgdir}/boot"

  image=arch/arm64/boot/Image

  chromeos_boards=(
    'elm'
    'gru'
    'kukui'
    'trogdor'
    'asurada'
  )
  chromebook_dtbs=($(for b in ${chromeos_boards[@]}; do find arch/arm64/boot -name "*${b}*.dtb" | LC_COLLATE=C sort; done))

  lz4 -20 -z -f -m ${image}
  echo ${chromebook_dtbs[@]} | ../generate_chromebook_its.sh ${image}.lz4 arm64 lz4 > kernel.its

  mkimage -D "-I dts -O dtb -p 2048" -f kernel.its vmlinux.uimg
  dd if=/dev/zero of=bootloader.bin bs=512 count=1
  echo "console=tty0 console=ttyS2,115200n8 earlyprintk=ttyS2,115200n8 console=ttyMSM0,115200n8 init=/sbin/init root=PARTUUID=%U/PARTNROFF=1 rootwait rw noinitrd" > cmdline
  vbutil_kernel \
    --pack vmlinux.kpart \
    --version 1 \
    --vmlinuz vmlinux.uimg \
    --arch aarch64 \
    --keyblock ../kernel.keyblock \
    --signprivate ../kernel_data_key.vbprivk \
    --config cmdline \
    --bootloader bootloader.bin

  cp vmlinux.kpart "${pkgdir}/boot"
}

for _p in ${pkgname[@]}; do
  eval "package_${_p}() {
    _package${_p#${pkgbase}}
  }"
done
