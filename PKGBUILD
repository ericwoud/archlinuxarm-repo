# Maintainer: Eric Woudstra <ericwouds@gmail.com>

# ldd /bin/hostapd
#	linux-vdso.so.1 (0x0000ffffbedca000)
#	libnl-3.so.200 => /usr/lib/libnl-3.so.200 (0x0000ffffbeb20000)
#	libnl-genl-3.so.200 => /usr/lib/libnl-genl-3.so.200 (0x0000ffffbeaf0000)
#	libssl.so.3 => /usr/lib/libssl.so.3 (0x0000ffffbe9c0000)
#	libcrypto.so.3 => /usr/lib/libcrypto.so.3 (0x0000ffffbe3c0000)
#	libm.so.6 => /usr/lib/libm.so.6 (0x0000ffffbe300000)
#	libsqlite3.so.0 => /usr/lib/libsqlite3.so.0 (0x0000ffffbe170000)
#	libc.so.6 => /usr/lib/libc.so.6 (0x0000ffffbdfc0000)
#	/lib/ld-linux-aarch64.so.1 => /usr/lib/ld-linux-aarch64.so.1 (0x0000ffffbed80000)
#	libgcc_s.so.1 => /usr/lib/libgcc_s.so.1 (0x0000ffffbdf80000)
# ldd /bin/ethtool
#	linux-vdso.so.1 (0x0000ffff9f7ac000)
#	libm.so.6 => /usr/lib/libm.so.6 (0x0000ffff9f5e0000)
#	libmnl.so.0 => /usr/lib/libmnl.so.0 (0x0000ffff9f5c0000)
#	libc.so.6 => /usr/lib/libc.so.6 (0x0000ffff9f410000)
#	/lib/ld-linux-aarch64.so.1 => /usr/lib/ld-linux-aarch64.so.1 (0x0000ffff9f770000)

pkgname=hostapd-static-git
pkgver=2.11.1315.df577c18438d
pkgrel=1

#_libnlver=libnl3.12.0
_libnlver=main
#_sslver=openssl-3.6.0
_sslver=master
#_sqlitever=version-3.51.1
_sqlitever=master

pkgdesc="Static build of User space daemon for access point and authentication servers"
arch=('x86_64' 'aarch64')
url="https://w1.fi/hostapd/"
license=('BSD-3-Clause')
depends=()
makedepends=('git' 'musl' 'kernel-headers-musl')
provides=("hostapd=$pkgver")
conflicts=('hostapd')
backup=(etc/hostapd/hostapd.{accept,conf,deny,eap_user,radius_clients,vlan,wpa_psk})
options=('!emptydirs' '!lto')

source=("git+https://git.w1.fi/hostap.git"
        "git+https://gitlab.archlinux.org/archlinux/packaging/packages/hostapd.git"
        "git+https://github.com/thom311/libnl.git#tag=${_libnlver//./_}"
        "git+https://github.com/sqlite/sqlite.git#tag=${_sqlitever}"
        "git+https://github.com/openssl/openssl.git#tag=${_sslver}"
)
sha512sums=('SKIP' 'SKIP' 'SKIP' 'SKIP' 'SKIP')
#validpgpkeys=()

# to enable func64 interface in musl for 64-bit file system functions
# disable exceptions for musl build
export CFLAGS+=' -D_LARGEFILE64_SOURCE -fno-exceptions'
export CXXFLAGS+=' -D_LARGEFILE64_SOURCE -fno-exceptions'
export LDFLAGS="$LDFLAGS -static"
export CC=musl-gcc
export CXX=musl-gcc

