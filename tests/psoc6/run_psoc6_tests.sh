#/usr/bin/sh

#
# psoc6/run_psoc6_tests.sh -h
#


echo "executing $0 $* ..."


usage() {
  echo "Usage: $0 -a <run all test> -c <clean results directory and file before running tests> -d <device to be used> -f <run failing tests only> -i <run implemented tests only> -n <run not yet implemented tests only>" 1>&2;
  exit 1;
}


while getopts "acd:fhin" o; do
  case "${o}" in
    a)
       all=1
       ;;
    c)
       cleanResultsDirectoryFirst=1
       ;;
    d)
       device=${OPTARG}
       ;;
    f)
       failing=1
       ;;
    h)
       usage
       ;;
    i)
       implemented=1
       ;;
    n)
       notYetImplemented=1
       ;;
    *)
       usage
       ;;
  esac
done


if [ -z "${all}" ]; then
  all=0
fi


if [ -z "${cleanResultsDirectoryFirst}" ]; then
  cleanResultsDirectoryFirst=0
fi


if [ -z "${device}" ]; then
  device="/dev/ttyACM0"
fi


if [ -z "${failing}" ]; then
  failing=0
fi


if [ -z "${implemented}" ]; then
  implemented=0
fi


if [ -z "${notYetImplemented}" ]; then
  notYetImplemented=0
fi


resultsFile="psoc6_test_results.log"
passResultsFile="psoc6_test_passed.log"
skipResultsFile="psoc6_test_skipped.log"
failResultsFile="psoc6_test_failed.log"


echo
echo "  device            : ${device}"
echo
echo "  results file      : ${resultsFile}"
echo "  pass results file : ${passResultsFile}"
echo "  skip results file : ${skipResultsFile}"
echo "  fail results file : ${failResultsFile}"
echo


if [ ${cleanResultsDirectoryFirst} == 1 ]; then

  echo
  echo "  removing results directory and file if existent ..."

  if [ -e results ]; then

    rm -rf results ${resultsFile}

  fi

  echo "  done ."
  echo

fi


if [ ${all} == 1 ]; then

  echo "  running all tests ..."
  echo

fi


if [ ${all} == 1 ] || [ ${implemented} == 1 ]; then

  echo "  running implemented tests ..."
  echo

### for manual testing of the io and unicode modules, files can be copied this way; still the file path in the io/*.py scripts must be 
### adapted to our filesystem mount point

  # prepare execution of tests by uploading required files to on-board flash
  #../tools/mpremote/mpremote.py cp import/.py :/flash/import/.py

  ### io
  # ../tools/mpremote/mpremote.py mkdir :/flash/io
  # ../tools/mpremote/mpremote.py mkdir :/flash/io/data

#   ../tools/mpremote/mpremote.py rm :/flash/io/data/bigfile1
#   ../tools/mpremote/mpremote.py rm :/flash/io/data/file1
#   ../tools/mpremote/mpremote.py rm :/flash/io/data/file2

