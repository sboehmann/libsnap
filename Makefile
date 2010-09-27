.PHONY: all test cloc doc clean mrproper

all: cmake
	cd build && $(MAKE) && $(MAKE) install

test: cmake
	cd build ; $(MAKE) test

package: cmake
	cd build ; $(MAKE) package ; $(MAKE) package_source

apidoc: cmake
	cd build ; $(MAKE) apidoc

www: cmake
	cd build ; $(MAKE) www
	#cd doc/website &&  jekyll

clean:
	find -type f -iname "*~" -exec rm '{}' \;
	cd build && $(MAKE) clean

mrproper:
	rm -rf build/
	rm -rf install/
	find -type f -iname "*~" -exec rm '{}' \;

init:
	mkdir -p install
	mkdir -p build

cmake: init
	cd build ; cmake .. -DCMAKE_INSTALL_PREFIX=../install
