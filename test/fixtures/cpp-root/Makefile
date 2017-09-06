default: build

run: build
	./hello -c cpp.js

build: CMakeLists.txt
	mkdir build
	(cd build && cmake ..)
	(cd build && make -j7)

clean:
	rm -rf build

####################
# for local development:
#   make devlibs && make hello && LD_LIBRARY_PATH=cpp/lib make run

devclean:
	rm -rf cpp
	rm -f cpp-1.0.5-linux-x64.tgz

devlibs: cpp-1.0.5-linux-x64.tgz
	mkdir -p cpp
	tar -m --directory cpp -zxf cpp-1.0.5-linux-x64.tgz

#cpp-1.0.5-linux-x64.tgz:
#	wget -c 'https://github.com/flavorjones/cppcms-buildpack-test/blob/master/vendor/cflinuxfs2/cppcms-1.0.5-linux-x64.tgz?raw=true' -O cppcms-1.0.5-linux-x64.tgz
