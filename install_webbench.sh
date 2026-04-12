#!/bin/bash
echo "开始安装webbench..."
echo "安装依赖包..."
yum install -y gcc make wget
echo "下载webbench源代码..."
wget http://home.tiscali.cz/~cz210552/webbench/webbench-1.5.tar.gz
echo "解压源代码..."
tar -zxvf webbench-1.5.tar.gz
echo "编译安装..."
cd webbench-1.5
make
make install
echo "验证安装..."
webbench --version
echo "清理安装文件..."
cd ..
rm -rf webbench-1.5 webbench-1.5.tar.gz
echo "webbench安装完成！"
echo "使用示例: webbench -c 100 -t 30 http://localhost:8080/"
