for i in {1..1000}
do
  echo "Test(" $i ") ................................................. :-" >> final_result.out
  ./a.out >> final_result.out
done
