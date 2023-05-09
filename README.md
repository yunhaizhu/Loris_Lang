[![Quality gate](https://sonarcloud.io/api/project_badges/quality_gate?project=yunhaizhu_Loris_Lang)](https://sonarcloud.io/summary/new_code?id=yunhaizhu_Loris_Lang)

# Loris_Lang
Loris is a ownership based memory safe dynamic programming language. 

Safe as Rust, Faster as Lua, Easy use as Python!

## Install packages
1. sudo apt-get install -y libunwind-dev libgoogle-perftools-dev libxxhash-dev libjson-c-dev libcjson-dev flex yacc libelf-dev libconfuse-dev fuse libfl-dev libgsl-dev python3-pip indent golang-go graphviz libcurl4-openssl-dev
2. pip3 install jinja2
3. go install github.com/google/pprof@latest

## Add Doxgen
find ./src -name *.[ch] |xargs python3 add_doxygen_comment.py

## Build and Run loris language
1. mkdir build
2. cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build
3. cmake --install build/
4. cd ./deploy/Debug/
5. ./Loris_Lang -b script/test_all/selfcheck.batch

## Performace finetune
Loris time cost: \
First time using interpreter mode, run cost 2.74s.   
Second time using binary mode, run cost 2.4226s

1. cd ./deploy/Debug/ 
2. ./Loris_Lang -s script/test_all/advanced/test_profile.ll 
3. ./Loris_Lang -s script/test_all/advanced/test_profile.ll 


Python 3 \
Total time taken: 11.582136392593384 seconds, j = 200000000
1. python3 script/test_all/advanced/profile.py


Lua 5.3.5 \
Total time taken: 2.45 seconds, j = 200000000
1. lua script/test_all/advanced/profile.lua
