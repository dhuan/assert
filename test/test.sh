if [ -z "${ASSERT_PROGRAM}" ]
then
    printf "\$ASSERT_PROGRAM missing!\n"

    exit 1
fi

TMP=$(mktemp)
TMP2=$(mktemp)

find test/cases | grep yml$ | grep -v expect_output.yml | while read TEST_CASE_FILE;
do
    EXPECT_OUTPUT_FILE=$(echo $TEST_CASE_FILE | sed s/.yml/.expect_output.yml/)

    tail -n +2 < $EXPECT_OUTPUT_FILE > $TMP2

    EXPECT_STATUS_CODE=$(head -n 1 < $EXPECT_OUTPUT_FILE)

    printf "TEST: %s\n" $TEST_CASE_FILE

    $ASSERT_PROGRAM $TEST_CASE_FILE > $TMP

    ASSERT_STATUS_CODE_RESULT="$?"

    if [ "$?" != "${EXPECT_STATUS_CODE}" ]
    then
        printf "Failed: %s\nExpected status code to be %s. Instead got %s\n." "${TEST_CASE_FILE}" "${EXPECT_STATUS_CODE}" "${ASSERT_STATUS_CODE_RESULT}"

        exit 1
    fi

    diff "$TMP" "$TMP2"
    if [ "$?" != "0" ]
    then
        printf "Expected output did not match! Check diff above.\n"

        exit 1
    fi

    printf "✔ PASSED!\n"
done
