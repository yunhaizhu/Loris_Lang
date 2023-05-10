[![Quality gate](https://sonarcloud.io/api/project_badges/quality_gate?project=yunhaizhu_Loris_Lang)](https://sonarcloud.io/summary/new_code?id=yunhaizhu_Loris_Lang)

# Loris_Lang
Loris is a ownership based memory safe dynamic programming language. 

Safe as Rust, Faster as Lua, Easy use as Python!

## Install packages
1. sudo apt-get install -y libunwind-dev libgoogle-perftools-dev libxxhash-dev libjson-c-dev libcjson-dev flex yacc libelf-dev libconfuse-dev fuse libfl-dev libgsl-dev python3-pip indent golang-go graphviz libcurl4-openssl-dev
2. pip3 install jinja2
3. go env -w GOPROXY=direct
4. go env -w GOSUMDB=off
5. go install github.com/google/pprof@latest
6. sudo apt install graphviz

## Add Doxgen
find ./src -name *.[ch] |xargs python3 add_doxygen_comment.py

## Build and Run loris language
1. mkdir build
2. cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
3. cmake --build build/
3. cmake --install build/
4. cd ./deploy/Release/
5. ./Loris_Lang -b script/test_all/selfcheck.batch

## Performance finetune
Loris time cost: \
First time using interpreter mode, time cost:20935.9490ms 20.9359s \
Second time using compiled binary mode, time cost:21721.8899ms 21.7219s

1. cd ./deploy/Release/ 
2. ./Loris_Lang -s script/test_all/advanced/test_profile.ll 
3. ./Loris_Lang -s script/test_all/advanced/test_profile.ll 


Python 3 \
Total time taken: 116.1728925704956 seconds, j = 2000000000
1. python3 script/test_all/advanced/profile.py


Lua 5.3.5 \
TTotal time taken: 22.37 seconds, j = 2000000000
1. lua script/test_all/advanced/profile.lua
