#include "../../defer.h"
#include "hello.skel.h" // 1. 包含自动生成的骨架头文件
#include <bpf/libbpf.h>
#include <signal.h>
#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>

static volatile bool exiting = false;

static void sig_handler(int sig) { exiting = true; }

int main(int argc, char **argv) {
  struct hello_bpf *skel;
  int err;

  // 设置信号处理，方便 Ctrl+C 退出
  signal(SIGINT, sig_handler);
  signal(SIGTERM, sig_handler);

  // 2. 打开 (Open) 并 加载 (Load) BPF 程序
  // libbpf 帮你处理了字节码验证、重定位等复杂工作
  skel = hello_bpf__open_and_load();
  if (!skel) {
    fprintf(stderr, "Failed to open and load BPF skeleton\n");
    return 1;
  }

  defer(hello_bpf__destroy(skel););

  // 3. 挂载 (Attach) 到内核钩子
  // 这一步之后，BPF 程序开始在内核中运行
  err = hello_bpf__attach(skel);
  if (err) {
    fprintf(stderr, "Failed to attach BPF skeleton\n");
  }

  printf("Successfully started! Please run `sudo cat "
         "/sys/kernel/debug/tracing/trace_pipe` to see output.\n");

  // 4. 主循环：保持进程存活，直到用户退出
  while (!exiting) {
    sleep(1);
  }

  return 0;
}
