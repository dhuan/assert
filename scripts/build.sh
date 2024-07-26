set -ex
gcc -I libyaml/include -o assert.o -c assert.c
gcc -I libyaml/include -o config.o -c config.c
gcc -I libyaml/include -o options.o -c options.c
gcc -I libyaml/include -o common.o -c common.c
gcc -o ./dist/assert assert.o config.o options.o common.o libyaml/src/.libs/libyaml.a
