[![Quality gate](https://sonarcloud.io/api/project_badges/quality_gate?project=yunhaizhu_Loris_Lang)](https://sonarcloud.io/summary/new_code?id=yunhaizhu_Loris_Lang)

# Loris_Lang
Loris is a ownership based memory safe dynamic programming language. 

Safe as Rust, Easy use like Python!

## Install packages
1. sudo apt-get install -y libunwind-dev libgoogle-perftools-dev libxxhash-dev libjson-c-dev libcjson-dev flex yacc libelf-dev libconfuse-dev fuse libfl-dev libgsl-dev python3-pip indent golang-go graphviz libcurl4-openssl-dev
2. pip3 install jinja2
3. go install github.com/google/pprof@latest

## Build and Run loris language
1. mkdir build
2. cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build
3. cmake --install build/ --config Release
4. cd ./deploy/Debug/
5. ./Loris_Lang -b script/test_all/selfcheck.batch
