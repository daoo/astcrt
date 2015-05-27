default: linux-debug

all: linux-debug linux-release linux-extreme arm-release

build/%.ninja: build.ninja.template configure
	./configure

linux-debug: build/linux-debug.ninja
	@ninja -C build -f linux-debug.ninja

linux-release: build/linux-release.ninja
	@ninja -C build -f linux-release.ninja

linux-extreme: build/linux-extreme.ninja
	@ninja -C build -f linux-extreme.ninja

arm-release: build/arm-release.ninja
	@ninja -C build -f arm-release.ninja

distclean:
	rm -fr build