prepare() {
  cd "${srcdir}/hostap"
  # fix include locations in main configuration file
  sed -e 's|/etc/hostapd|/etc/hostapd/hostapd|g' \
      -e 's|/var/run|/run|g' \
      -e 's|radius_attr.sqlite|/var/lib/hostapd/radius_attr.sqlite|g' \
      -e 's|hostapd.cred|/var/lib/hostapd/hostapd.cred|g' \
      -e 's|hostapd.ap_settings|/var/lib/hostapd/hostapd.ap_settings|g' \
      -e 's|hostapd_wps_pin_requests|hostapd/wps_pin_requests|g' \
      -i "hostapd/hostapd.conf"
  ( sed \
      -e '/^#CONFIG_RADIUS_SERVER=y/s/^#//' \
      -e '/^#CONFIG_DRIVER_WIRED=y/s/^#//' \
      -e '/^#CONFIG_DRIVER_NONE=y/s/^#//' \
      -e '/^#CONFIG_IEEE80211N=y/s/^#//' \
      -e '/^#CONFIG_WNM=y/s/^#//' \
      -e '/^#CONFIG_IEEE80211R=y/s/^#//' \
      -e '/^#CONFIG_IEEE80211AC=y/s/^#//' \
      -e '/^#CONFIG_IEEE80211AX=y/s/^#//' \
      -e '/^#CONFIG_IEEE80211BE=y/s/^#//' \
      -e '/^#CONFIG_FULL_DYNAMIC_VLAN=y/s/^#//' \
      -e '/^#CONFIG_LIBNL32=y/s/^#//' \
      -e '/^#CONFIG_ACS=y/s/^#//' \
      -e '/^#CONFIG_WEP=y/s/^#//' \
      -e '/^#CONFIG_SAE=y/s/^#//' \
      -e '/^#CONFIG_SAE_PK=y/s/^#//' \
      -e '/^#CONFIG_ELOOP_EPOLL=y/s/^#//' \
      -e '/^#CONFIG_FST=y/s/^#//' \
      -e '/^#CONFIG_FST_TEST=y/s/^#//' \
      -e '/^#CONFIG_MBO=y/s/^#//' \
      -e '/^#CONFIG_WPA_CLI_EDIT=y/s/^#//' \
      -e '/^#CONFIG_AIRTIME_POLICY=y/s/^#//' \
      -e '/^#CONFIG_OCV=y/s/^#//' \
      hostapd/defconfig
  ) > hostapd/.config

  cd "${srcdir}"/openssl
  sed -i "s@./demoCA@/etc/ssl@g" apps/CA.pl.in apps/openssl.cnf
}

pkgver() {
  cd "${srcdir}/hostap"
  _tag=$(git tag -l --sort -v:refname | grep -E '^hostap_[0-9_\.]+$' | head -n1)
  _rev=$(git rev-list --count $_tag..HEAD)
  _rev=$(( _rev + $(git -C "${startdir}" rev-list --count HEAD) ))
  _hash=$( git                        rev-parse --short HEAD | tail -c 4)
  _hash+=$(git -C "${srcdir}/hostapd" rev-parse --short HEAD | tail -c 4)
  _hash+=$(git -C "${srcdir}/openssl" rev-parse --short HEAD | tail -c 4)
  _hash+=$(git -C "${srcdir}/libnl"   rev-parse --short HEAD | tail -c 4)
  printf "%s.%s.%s" "$_tag" "$_rev" "$_hash" | sed 's/^hostap_//;s/_/./g'
}

