#!/bin/sh

set -e

if [ -z "$CONTAINER_PROGRAM" ]
then
    printf '$CONTAINER_PROGRAM missing!\n'

    exit 1
fi

if ! ${CONTAINER_PROGRAM} run -tid debian sh
then
    printf 'Failed to start container!\n'

    exit 1
fi

CONTAINER_ID=$(${CONTAINER_PROGRAM} ps -q --last 1)

git archive --format=tgz HEAD | \
    ${CONTAINER_PROGRAM} exec -i "${CONTAINER_ID}" tar xz -C /root

${CONTAINER_PROGRAM} exec -i "${CONTAINER_ID}" sh <<EOF
    set -e
    apt update -y
    apt install -y \
        make git binutils autoconf automake autotools-dev csh g++ libtool
    cd /root
    make get_dependencies
    make build
    ASSERT_PROGRAM=dist/assert make tests
EOF
