# No. of tests: 4 per file

echo "Running tests: "

echo "" > tests/tests.log

if ! [ -x "$(command -v cat)" ]; then
    echo 'Error: cat is not installed.' >&2
    #exit 1
fi

tests=0

for i in tests/*.txt
do 
  # Normal print
  echo "test No.: $tests"
  if [ "$(./bin/kat $i)" = "$(cat $i)" ]; then
    ((tests+=1))
  else
    echo "failed test on file(normal test): $i"
    #exit 1
  fi

  # Tab print
  echo "test No.: $tests"
  if [ "$(./bin/kat -t $i)" = "$(cat -T $i)" ]; then
    ((tests+=1))
  else
    # logging 
    echo -e "kat output: \n" >> tests/tests.log
    ./bin/kat -t $i >> tests/tests.log
    echo -e "\ncat output: \n" >> tests/tests.log
    cat -T $i >> tests/tests.log

    echo "failed test on file(tab test): $i"
    #exit 1
  fi

  # Ends print
  echo "test No.: $tests"
  if [ "$(./bin/kat -e $i)" = "$(cat -E $i)" ]; then
    ((tests+=1))
  else
    echo "failed test on file(ends test): $i"
    #exit 1
  fi

  # line number 
  echo "test No.: $tests"
  if [ "$(./bin/kat -n $i)" = "$(cat -n $i)" ]; then
    ((tests+=1))
  else
    echo "failed test on file(ends test): $i"
    ##exit 1
  fi
done

files=`ls -l tests/*.txt | wc -l`

if [ $(( $files * 4 )) -eq $tests ]; then
  echo "Passed all the tests"
else
  echo "Passed: $tests, Failed: $(( $files*4 - $tests ))"
fi