build() {
  export PKG_CONFIG_PATH="${srcdir}"/temp/usr/lib/pkgconfig
  export PATH="${srcdir}/temp/usr/bin:${PATH}"

  # this uses malloc_usable_size, which is incompatible with fortification level 3
  export CFLAGS="${CFLAGS/_FORTIFY_SOURCE=3/_FORTIFY_SOURCE=2}"
  export CXXFLAGS="${CXXFLAGS/_FORTIFY_SOURCE=3/_FORTIFY_SOURCE=2}"
  export CPPFLAGS="$CPPFLAGS \
        -DSQLITE_ENABLE_COLUMN_METADATA=1 \
        -DSQLITE_ENABLE_UNLOCK_NOTIFY \
        -DSQLITE_ENABLE_DBSTAT_VTAB=1 \
        -DSQLITE_ENABLE_FTS3_PARENTHESIS \
        -DSQLITE_SECURE_DELETE \
        -DSQLITE_ENABLE_STMTVTAB \
        -DSQLITE_ENABLE_STAT4 \
        -DSQLITE_MAX_VARIABLE_NUMBER=250000 \
        -DSQLITE_MAX_EXPR_DEPTH=10000 \
        -DSQLITE_ENABLE_MATH_FUNCTIONS"

  cd "${srcdir}"/sqlite
  ./configure --prefix="${srcdir}"/temp/usr \
    --disable-shared \
    --enable-static \
    --enable-fts3 \
    --fts4 \
    --fts5 \
    --rtree \
    --soname=legacy
  sed -i -e 's/$(LDFLAGS.libsqlite3)/ -Wl,-O1,--as-needed \0/g' main.mk
  make install

  cd "${srcdir}"/"libnl"
  ./autogen.sh
  ./configure \
      --prefix="${srcdir}"/temp/usr \
      --sysconfdir="${srcdir}"/temp/etc \
      --sbindir=/usr/bin \
      --disable-cli \
      --disable-debug \
      --enable-static --disable-shared
  make install-libLTLIBRARIES

  # check pacman-static for option for arch's other then aarch64
  cd "${srcdir}"/openssl
  case ${CARCH} in
        x86_64)
            openssltarget='linux-x86_64'
            optflags='enable-ec_nistp_64_gcc_128'
            ;;
        aarch64)
            openssltarget='linux-aarch64'
            optflags='no-afalgeng'
            ;;
    esac
  # mark stack as non-executable: http://bugs.archlinux.org/task/12434
  ./Configure --prefix="${srcdir}"/temp/usr \
                --openssldir=/etc/ssl \
                --libdir=lib \
                -static \
                no-ssl3-method \
                ${optflags} \
                "${openssltarget}" \
                "-Wa,--noexecstack ${CPPFLAGS} ${CFLAGS} ${LDFLAGS}"
  make build_libs
  make install_dev

  rm "${srcdir}"/temp/usr/lib/lib*.la

  cd "${srcdir}/hostap"
  _staticlibs="-Wl,-rpath=${srcdir}/temp/usr/lib -L${srcdir}/temp/usr/lib -I${srcdir}/temp/usr/include"
  export   LDFLAGS+="  -pie $_staticlibs"
  export    CFLAGS+=" -fPIE $_staticlibs"
  export CXXDFLAGS+=" -fPIE $_staticlibs"
  make -C "hostapd" LIBS="-lsqlite3 -lssl -lcrypto -lnl-3 -lnl-genl-3 -lm -lpthread"
}

package() {
  cd "${srcdir}/hostap"
  make \
    -C "hostapd" \
    DESTDIR="$pkgdir" \
    BINDIR="/usr/bin" \
    install
  install -Dm644 "$srcdir"/hostapd/hostapd{,@}.service -t "$pkgdir/usr/lib/systemd/system"
  install -Dm644 "COPYING" -t "$pkgdir/usr/share/licenses/hostapd"
  install -Dm640 hostapd/hostapd.{accept,conf,deny,eap_user,radius_clients,vlan,wpa_psk} -t "$pkgdir/etc/hostapd"
  install -Dm644 hostapd/{hostapd.sim_db,wired.conf,hlr_auc_gw.{txt,milenage_db}} hostapd/{README*,ChangeLog} -t "$pkgdir/usr/share/doc/hostapd"
  install -Dm644 "hostapd/hostapd.8" -t "$pkgdir/usr/share/man/man8"
  install -Dm644 "hostapd/hostapd_cli.1" -t "$pkgdir/usr/share/man/man1"
  install -dm750 "$pkgdir/var/lib/hostapd"
}
