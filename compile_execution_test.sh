#!/bin/bash




TEST_DIR="./oj_server"
OUTPUT_DIR="$(pwd)/test_output"
LOG_FILE="$OUTPUT_DIR/test_log.txt"


mkdir -p $OUTPUT_DIR


> $LOG_FILE


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
    local status=$2
    local message=$3
    TEST_RESULTS+=("$test_name:$status:$message")
}


log "INFO" "开始编译测试..."


log "INFO" "  1.1 预处理测试"
cd $TEST_DIR
if g++ -E oj_server.cpp -o $OUTPUT_DIR/preprocessed.cpp -std=c++11; then
    log "INFO" "  预处理成功"
    record_result "预处理测试" "PASS" "预处理成功"
else
    log "ERROR" "  预处理失败"
    record_result "预处理测试" "FAIL" "预处理失败"
    exit 1
fi


log "INFO" "  1.2 编译测试"
if g++ -c oj_server.cpp -o $OUTPUT_DIR/oj_server.o -std=c++11; then
    log "INFO" "  编译成功"
    record_result "编译测试" "PASS" "编译成功"
else
    log "ERROR" "  编译失败"
    record_result "编译测试" "FAIL" "编译失败"
    exit 1
fi


log "INFO" "  1.3 完整编译链接测试"
if make -f makefile; then
    log "INFO" "  完整编译链接成功"
    record_result "完整编译测试" "PASS" "编译链接成功"
else
    log "ERROR" "  完整编译链接失败"
    record_result "完整编译测试" "FAIL" "编译链接失败"
    exit 1
fi


log "INFO" "2. 可执行文件生成验证"
if [ -f "oj_server" ] && [ -x "oj_server" ]; then
    log "INFO" "  可执行文件存在且可执行"
    # 检查文件大小
    file_size=$(ls -lh oj_server | awk '{print $5}')
    log "INFO" "  可执行文件大小: $file_size"
    record_result "可执行文件验证" "PASS" "可执行文件存在且可执行，大小: $file_size"
else
    log "ERROR" "  可执行文件不存在或不可执行"
    record_result "可执行文件验证" "FAIL" "可执行文件不存在或不可执行"
    exit 1
fi


log "INFO" "3. 程序启动测试"

./oj_server &
SERVER_PID=$!


sleep 2


if ps -p $SERVER_PID > /dev/null; then
    log "INFO" "  服务器启动成功，PID: $SERVER_PID"
    record_result "程序启动测试" "PASS" "服务器启动成功，PID: $SERVER_PID"
else
    log "ERROR" "  服务器启动失败"
    record_result "程序启动测试" "FAIL" "服务器启动失败"
    exit 1
fi


log "INFO" "4. 核心功能执行测试"


log "INFO" "  4.1 测试题目列表页面"
if curl -s -o $OUTPUT_DIR/index.html http://localhost:8080/ && [ -s "$OUTPUT_DIR/index.html" ]; then
    log "INFO" "  题目列表页面访问成功"
    record_result "题目列表测试" "PASS" "题目列表页面访问成功"
else
    log "ERROR" "  题目列表页面访问失败"
    record_result "题目列表测试" "FAIL" "题目列表页面访问失败"
fi


log "INFO" "  4.2 测试题目详情页面"
if curl -s -o $OUTPUT_DIR/question_1.html http://localhost:8080/question/1 && [ -s "$OUTPUT_DIR/question_1.html" ]; then
    log "INFO" "  题目详情页面访问成功"
    record_result "题目详情测试" "PASS" "题目详情页面访问成功"
else
    log "ERROR" "  题目详情页面访问失败"
    record_result "题目详情测试" "FAIL" "题目详情页面访问失败"
fi


log "INFO" "  4.3 测试判题页面"
if curl -s -o $OUTPUT_DIR/judge_1.html http://localhost:8080/judge/1 && [ -s "$OUTPUT_DIR/judge_1.html" ]; then
    log "INFO" "  判题页面访问成功"
    record_result "判题页面测试" "PASS" "判题页面访问成功"
else
    log "ERROR" "  判题页面访问失败"
    record_result "判题页面测试" "FAIL" "判题页面访问失败"
fi


