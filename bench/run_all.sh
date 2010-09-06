
echo "\n"
echo "loop.rb"
echo "\n-------------------"
./run.sh loop.rb 1000000

echo "\n"
echo "nest_simple.rb"
echo "\n-------------------"
./run.sh nest_simple.rb 100000

echo "\n"
echo "nest_funcs.rb"
echo "\n-------------------"
./run.sh nest_funcs.rb 100000

echo "\n"
echo "nest_funcs_and_exit.rb"
echo "\n-------------------"
./run.sh nest_funcs_and_exit.rb 100000

echo "\n"
echo "if_exit_with_yield.rb"
echo "\n-------------------"
./run.sh if_exit_with_yield.rb 100000

echo "\n"
echo "bm_fact.rb"
echo "\n-------------------"
./run.sh bm_fact.rb 1000

echo "\n"
echo "bm_nbody.rb"
echo "\n-------------------"
./run.sh bm_nbody.rb 1000