#   ../tools/mpremote/mpremote.py cp io/data/bigfile1 :/flash/io/data/bigfile1
#   ../tools/mpremote/mpremote.py cp io/data/file1 :/flash/io/data/file1
#   ../tools/mpremote/mpremote.py cp io/data/file2 :/flash/io/data/file2


  ### unicode
  # ../tools/mpremote/mpremote.py mkdir :/flash/unicode
  # ../tools/mpremote/mpremote.py mkdir :/flash/unicode/data

  # ../tools/mpremote/mpremote.py cp unicode/data/utf-8_1.txt :/flash/unicode/data/utf-8_1.txt
  # ../tools/mpremote/mpremote.py cp unicode/data/utf-8_2.txt :/flash/unicode/data/utf-8_2.txt
  # ../tools/mpremote/mpremote.py cp unicode/data/utf-8_invalid.txt :/flash/unicode/data/utf-8_invalid.txt


  ./run-tests.py --target psoc6 --device ${device} \
          io/builtin_print_file.py \
          unix/time.py \
    | tee -a ${resultsFile}


  ./run-tests.py --target psoc6 --device ${device} \
        -d \
          basics \
          extmod \
          feature_check \
          float \
          frozen \
          import \
          inlineasm \
          micropython \
          misc \
          perf_bench \
          psoc6 \
          stress \
          unicode \
        \
        -e extmod/utime_res.py \
        -e extmod/vfs_lfs_mtime.py \
        \
        -e feature_check/async_check.py \
        -e feature_check/bytearray.py \
        -e feature_check/byteorder.py \
        -e feature_check/complex.py \
        -e feature_check/const.py \
        -e feature_check/coverage.py \
        -e feature_check/float.py \
        -e feature_check/native_check.py \
        -e feature_check/repl_emacs_check.py \
        -e feature_check/repl_words_move_check.py \
        -e feature_check/set_check.py \
        -e feature_check/slice.py \
        -e feature_check/uio_module.py \
        \
        -e float/math_domain_special.py \
        \
        -e import/gen_context.py \
        -e import/import1a.py \
        -e import/import2a.py \
        -e import/import3a.py \
        -e import/import_file.py \
        -e import/import_long_dyn.py \
        -e import/import_override.py \
        -e import/import_pkg1.py \
        -e import/import_pkg2.py \
        -e import/import_pkg3.py \
        -e import/import_pkg4.py \
        -e import/import_pkg5.py \
        -e import/import_pkg6.py \
        -e import/import_pkg7.py \
        -e import/import_pkg8.py \
        -e import/module_dict.py \
        -e import/try_module.py \
        \
        -e micropython/import_mpy_invalid.py \
        -e micropython/import_mpy_native.py \
        -e micropython/viper_error.py \
        \
        -e perf_bench/core_import_mpy_multi.py \
        -e perf_bench/core_import_mpy_single.py \
        -e 'perf_bench/viper_call.*.py' \
        \
        -e unicode/file1.py \
        -e unicode/file2.py \
        -e unicode/file_invalid.py \
    | tee -a ${resultsFile}

  echo
  echo "  done ."
  echo

fi

# rp2 tests="basics/*.py micropython/*.py float/*.py import/*.py io/*.py misc/*.py unicode/*.py extmod/*.py unix/*.py"


### not yet enabled/implemented, therefore failing
if [ ${all} == 1 ] || [ ${notYetImplemented} == 1 ]; then

  echo "  running not yet implemented tests ..."
  echo

  ./run-tests.py --target psoc6 --device ${device} -d \
        multi_bluetooth \
        multi_net \
        net_hosted \
        net_inet \
        thread \
    | tee -a ${resultsFile}

  echo
  echo "  done ."
  echo

fi


### all excluded because it is know that these tests fail. In case of folder 
#
#             - cpydiff : please refer to documentetion within tests
#             - cmdline : tests are for command line Python execution and not for embedded MPY
#             - io      : tests required data files on board flash and a change in the file paths to match the flash mount point
#             - internal_bench : used for measuring run time of certain operations. Will always fail because runtime of Python and MPY will differ.
# 
### therefore have been placed in this category
###
if [ ${all} == 1 ] || [ ${failing} == 1 ]; then

  echo "  running failing tests ..."
  echo


  # prepare execution of tests by uploading required files to on-board flash
  ../tools/mpremote/mpremote.py cp internal_bench/bench.py :/flash/bench.py


  ./run-tests.py --target psoc6 --device ${device} -d \
        cmdline \
        cpydiff \
        internal_bench \
        io \
        jni \
    | tee -a ${resultsFile}

  echo
  echo "  done ."
  echo

fi


if [ ${all} == 1 ]; then

  echo "  running all tests done."
  echo

fi


echo
echo "generating pass, skip and fail files ..."

grep -i 'pass ' ${resultsFile} > ${passResultsFile}
grep -i 'skip ' ${resultsFile} > ${skipResultsFile}
grep -i 'FAIL ' ${resultsFile} > ${failResultsFile}

echo "generating pass, skip and fail files done."


echo
echo "executing $0 $* done."
