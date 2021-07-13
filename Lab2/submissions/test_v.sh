make clean
make
cd lab2_assign
./runit.sh ../sched_v_out ../sched -v
./gradeit.sh ./refout_v ../sched_v_out