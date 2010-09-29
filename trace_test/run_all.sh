cd ~/src/misc/rubinius/trace_test
RBX=../bin/rbx
LOG=test.log

echo "" > $LOG


echo ""
echo "sqrt.rb (digs into stdlib)"
echo "-------------------"
$RBX sqrt.rb 10000 trace >> $LOG

echo ""
echo "sqrt_int.rb (setup_unwind,pop_unwind, nothing thrown)"
echo "-------------------"
$RBX sqrt_int.rb 10000 trace >> $LOG

echo ""
echo "loop.rb"
echo "-------------------"
$RBX loop.rb 1000000 trace >> $LOG

echo ""
echo "nest_simple.rb"
echo "-------------------"
$RBX nest_simple.rb 100000 trace >> $LOG

echo ""
echo "nest_funcs.rb"
echo "-------------------"
$RBX nest_funcs.rb 10000 trace >> $LOG

echo ""
echo "nest_funcs_and_exit.rb"
echo "-------------------"
$RBX nest_funcs_and_exit.rb 10000 trace >> $LOG

echo ""
echo "if_exit.rb"
echo "-------------------"
$RBX if_exit.rb 100000 trace >> $LOG

echo ""
echo "if_exit_with_yield.rb"
echo "-------------------"
$RBX if_exit_with_yield.rb 100000 trace >> $LOG

echo ""
echo "if_exit_with_yield_simple.rb"
echo "-------------------"
$RBX if_exit_with_yield_simple.rb 100000 trace >> $LOG

echo ""
echo "if_exit_in_func.rb"
echo "-------------------"
$RBX if_exit_in_func.rb 100000 trace >> $LOG

echo ""
echo "if_exit_in_two_funcs.rb"
echo "-------------------"
$RBX if_exit_in_two_funcs.rb 100000 trace >> $LOG


