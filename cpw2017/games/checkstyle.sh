# This checks that all of our C code conforms to accepted style
# conventions. This is entirely OPTIONAL and is not necessary to run.

#!/bin/bash

STATUS=0

# readability/casting: Warnings about C casting instead of C++ casting
# runtime/int: Warnings about using C number types instead of C++ ones
# build/include_subdir: Warnings about prefacing included header files with directory

cpplint --quiet --extensions=c,h --headers=h --filter=-readability/casting,-runtime/int,-build/include_subdir --recursive .

if [ $? -ne "0" ]
then
    STATUS=1
    echo -e "\nCheckstyle: FAILED"
else
    echo -e "\nCheckstyle: PASSED"
fi

exit ${STATUS}
