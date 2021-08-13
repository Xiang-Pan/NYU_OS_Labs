<!--
 * @Author: Xiang Pan
 * @Date: 2021-07-13 19:19:38
 * @LastEditTime: 2021-07-27 18:59:53
 * @LastEditors: Xiang Pan
 * @Description: 
 * @FilePath: /Lab3/README.md
 * xiangpan@nyu.edu
-->
# Lab3
Please ensure gcc-9.1 works properly!
```
make clean
make
cd lab3_assign/scripts
./runit.sh ../inputs ../../mmu_outs ../../mmu
./gradeit.sh ../../mmu_outs ../refout
```