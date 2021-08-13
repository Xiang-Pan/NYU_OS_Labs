###
 # @Author: Xiang Pan
 # @Date: 2021-08-12 18:35:36
 # @LastEditTime: 2021-08-12 20:00:43
 # @LastEditors: Xiang Pan
 # @Description: 
 # @FilePath: /Lab4/test_generated_samples.sh
 # xiangpan@nyu.edu
### 
function rand(){
    min=$1
    max=$(($2-$min+1))
    num=$(($RANDOM+1000000000)) #增加一个10位的数再求余
    echo $(($num%$max+$min))
}

function random_float() {
  local precision=$1
  local scale=$2
  local random=$( echo "${RANDOM}/${RANDOM_MAX_INT}*(10^${precision}) + ${RANDOM}/${RANDOM_MAX_INT} " | bc -l )
  printf "%.${scale}f" ${random}
    # echo $(($random))
}

num_ios=$(rand 1 9999)
echo $num_ios

track_num=$(rand 1 1999)
echo $track_num

lambda=$(seq 0 .01 10 | shuf | head -n1)
echo $lambda

interarrival_factor=$(seq 0 .01 2 | shuf | head -n1)
echo $interarrival_factor

./iomake -t $track_num -i $num_ios -L $lambda -f $interarrival_factor > ./generated_samples/random_samples
echo "./iomake -t $track_num -i $num_ios -L $lambda -f $interarrival_factor > ./generated_samples/random_samples"

SCHED="./iosched"
OUTDIR="./generated_samples_outs"

SCHED_REF="./iosched_ref"
OUTDIR_REF="./generated_samples_outs_ref"


SCHEDS="i j s c f"

for s in ${SCHEDS}; do 
    f="random_samples"
    echo "${SCHED} -s${s} ./generated_samples/${f}"
    ${SCHED} -s${s} -v ./generated_samples/${f} > ${OUTDIR}/${f}_${s} 
    ${SCHED_REF} -s${s} -v ./generated_samples/${f} > ${OUTDIR_REF}/${f}_${s} 
    echo "diff ${OUTDIR}/${f}_${s} ${OUTDIR}/${f}_${s}"
    diff ${OUTDIR_REF}/${f}_${s} ${OUTDIR}/${f}_${s} >> ./generated_samples_diff/${f}_${s}
    # diff ${OUTDIR_REF}/${f}_${s} ${OUTDIR}/${f}_${s} 
done



# # ./runit.sh  ./iosched_out_v ../iosched -v
# # ./gradeit.sh ./lab4_ref_v ../iosched_out_v