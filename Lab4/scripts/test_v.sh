###
 # @Author: Xiang Pan
 # @Date: 2021-07-21 01:16:31
 # @LastEditTime: 2021-07-21 01:19:13
 # @LastEditors: Xiang Pan
 # @Description: 
 # @FilePath: /Lab4/test_v.sh
 # xiangpan@nyu.edu
### 
cd lab4_assign
./runit.sh  ../iosched_out_v ../iosched -v
./gradeit.sh ./lab4_ref_v ../iosched_out_v