args=$1

buildType="Debug"
clean=false

if [ "${args}" = "release" ]; then
    buildType="Release"
elif [ "${args}" = "clean" ]; then
    clean=true
fi

build(){
    for i in $(ls -d */); do 
        cd $i
        if [ ${clean} = true ]; then
            rm CMakeCache.txt
            make clean 
        else
            cmake3 -DCMAKE_BUILD_TYPE=${buildType} ./ 
            make
        fi
        cd ..
    done
}

cd src/libs/
build

cd ../apps/
build

cd ../tools/
build
