# poll_wrapper2

极简 C 语言 `poll` 封装，面向小型网络程序，不依赖第三方网络库。  
`poll` 对文件描述符通用，因此天然支持 `AF_UNIX` socket。

## 目录

- `/home/runner/work/poll_wrapper2/poll_wrapper2/include/poll_wrapper.h`
- `/home/runner/work/poll_wrapper2/poll_wrapper2/src/poll_wrapper.c`
- `/home/runner/work/poll_wrapper2/poll_wrapper2/tests/test_unix_poll_wrapper.c`

## 构建与测试

```bash
cd /home/runner/work/poll_wrapper2/poll_wrapper2
make test
```