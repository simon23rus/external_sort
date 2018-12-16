set -e

echo compiling ...
g++ -std=c++14 external_sort.cpp -o external_sort
echo done!

echo start randomizing file...
python3 random_file_generator.py --lines-number 93335 --max-line-length 80 --output-filename generated_for_sort.txt
echo done!

echo start sort ...

./external_sort generated_for_sort.txt test_temp1.txt test_temp2.txt 1

echo done!

echo start checking sorting order...

python3 sorting_checker.py --input-filename generated_for_sort.txt

rm external_sort

echo end