log "INFO" "5. 异常处理测试"


log "INFO" "  5.1 测试不存在的题目"
response=$(curl -s -w "%{http_code}" -o $OUTPUT_DIR/error_404.html http://localhost:8080/question/999)
if [ "$response" == "200" ]; then
    log "INFO" "  不存在的题目处理成功"
    record_result "异常处理测试" "PASS" "不存在的题目处理成功"
else
    log "ERROR" "  不存在的题目处理失败，HTTP状态码: $response"
    record_result "异常处理测试" "FAIL" "不存在的题目处理失败"
fi


log "INFO" "6. 资源使用测试"


log "INFO" "  6.1 内存使用测试"
if ps -p $SERVER_PID -o rss= > $OUTPUT_DIR/memory_usage.txt; then
    memory_usage=$(cat $OUTPUT_DIR/memory_usage.txt)
    log "INFO" "  服务器内存使用: ${memory_usage} KB"
    record_result "内存使用测试" "PASS" "内存使用: ${memory_usage} KB"
else
    log "ERROR" "  内存使用测试失败"
    record_result "内存使用测试" "FAIL" "内存使用测试失败"
fi


log "INFO" "  6.2 CPU使用测试"
if ps -p $SERVER_PID -o %cpu= > $OUTPUT_DIR/cpu_usage.txt; then
    cpu_usage=$(cat $OUTPUT_DIR/cpu_usage.txt)
    log "INFO" "  服务器CPU使用: ${cpu_usage}%"
    record_result "CPU使用测试" "PASS" "CPU使用: ${cpu_usage}%"
else
    log "ERROR" "  CPU使用测试失败"
    record_result "CPU使用测试" "FAIL" "CPU使用测试失败"
fi


log "INFO" "7. 资源释放验证"


kill $SERVER_PID


sleep 2


if ! ps -p $SERVER_PID > /dev/null; then
    log "INFO" "  服务器成功关闭，资源释放"
    record_result "资源释放验证" "PASS" "服务器成功关闭，资源释放"
else
    log "ERROR" "  服务器未成功关闭"
    record_result "资源释放验证" "FAIL" "服务器未成功关闭"
    # 强制关闭
    kill -9 $SERVER_PID
fi


log "INFO" "8. 生成测试报告"

REPORT_FILE="$OUTPUT_DIR/test_report.txt"
> $REPORT_FILE

echo "=====================================" >> $REPORT_FILE
echo "编译执行全流程测试报告" >> $REPORT_FILE
echo "=====================================" >> $REPORT_FILE
echo "测试时间: $(date)" >> $REPORT_FILE
echo "" >> $REPORT_FILE
echo "测试结果:" >> $REPORT_FILE
echo "=====================================" >> $REPORT_FILE

PASS_COUNT=0
FAIL_COUNT=0

for result in "${TEST_RESULTS[@]}"; do
    IFS=":" read -r test_name status message <<< "$result"
    echo "$test_name: $status - $message" >> $REPORT_FILE
    if [ "$status" == "PASS" ]; then
        ((PASS_COUNT++))
    else
        ((FAIL_COUNT++))
    fi
done

echo "=====================================" >> $REPORT_FILE
echo "测试统计:" >> $REPORT_FILE
echo "总测试项: ${#TEST_RESULTS[@]}" >> $REPORT_FILE
echo "通过: $PASS_COUNT" >> $REPORT_FILE
echo "失败: $FAIL_COUNT" >> $REPORT_FILE

if [ $FAIL_COUNT -eq 0 ]; then
    echo "" >> $REPORT_FILE
    echo "测试结果: ${GREEN}全部通过${NC}" >> $REPORT_FILE
    log "INFO" "全部测试通过！"
else
    echo "" >> $REPORT_FILE
    echo "测试结果: ${RED}部分失败${NC}" >> $REPORT_FILE
    log "ERROR" "部分测试失败！"
fi

echo "" >> $REPORT_FILE
echo "测试报告生成于: $REPORT_FILE" >> $REPORT_FILE


log "INFO" "测试报告已生成"
cat $REPORT_FILE


cd ..


if [ $FAIL_COUNT -eq 0 ]; then
    exit 0
else
    exit 1
fi
