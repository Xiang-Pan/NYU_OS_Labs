make clean
make
cd lab2_assign
./runit.sh ../sched_out ../sched -v
./gradeit.sh ./refout_v ../sched_out