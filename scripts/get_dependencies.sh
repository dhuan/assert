set -e

if [ -d "libyaml" ]
then
    printf "'libyaml' folder already exists. If you'd like to install the dependencies from start then use:\n$ make clean_up_dependencies && make build\n"

    exit 1
fi

git clone -b 0.2.5 https://github.com/yaml/libyaml.git
cd libyaml
./bootstrap
./configure
make
