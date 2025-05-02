#!/bin/bash

BUILD_PATH="build/Desktop_Qt_6_5_3_GCC_64bit-Debug"

cd "$BUILD_PATH" || exit 1

./cnca3d | tee test_results.log

echo "--------------------------------------------------------------------------"

total_passed=0
total_failed=0
total_skipped=0
total_blacklisted=0

while IFS= read -r line; do
    if [[ "$line" == *"Start testing of"* ]]; then
        test_class=$(echo "$line" | sed -E 's/.*Start testing of (.*) .*/\1/')
        printf "%-20s" "$test_class:"
    fi
    if [[ "$line" == *"Totals:"* ]]; then
    
        passed=$(echo "$line" | awk -F'passed,' '{print $1}' | awk '{print $NF}')
        failed=$(echo "$line" | awk -F'failed,' '{print $1}' | awk '{print $NF}')
        skipped=$(echo "$line" | awk -F'skipped,' '{print $1}' | awk '{print $NF}')
        blacklisted=$(echo "$line" | awk -F'blacklisted,' '{print $1}' | awk '{print $NF}')
        
        total_passed=$((total_passed + passed))
        total_failed=$((total_failed + failed))
        total_skipped=$((total_skipped + skipped))
        total_blacklisted=$((total_blacklisted + blacklisted))

        echo $(echo "$line" | sed -E 's/^Totals://')
    fi
done < test_results.log

echo -n "Totals:"
echo -n " $total_passed passed,"
echo -n " $total_failed failed,"
echo -n " $total_skipped skipped,"
echo " $total_blacklisted blacklisted"
echo "--------------------------------------------------------------------------"

cd - || exit 1
