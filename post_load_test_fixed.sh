#!/bin/bash




URL="http://localhost:8080/"
OUTPUT_DIR="./post_load_test_results"
LOG_FILE="$OUTPUT_DIR/test_log.txt"
REPORT_FILE="$OUTPUT_DIR/test_report.txt"
RESULTS_FILE="$OUTPUT_DIR/results.txt"


CONCURRENCY=10              # 并发数
DURATION=10                # 持续时间（秒）
REQUEST_INTERVAL=0.1       # 请求间隔（秒）


REQUEST_BODY='{
  "code": "int main(){return 0;}",
  "input": "",
  "cpu": 1,
  "mem": 128
}'


mkdir -p $OUTPUT_DIR


> $LOG_FILE
> $REPORT_FILE
> $RESULTS_FILE


GREEN="\033[0;32m"
YELLOW="\033[1;33m"
RED="\033[0;31m"
NC="\033[0m" # No Color


function log() {
    local level=$1
    local message=$2
    local timestamp=$(date '+%Y-%m-%d %H:%M:%S')
    
    case $level in
        "INFO")
            echo -e "[$timestamp] ${GREEN}INFO${NC}: $message" >> $LOG_FILE
            echo -e "[$timestamp] ${GREEN}INFO${NC}: $message"
            ;;
        "WARNING")
            echo -e "[$timestamp] ${YELLOW}WARNING${NC}: $message" >> $LOG_FILE
            echo -e "[$timestamp] ${YELLOW}WARNING${NC}: $message"
            ;;
        "ERROR")
            echo -e "[$timestamp] ${RED}ERROR${NC}: $message" >> $LOG_FILE
            echo -e "[$timestamp] ${RED}ERROR${NC}: $message"
            ;;
        *)
            echo -e "[$timestamp] INFO: $message" >> $LOG_FILE
            echo -e "[$timestamp] INFO: $message"
            ;;
    esac
}


function send_request() {
    local start_time=$(date +%s.%N)
    local response=$(curl -s -w "%{http_code} %{time_total}" -X POST -H "Content-Type: application/json" -d "$REQUEST_BODY" "$URL")
    local end_time=$(date +%s.%N)
    local response_time=$(echo "$end_time - $start_time" | bc)
    local status=$(echo $response | awk '{print $1}')
    
    # 记录结果到文件
    echo "$status $response_time" >> $RESULTS_FILE
}


function concurrent_thread() {
    local end_time=$(( $(date +%s) + DURATION ))
    
    while (( $(date +%s) < end_time )); do
        send_request
        sleep $REQUEST_INTERVAL
    done
}


log "INFO" "开始POST接口压力测试..."
log "INFO" "测试URL: $URL"
log "INFO" "并发数: $CONCURRENCY"
log "INFO" "持续时间: ${DURATION}秒"
log "INFO" "请求间隔: ${REQUEST_INTERVAL}秒"


TEST_START=$(date +%s)


log "INFO" "启动并发线程..."
for ((i=1; i<=$CONCURRENCY; i++)); do
    concurrent_thread &
done


wait


TEST_END=$(date +%s)
TEST_DURATION=$((TEST_END - TEST_START))


TOTAL_REQUESTS=0
SUCCESS_COUNT=0
FAILED_COUNT=0
TOTAL_RESPONSE_TIME=0
MAX_RESPONSE_TIME=0

while read -r line; do
    local status=$(echo $line | awk '{print $1}')
    local response_time=$(echo $line | awk '{print $2}')
    
    ((TOTAL_REQUESTS++))
    
    if [[ "$status" == "200" ]]; then
        ((SUCCESS_COUNT++))
    else
        ((FAILED_COUNT++))
    fi
    
    TOTAL_RESPONSE_TIME=$(echo "$TOTAL_RESPONSE_TIME + $response_time" | bc)
    
    if (( $(echo "$response_time > $MAX_RESPONSE_TIME" | bc -l) )); then
        MAX_RESPONSE_TIME=$response_time
    fi
done < $RESULTS_FILE


SUCCESS_RATE=0
AVG_RESPONSE_TIME=0
QPS=0

