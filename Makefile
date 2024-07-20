all: get_dependencies build

get_dependencies:
	sh scripts/get_dependencies.sh

clean_up_dependencies:
	sh scripts/clean_up_dependencies.sh

build:
	sh scripts/build.sh
