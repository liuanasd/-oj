#!/bin/bash




BASE_URL="http://localhost:8080"
OUTPUT_DIR="./load_test_results"
LOG_FILE="$OUTPUT_DIR/load_test_log.txt"
REPORT_FILE="$OUTPUT_DIR/load_test_report.txt"


INITIAL_CONCURRENCY=10      # 初始并发数
STEP=50                     # 递增步长
MAX_CONCURRENCY=500         # 最大并发数
REQUESTS_PER_TEST=500       # 每次测试的请求数
TEST_DURATION=30            # 测试持续时间（秒）


mkdir -p $OUTPUT_DIR


> $LOG_FILE
> $REPORT_FILE


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


TEST_RESULTS=()


function record_result() {
    local test_name=$1
    local concurrency=$2
    local total_time=$3
    local success=$4
    local failed=$5
    local error_rate=$6
    local avg_response_time=$7
    local qps=$8
    TEST_RESULTS+=("$test_name:$concurrency:$total_time:$success:$failed:$error_rate:$avg_response_time:$qps")
}


function run_load_test() {
    local endpoint=$1
    local concurrency=$2
    local test_name=$3
    local log_file="$OUTPUT_DIR/temp_${test_name}_${concurrency}.txt"
    > $log_file
    
    log "INFO" "测试 $test_name (并发数: $concurrency)"
    
    # 记录开始时间
    local test_start=$(date +%s)
    
    # 执行并发请求
    for ((i=1; i<=$REQUESTS_PER_TEST; i++)); do
        curl -s -o /dev/null -w "%{http_code} %{time_total}\n" "$BASE_URL$endpoint" >> $log_file &
        
        # 控制并发数
        if (( i % concurrency == 0 )); then
            wait
        fi
    done
    
    # 等待所有请求完成
    wait
    
    # 记录结束时间
    local test_end=$(date +%s)
    local total_time=$((test_end - test_start))
    
    # 分析结果
    local success=0
    local total_response_time=0
    local failed=0
    
    while read -r line; do
        local status=$(echo $line | awk '{print $1}')
        local time=$(echo $line | awk '{print $2}')
        
        if [[ "$status" == "200" ]]; then
            ((success++))
            total_response_time=$(echo "$total_response_time + $time" | bc)
        else
            ((failed++))
        fi
    done < $log_file
    
    # 计算结果
    local error_rate=0
    local avg_response_time=0
    local qps=0
    
    if (( REQUESTS_PER_TEST > 0 )); then
        error_rate=$(echo "scale=2; $failed * 100 / $REQUESTS_PER_TEST" | bc)
    fi
    
    if (( success > 0 )); then
        avg_response_time=$(echo "scale=3; $total_response_time / $success" | bc)
    fi
    
    if (( total_time > 0 )); then
        qps=$(echo "scale=2; $REQUESTS_PER_TEST / $total_time" | bc)
    fi
    
    # 输出结果
    log "INFO" "  总耗时: ${total_time}s, 成功: ${success}, 失败: ${failed}, 错误率: ${error_rate}%, 平均响应时间: ${avg_response_time}s, QPS: ${qps}"
    
    # 记录结果
    record_result "$test_name" "$concurrency" "$total_time" "$success" "$failed" "$error_rate" "$avg_response_time" "$qps"
    
    # 清理临时文件
    rm $log_file
}


log "INFO" "开始压测全流程..."
log "INFO" "测试URL: $BASE_URL"
log "INFO" "初始并发数: $INITIAL_CONCURRENCY"
log "INFO" "递增步长: $STEP"
log "INFO" "最大并发数: $MAX_CONCURRENCY"
log "INFO" "每次测试请求数: $REQUESTS_PER_TEST"


ENDPOINTS=(
    "/"
    "/question/1"
    "/judge/1"
)

ENDPOINT_NAMES=(
    "首页"
    "题目详情"
    "判题页面"
)


for ((i=0; i<${#ENDPOINTS[@]}; i++)); do
    local endpoint=${ENDPOINTS[$i]}
    local endpoint_name=${ENDPOINT_NAMES[$i]}
    
    log "INFO" ""
    log "INFO" "====================================="
    log "INFO" "测试端点: $endpoint_name ($endpoint)"
    log "INFO" "====================================="
    
    # 递增并发测试
    current_concurrency=$INITIAL_CONCURRENCY
    while (( current_concurrency <= MAX_CONCURRENCY )); do
        run_load_test "$endpoint" "$current_concurrency" "$endpoint_name"
        current_concurrency=$((current_concurrency + STEP))
    done
done


log "INFO" ""
log "INFO" "====================================="
log "INFO" "生成压测报告..."
log "INFO" "====================================="

echo "=====================================" >> $REPORT_FILE
echo "压测全流程测试报告" >> $REPORT_FILE
echo "=====================================" >> $REPORT_FILE
echo "测试时间: $(date)" >> $REPORT_FILE
echo "测试URL: $BASE_URL" >> $REPORT_FILE
echo "初始并发数: $INITIAL_CONCURRENCY" >> $REPORT_FILE
echo "递增步长: $STEP" >> $REPORT_FILE
echo "最大并发数: $MAX_CONCURRENCY" >> $REPORT_FILE
echo "每次测试请求数: $REQUESTS_PER_TEST" >> $REPORT_FILE
echo "" >> $REPORT_FILE


for ((i=0; i<${#ENDPOINTS[@]}; i++)); do
    local endpoint_name=${ENDPOINT_NAMES[$i]}
    echo "=====================================" >> $REPORT_FILE
    echo "端点: $endpoint_name" >> $REPORT_FILE
    echo "=====================================" >> $REPORT_FILE
    echo "并发数\t总耗时(s)\t成功\t失败\t错误率(%)\t平均响应时间(s)\tQPS" >> $REPORT_FILE
    echo "=====================================" >> $REPORT_FILE
    
    for result in "${TEST_RESULTS[@]}"; do
        IFS=":" read -r test_name concurrency total_time success failed error_rate avg_response_time qps <<< "$result"
        if [ "$test_name" == "$endpoint_name" ]; then
            echo "$concurrency\t$total_time\t$success\t$failed\t$error_rate\t$avg_response_time\t$qps" >> $REPORT_FILE
        fi
    done
    
echo "" >> $REPORT_FILE
done


echo "=====================================" >> $REPORT_FILE
echo "性能瓶颈分析" >> $REPORT_FILE
echo "=====================================" >> $REPORT_FILE


for ((i=0; i<${#ENDPOINTS[@]}; i++)); do
    local endpoint_name=${ENDPOINT_NAMES[$i]}
    local max_qps=0
    local best_concurrency=0
    
    for result in "${TEST_RESULTS[@]}"; do
        IFS=":" read -r test_name concurrency total_time success failed error_rate avg_response_time qps <<< "$result"
        if [ "$test_name" == "$endpoint_name" ]; then
            if (( $(echo "$qps > $max_qps" | bc -l) )); then
                max_qps=$qps
                best_concurrency=$concurrency
            fi
        fi
    done
    
    echo "$endpoint_name 最佳并发数: $best_concurrency, 最大QPS: $max_qps" >> $REPORT_FILE
done

echo "" >> $REPORT_FILE
echo "=====================================" >> $REPORT_FILE
echo "测试完成" >> $REPORT_FILE
echo "测试报告生成于: $REPORT_FILE" >> $REPORT_FILE


log "INFO" "压测报告已生成"
cat $REPORT_FILE

log "INFO" "压测全流程完成！"
