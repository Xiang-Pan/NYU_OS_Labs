###
 # @Author: Xiang Pan
 # @Date: 2021-07-19 16:31:16
 # @LastEditTime: 2021-07-27 18:54:56
 # @LastEditors: Xiang Pan
 # @Description: 
 # @FilePath: /Lab3/test.sh
 # xiangpan@nyu.edu
### 
make clean
make
cd lab3_assign/scripts
./runit.sh ../inputs ../../mmu_outs ../../mmu
./gradeit.sh ../../mmu_outs ../refout

# ./lab3_assign/scripts/runit.sh ./lab3_assign/inputs ./mmu_outs ./mmu
# ./lab3_assign/scripts/gradeit.sh ./lab3_assign/refout ./mmu_outs