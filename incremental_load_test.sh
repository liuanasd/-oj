#!/bin/bash



URL="http://localhost:8080/"
INITIAL_CONCURRENCY=10      # Initial concurrency
STEP=50                     # Step size
MAX_CONCURRENCY=500         # Max concurrency
REQUESTS=500                # Requests per test
THRESHOLD_RESPONSE_TIME=3   # Response time threshold (seconds)
THRESHOLD_ERROR_RATE=5      # Error rate threshold (%)


REPORT_FILE="incremental_load_test_report.txt"
> $REPORT_FILE


START_TIME=$(date)
echo "Test start time: $START_TIME" >> $REPORT_FILE
echo "Test URL: $URL" >> $REPORT_FILE
echo "Initial concurrency: $INITIAL_CONCURRENCY"
echo "Step size: $STEP"
echo "Max concurrency: $MAX_CONCURRENCY"
echo "Requests per test: $REQUESTS"
echo "Response time threshold: ${THRESHOLD_RESPONSE_TIME}s"
echo "Error rate threshold: ${THRESHOLD_ERROR_RATE}%"
echo "" >> $REPORT_FILE
echo "=====================================" >> $REPORT_FILE
echo "Concurrency\tTotal Time(s)\tSuccess\tFailed\tError Rate(%)\tAvg Response Time(s)\tQPS" >> $REPORT_FILE
echo "=====================================" >> $REPORT_FILE


function run_test() {
    local concurrency=$1
    local log_file="temp_test_results.txt"
    > $log_file
    
    echo "Testing concurrency: $concurrency"
    
    # Record start time
    local test_start=$(date +%s)
    
    # Execute concurrent requests
    for ((i=1; i<=$REQUESTS; i++)); do
        curl -s -o /dev/null -w "%{http_code} %{time_total}\n" "$URL" >> $log_file &
        
        # Control concurrency
        if (( i % concurrency == 0 )); then
            wait
        fi
    done
    
    # Wait for all requests to complete
    wait
    
    # Record end time
    local test_end=$(date +%s)
    local total_time=$((test_end - test_start))
    
    # Analyze results
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
    
    # Calculate results
    local error_rate=0
    local avg_response_time=0
    local qps=0
    
    if (( REQUESTS > 0 )); then
        error_rate=$(echo "scale=2; $failed * 100 / $REQUESTS" | bc)
    fi
    
    if (( success > 0 )); then
        avg_response_time=$(echo "scale=3; $total_response_time / $success" | bc)
    fi
    
    if (( total_time > 0 )); then
        qps=$(echo "scale=2; $REQUESTS / $total_time" | bc)
    fi
    
    # Output results
    echo "$concurrency\t$total_time\t$success\t$failed\t$error_rate\t$avg_response_time\t$qps" >> $REPORT_FILE
    
    # Clean up temp file
    rm $log_file
    
    # Return key metrics
    echo "$avg_response_time $error_rate"
}


current_concurrency=$INITIAL_CONCURRENCY
while (( current_concurrency <= MAX_CONCURRENCY )); do
    # Run test
    result=$(run_test $current_concurrency)
    avg_response_time=$(echo $result | awk '{print $1}')
    error_rate=$(echo $result | awk '{print $2}')
    
    # Check if thresholds reached
    if (( $(echo "$avg_response_time > $THRESHOLD_RESPONSE_TIME" | bc -l) )) || \
       (( $(echo "$error_rate > $THRESHOLD_ERROR_RATE" | bc -l) )); then
        echo "" >> $REPORT_FILE
        echo "=====================================" >> $REPORT_FILE
        echo "Test stopped: Performance threshold reached"
        echo "Stopped at concurrency: $current_concurrency"
        echo "Average response time: ${avg_response_time}s (threshold: ${THRESHOLD_RESPONSE_TIME}s)"
        echo "Error rate: ${error_rate}% (threshold: ${THRESHOLD_ERROR_RATE}%)"
        echo "Test stopped: Performance threshold reached" >> $REPORT_FILE
        echo "Stopped at concurrency: $current_concurrency" >> $REPORT_FILE
        break
    fi
    
    # Increase concurrency
    current_concurrency=$((current_concurrency + STEP))
done


END_TIME=$(date)
echo "Test end time: $END_TIME" >> $REPORT_FILE


echo ""
echo "====================================="
echo "Test report generated: $REPORT_FILE"
echo "====================================="
cat $REPORT_FILE
