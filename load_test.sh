#!/bin/bash


URL="http://localhost:8080/"
REQUESTS=1000
CONCURRENCY=100


LOG_FILE="load_test_results.txt"
> $LOG_FILE


function send_request() {
    curl -s -o /dev/null -w "%{http_code} %{time_total}\n" "$URL"
}


echo "开始压测 $URL"
echo "请求数: $REQUESTS"
echo "并发数: $CONCURRENCY"
echo "开始时间: $(date)"


START_TIME=$(date +%s)


for ((i=1; i<=$REQUESTS; i++)); do
    send_request >> $LOG_FILE &
    
    # 控制并发数
    if (( i % CONCURRENCY == 0 )); then
        wait
    fi
done


wait


END_TIME=$(date +%s)
TOTAL_TIME=$((END_TIME - START_TIME))


SUCCESS=0
TOTAL_RESPONSE_TIME=0

while read -r line; do
    STATUS=$(echo $line | awk '{print $1}')
    TIME=$(echo $line | awk '{print $2}')
    
    if [[ "$STATUS" == "200" ]]; then
        ((SUCCESS++))
        TOTAL_RESPONSE_TIME=$(echo "$TOTAL_RESPONSE_TIME + $TIME" | bc)
    fi
done < $LOG_FILE


if (( SUCCESS > 0 )); then
    AVG_RESPONSE_TIME=$(echo "scale=3; $TOTAL_RESPONSE_TIME / $SUCCESS" | bc)
else
    AVG_RESPONSE_TIME=0
fi

QPS=$(echo "scale=2; $REQUESTS / $TOTAL_TIME" | bc)


echo "结束时间: $(date)"
echo "总耗时: $TOTAL_TIME 秒"
echo "成功请求: $SUCCESS"
echo "失败请求: $((REQUESTS - SUCCESS))"
echo "平均响应时间: $AVG_RESPONSE_TIME 秒"
echo "QPS: $QPS"


rm $LOG_FILE
