# This checks that all of our Python code conforms to accepted style
# conventions. This is entirely OPTIONAL and is not necessary to run.

#!/bin/bash

STATUS=0

flake8 --filename=*.py

if [ $? -ne "0" ]
then
    STATUS=1
    echo -e "\nCheckstyle: FAILED"
else
    echo -e "\nCheckstyle: PASSED"
fi

exit ${STATUS}