if (( TOTAL_REQUESTS > 0 )); then
    SUCCESS_RATE=$(echo "scale=2; $SUCCESS_COUNT * 100 / $TOTAL_REQUESTS" | bc)
fi

if (( SUCCESS_COUNT > 0 )); then
    AVG_RESPONSE_TIME=$(echo "scale=3; $TOTAL_RESPONSE_TIME / $SUCCESS_COUNT" | bc)
fi

if (( TEST_DURATION > 0 )); then
    QPS=$(echo "scale=2; $TOTAL_REQUESTS / $TEST_DURATION" | bc)
fi


log "INFO" ""
log "INFO" "====================================="
log "INFO" "生成测试报告..."
log "INFO" "====================================="

echo "=====================================" >> $REPORT_FILE
echo "POST接口压力测试报告" >> $REPORT_FILE
echo "=====================================" >> $REPORT_FILE
echo "测试时间: $(date)" >> $REPORT_FILE
echo "测试URL: $URL" >> $REPORT_FILE
echo "并发数: $CONCURRENCY" >> $REPORT_FILE
echo "持续时间: ${TEST_DURATION}秒" >> $REPORT_FILE
echo "" >> $REPORT_FILE
echo "测试结果:" >> $REPORT_FILE
echo "=====================================" >> $REPORT_FILE
echo "总请求数: $TOTAL_REQUESTS" >> $REPORT_FILE
echo "成功率: ${SUCCESS_RATE}%" >> $REPORT_FILE
echo "失败数: $FAILED_COUNT" >> $REPORT_FILE
echo "平均响应时间: ${AVG_RESPONSE_TIME}秒" >> $REPORT_FILE
echo "最大响应时间: ${MAX_RESPONSE_TIME}秒" >> $REPORT_FILE
echo "QPS: ${QPS}" >> $REPORT_FILE
echo "" >> $REPORT_FILE


echo "=====================================" >> $REPORT_FILE
echo "分析结果:" >> $REPORT_FILE
echo "=====================================" >> $REPORT_FILE


if (( $(echo "$SUCCESS_RATE >= 95" | bc -l) )); then
    echo "系统稳定性: 稳定" >> $REPORT_FILE
    log "INFO" "系统稳定性: 稳定"
else
    echo "系统稳定性: 不稳定" >> $REPORT_FILE
    log "ERROR" "系统稳定性: 不稳定"
fi


if (( $(echo "$AVG_RESPONSE_TIME <= 0.1" | bc -l) )); then
    echo "性能状态: 优秀" >> $REPORT_FILE
    log "INFO" "性能状态: 优秀"
elif (( $(echo "$AVG_RESPONSE_TIME <= 0.5" | bc -l) )); then
    echo "性能状态: 良好" >> $REPORT_FILE
    log "INFO" "性能状态: 良好"
else
    echo "性能状态: 较差" >> $REPORT_FILE
    log "ERROR" "性能状态: 较差"
fi


echo "" >> $REPORT_FILE
echo "=====================================" >> $REPORT_FILE
echo "优化建议:" >> $REPORT_FILE
echo "=====================================" >> $REPORT_FILE
echo "1. 线程池优化: 调整服务器线程池大小，根据硬件资源合理配置并发线程数" >> $REPORT_FILE
echo "2. 异步处理: 对于编译运行等耗时操作，考虑使用异步处理模式，避免阻塞主线程" >> $REPORT_FILE
echo "3. 缓存机制: 对于重复的编译请求，考虑实现缓存机制，避免重复编译" >> $REPORT_FILE
echo "4. 资源限制: 合理设置CPU和内存限制，避免单个请求占用过多资源" >> $REPORT_FILE
echo "5. 负载均衡: 如果流量较大，考虑使用负载均衡机制分散请求压力" >> $REPORT_FILE
echo "6. 代码优化: 检查编译执行过程中的性能瓶颈，优化编译速度" >> $REPORT_FILE

echo "" >> $REPORT_FILE
echo "测试报告生成于: $REPORT_FILE" >> $REPORT_FILE


log "INFO" "测试报告已生成"
cat $REPORT_FILE

log "INFO" "POST接口压力测试完成！"
