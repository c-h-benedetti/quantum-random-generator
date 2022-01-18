
if [ ! -d "./cpr" ]; then
	mkdir "cpr"
	cd "cpr"
	git clone "https://github.com/libcpr/cpr.git"
	cd ..
fi

mkdir "build"
cd "build"
yes | rm -r *
cmake .. && make
echo "Done"