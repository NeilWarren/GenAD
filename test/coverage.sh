#! /bin/bash

# grab pwd
TEST_DIR="$(pwd)"

# get project root dir
cd ..
MY_PATH="$( cd "$( dirname "$0" )" && pwd )"
PROJECT_ROOT=${MY_PATH}
cd ${TEST_DIR}

COV_SRC_DIR=${TEST_DIR}/build/CMakeFiles/GenADTests.dir
COVERAGE_DIR=${TEST_DIR}/coverage

# create coverage dir if not exists
if [ ! -d "${COVERAGE_DIR}" ]; then
  mkdir "${COVERAGE_DIR}"
fi

cd ${COVERAGE_DIR}

lcov --capture -d ${COV_SRC_DIR} -o GenAD_COV.info

# remove usr info
lcov --remove GenAD_COV.info "/usr/*" "${TEST_DIR}/*" -o GenAD_COV.info

# generate html version of coverage
genhtml GenAD_COV.info -o CODE_COVERAGE
cd ..

echo "Code Coverage generated in ./coverage/CODE_COVERAGE;"
