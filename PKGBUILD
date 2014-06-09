# Maintainer: Jorge Martinez Hernandez <jorgemartinezhernandez@gmail.com>
# Contributor: Jorge Martinez Hernandez <jorgemartinezhernandez@gmail.com>

pkgname=feednix
pkgver=v0.6.3.0.r905d648
pkgrel=1
pkgdesc="A simple ncurses-based console client for Feedly"


arch=('any')
url="http://feednix-jarkore.rhcloud.com/"
license=('GPL3')
depends=('ncurses' 'curl' 'jsoncpp')
makedepends=('git' 'gcc')
md5sums=('SKIP')

source=('Feednix::git+http://github.com/Jarkore/Feednix')

pkgver() {
          cd "$srcdir/Feednix"
          git describe --long | sed -r 's/([^-]*g)/r\l/;s/-/./g'

}

build(){
        cd Feednix
        make
}

package(){
        cd "${srcdir}/Feednix"
        install -D -m755 feednix ${pkgdir}/usr/bin/feednix || return 1
        install -D -m644 LICENSE ${pkgdir}/usr/share/licenses/feednix/LICENSE
        make clean
}