#include "vmlinux.h"         // 1. 包含内核所有数据结构定义 (CO-RE 核心)
#include <bpf/bpf_helpers.h> // 2. 包含 bpf_printk 等辅助函数

char LICENSE[] SEC("license") =
    "Dual BSD/GPL"; // 3. 必须声明 License，否则无法加载

// 4. 定义挂载点: 跟踪 execve 系统调用的入口
SEC("tp/syscalls/sys_enter_execve")
int handle_execve(void *ctx) {
  int pid = bpf_get_current_pid_tgid() >> 32; // 获取 PID

  // 5. 打印日志到 /sys/kernel/debug/tracing/trace_pipe
  // 注意：bpf_printk 类似于内核的 printf，方便调试
  bpf_printk("Hello eBPF! New process triggered by PID %d\n", pid);

  return 0;
}
