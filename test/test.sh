#!/bin/sh

if [ -z "${ASSERT_PROGRAM}" ]
then
    printf "\$ASSERT_PROGRAM missing!\n"

    exit 1
fi

TMP=$(mktemp)
TMP2=$(mktemp)

find test/cases | grep yml$ | grep -v expect_output.yml | while read -r TEST_CASE_FILE;
do
    EXPECT_OUTPUT_FILE=$(echo "${TEST_CASE_FILE}" | sed s/.yml/.expect_output.txt/)

    if [ ! -f "${EXPECT_OUTPUT_FILE}" ]
    then
        printf "Output expectation file not found! (%s)\n" "${EXPECT_OUTPUT_FILE}"

        exit 1
    fi

    tail -n +2 < "${EXPECT_OUTPUT_FILE}" > "${TMP2}"

    EXPECT_STATUS_CODE=$(head -n 1 < "${EXPECT_OUTPUT_FILE}")

    printf "TEST: %s\n" "${TEST_CASE_FILE}"

    ASSERT_FLAGS=$(head -n 1 < "${TEST_CASE_FILE}" | awk '{sub(/# ?/, "")}1')

    $ASSERT_PROGRAM ${ASSERT_FLAGS} "${TEST_CASE_FILE}" &> "${TMP}"

    ASSERT_STATUS_CODE_RESULT="$?"

    if [ "${ASSERT_STATUS_CODE_RESULT}" != "${EXPECT_STATUS_CODE}" ]
    then
        printf "Failed: %s\nExpected status code to be %s. Instead got %s\n." "${TEST_CASE_FILE}" "${EXPECT_STATUS_CODE}" "${ASSERT_STATUS_CODE_RESULT}"

        exit 1
    fi

    if ! diff "$TMP" "$TMP2"
    then
        printf "✗ Expected output did not match! Check diff above.\n"

        exit 1
    fi

    printf "✔ PASSED!\n"
done
