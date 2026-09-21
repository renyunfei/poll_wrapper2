# poll_wrapper2

极简 C 语言 `poll` 封装，面向小型网络程序，不依赖第三方网络库。  
`poll` 对文件描述符通用，因此天然支持 `AF_UNIX` socket。  
实现保持简洁，内部按线性表管理 fd，适合小规模连接场景。

## 目录

- `include/poll_wrapper.h`
- `src/poll_wrapper.c`
- `tests/test_unix_poll_wrapper.c`

## 构建与测试

```bash
cd poll_wrapper2
make test
```