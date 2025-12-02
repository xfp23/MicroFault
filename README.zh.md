
[En](./readme.md)

# MicroFault 库

一个轻量级的嵌入式故障管理系统，专为微控制器应用设计。提供高效的故障追踪和管理功能，支持可配置的严重级别。

## 特性

- 注册多个故障代码及其严重级别
- 启用/禁用单个故障或全部故障
- 查询最高严重级别的故障
- 可配置的故障代码位宽（8、16、32 位）
- 可配置的故障级别排序（正序或反序）
- 极小的内存占用，适合嵌入式系统

## 快速开始

### 1. 初始化系统

```c
MicroFault_Status_t MicroFault_Init(void);
```

**功能：** 初始化 MicroFault 模块。必须在使用任何其他函数前调用一次。

**参数：** 无

**返回值：**
- `MICROFAULT_STATUS_OK` - 初始化成功
- `MICROFAULT_STATUS_ERROR` - 初始化失败

**示例：**
```c
if (MicroFault_Init() == MICROFAULT_STATUS_OK) {
    // 准备就绪
}
```

---

### 2. 注册故障

```c
MicroFault_Status_t MicroFault_ResgisterFault(
    const MicroFault_FaultTableEntry_t *table,
    size_t size
);
```

**功能：** 注册故障表，包含故障代码和严重级别。

**参数：**
- `table` - 指向故障表项数组的指针
- `size` - 表中的项数（必须 > 0 且 ≤ `MICROFAULT_MAXNUMBER_OF_FAULTS`）

**返回值：**
- `MICROFAULT_STATUS_OK` - 故障注册成功
- `MICROFAULT_STATUS_INVALID_PARAM` - 参数无效（NULL 指针、大小为 0 或超过最大值）

**示例：**
```c
MicroFault_FaultTableEntry_t faults[] = {
    {MICROFAULT_LEVEL_MINOR, 0x01},      // 电机超时
    {MICROFAULT_LEVEL_GENERAL, 0x02},    // 温度过高
    {MICROFAULT_LEVEL_SERIOUS, 0x03},    // 电源断开
};

MicroFault_ResgisterFault(faults, 3);
```

---

### 3. 设置故障

```c
MicroFault_Status_t MicroFault_SetFault(MicroFault_FaultCode_t code);
```

**功能：** 通过故障代码启用/激活特定故障。

**参数：**
- `code` - 要激活的故障代码

**返回值：**
- `MICROFAULT_STATUS_OK` - 故障启用成功
- `MICROFAULT_STATUS_NOT_FOUND` - 未找到匹配的故障代码

**示例：**
```c
// 激活电机超时故障
MicroFault_SetFault(0x01);
```

---

### 4. 清除故障

```c
MicroFault_Status_t MicroFault_ClearFault(MicroFault_FaultCode_t code);
```

**功能：** 通过故障代码禁用/停用特定故障。

**参数：**
- `code` - 要清除的故障代码

**返回值：**
- `MICROFAULT_STATUS_OK` - 故障禁用成功
- `MICROFAULT_STATUS_NOT_FOUND` - 未找到匹配的故障代码

**示例：**
```c
// 清除电机超时故障
MicroFault_ClearFault(0x01);
```

---

### 5. 清除全部故障

```c
MicroFault_Status_t MicroFault_ClearALL(void);
```

**功能：** 一次性禁用所有活跃故障。

**参数：** 无

**返回值：**
- `MICROFAULT_STATUS_OK` - 所有故障清除成功

**示例：**
```c
MicroFault_ClearALL();  // 重置所有故障
```

---

### 6. 设置全部故障

```c
MicroFault_Status_t MicroFault_SetALL(void);
```

**功能：** 一次性启用所有已注册的故障。对测试很有用。

**参数：** 无

**返回值：**
- `MICROFAULT_STATUS_OK` - 所有故障启用成功

**示例：**
```c
MicroFault_SetALL();  // 激活所有故障（测试模式）
```

---

### 7. 定时器处理程序（查询最高故障级别）

```c
MicroFault_Status_t MicroFault_TimerHandler(MicroFault_FaultInfo_t *info);
```

**功能：** 查询所有活跃故障中最高的严重级别。通常在定时器中周期性调用。

**参数：**
- `info` - 指向输出结构的指针，包含最高故障级别

**返回值：**
- `MICROFAULT_STATUS_OK` - 查询成功
- `MICROFAULT_STATUS_INVALID_PARAM` - 传递的指针为 NULL

**示例：**
```c
MicroFault_FaultInfo_t faultInfo;

if (MicroFault_TimerHandler(&faultInfo) == MICROFAULT_STATUS_OK) {
    switch (faultInfo.level) {
        case MICROFAULT_LEVEL_SERIOUS:
            // 处理严重故障
            break;
        case MICROFAULT_LEVEL_GENERAL:
            // 处理一般故障
            break;
        case MICROFAULT_LEVEL_MINOR:
            // 处理轻微故障
            break;
        case MICROFAULT_LEVEL_NONE:
            // 无活跃故障
            break;
    }
}
```

---

## 返回值参考

所有函数都返回 `MicroFault_Status_t`：

| 值 | 含义 |
|-----|------|
| `MICROFAULT_STATUS_OK` | 操作成功 |
| `MICROFAULT_STATUS_ERROR` | 通用错误 |
| `MICROFAULT_STATUS_INVALID_PARAM` | 参数无效 |
| `MICROFAULT_STATUS_NOT_FOUND` | 未找到故障代码 |

---

## 配置

编辑 `MicroFault_conf.h` 来自定义行为：

- `MICROFAULT_FAULTCODE_BIT_WIDTH` - 故障代码位宽（8、16 或 32）
- `MICROFAULT_MAXNUMBER_OF_FAULTS` - 最大并发故障数
- `MICROFAULT_NUMBER_OF_USER_FAULTS` - 初始用户故障数
- `MICROFAULT_LEVEL_ORDER_REVERSED` - 严重级别排序（0=正序，1=反序）

---

## 完整使用示例

```c
#include "MicroFault.h"

// 定义故障
MicroFault_FaultTableEntry_t system_faults[] = {
    {MICROFAULT_LEVEL_MINOR, 0x10},      // 警告
    {MICROFAULT_LEVEL_GENERAL, 0x20},    // 错误
    {MICROFAULT_LEVEL_SERIOUS, 0x30},    // 严重
};

void init_fault_system(void) {
    MicroFault_Init();
    MicroFault_ResgisterFault(system_faults, 3);
}

void timer_isr(void) {
    MicroFault_FaultInfo_t info;
    
    if (MicroFault_TimerHandler(&info) == MICROFAULT_STATUS_OK) {
        if (info.level == MICROFAULT_LEVEL_SERIOUS) {
            // 紧急关闭
            system_shutdown();
        }
    }
}

void main(void) {
    init_fault_system();
    
    // 模拟故障
    MicroFault_SetFault(0x20);
    
    // 之后，清除它
    MicroFault_ClearFault(0x20);
    
    // 或清除所有故障
    MicroFault_ClearALL();
}
```

---

## 许可证

版权所有 (c) 2025
