
[中文](./README.zh.md)

# MicroFault Library

A lightweight, embedded fault management system for microcontroller applications. Provides efficient tracking and management of system faults with configurable severity levels.

## Features

- Register multiple fault codes with severity levels
- Enable/disable individual faults or all faults at once
- Query the highest severity fault level
- Configurable fault code bit width (8, 16, 32 bits)
- Configurable fault level ordering (normal or reversed)
- Minimal memory footprint suitable for embedded systems

## Quick Start

### 1. Initialize the System

```c
MicroFault_Status_t MicroFault_Init(void);
```

**Purpose:** Initialize the MicroFault module. Must be called once before any other operations.

**Parameters:** None

**Returns:**
- `MICROFAULT_STATUS_OK` - Initialization successful
- `MICROFAULT_STATUS_ERROR` - Initialization failed

**Example:**
```c
if (MicroFault_Init() == MICROFAULT_STATUS_OK) {
    // Ready to use
}
```

---

### 2. Register Faults

```c
MicroFault_Status_t MicroFault_ResgisterFault(
    const MicroFault_FaultTableEntry_t *table,
    size_t size
);
```

**Purpose:** Register a table of fault entries with their codes and severity levels.

**Parameters:**
- `table` - Pointer to an array of fault table entries
- `size` - Number of entries in the table (must be > 0 and ≤ `MICROFAULT_MAXNUMBER_OF_FAULTS`)

**Returns:**
- `MICROFAULT_STATUS_OK` - Faults registered successfully
- `MICROFAULT_STATUS_INVALID_PARAM` - Invalid parameters (NULL table, size 0, or size exceeds maximum)

**Example:**
```c
MicroFault_FaultTableEntry_t faults[] = {
    {MICROFAULT_LEVEL_MINOR, 0x01},      // Motor timeout
    {MICROFAULT_LEVEL_GENERAL, 0x02},    // Temperature high
    {MICROFAULT_LEVEL_SERIOUS, 0x03},    // Power loss
};

MicroFault_ResgisterFault(faults, 3);
```

---

### 3. Set a Fault

```c
MicroFault_Status_t MicroFault_SetFault(MicroFault_FaultCode_t code);
```

**Purpose:** Enable/activate a specific fault by its code.

**Parameters:**
- `code` - The fault code to activate

**Returns:**
- `MICROFAULT_STATUS_OK` - Fault enabled successfully
- `MICROFAULT_STATUS_NOT_FOUND` - No matching fault code found

**Example:**
```c
// Activate motor timeout fault
MicroFault_SetFault(0x01);
```

---

### 4. Clear a Fault

```c
MicroFault_Status_t MicroFault_ClearFault(MicroFault_FaultCode_t code);
```

**Purpose:** Disable/deactivate a specific fault by its code.

**Parameters:**
- `code` - The fault code to clear

**Returns:**
- `MICROFAULT_STATUS_OK` - Fault disabled successfully
- `MICROFAULT_STATUS_NOT_FOUND` - No matching fault code found

**Example:**
```c
// Clear motor timeout fault
MicroFault_ClearFault(0x01);
```

---

### 5. Clear All Faults

```c
MicroFault_Status_t MicroFault_ClearALL(void);
```

**Purpose:** Disable all active faults at once.

**Parameters:** None

**Returns:**
- `MICROFAULT_STATUS_OK` - All faults cleared successfully

**Example:**
```c
MicroFault_ClearALL();  // Reset all faults
```

---

### 6. Set All Faults

```c
MicroFault_Status_t MicroFault_SetALL(void);
```

**Purpose:** Enable all registered faults at once. Useful for testing.

**Parameters:** None

**Returns:**
- `MICROFAULT_STATUS_OK` - All faults enabled successfully

**Example:**
```c
MicroFault_SetALL();  // Activate all faults (test mode)
```

---

### 7. Timer Handler (Query Highest Fault Level)

```c
MicroFault_Status_t MicroFault_TimerHandler(MicroFault_FaultInfo_t *info);
```

**Purpose:** Query the highest severity fault level among all active faults. Typically called periodically (in a timer ISR).

**Parameters:**
- `info` - Pointer to output structure containing the highest fault level

**Returns:**
- `MICROFAULT_STATUS_OK` - Query successful
- `MICROFAULT_STATUS_INVALID_PARAM` - NULL pointer passed

**Example:**
```c
MicroFault_FaultInfo_t faultInfo;

if (MicroFault_TimerHandler(&faultInfo) == MICROFAULT_STATUS_OK) {
    switch (faultInfo.level) {
        case MICROFAULT_LEVEL_SERIOUS:
            // Handle serious fault
            break;
        case MICROFAULT_LEVEL_GENERAL:
            // Handle general fault
            break;
        case MICROFAULT_LEVEL_MINOR:
            // Handle minor fault
            break;
        case MICROFAULT_LEVEL_NONE:
            // No active faults
            break;
    }
}
```

---

## Return Values Reference

All functions return `MicroFault_Status_t`:

| Value | Meaning |
|-------|---------|
| `MICROFAULT_STATUS_OK` | Operation successful |
| `MICROFAULT_STATUS_ERROR` | Generic error |
| `MICROFAULT_STATUS_INVALID_PARAM` | Invalid parameter(s) |
| `MICROFAULT_STATUS_NOT_FOUND` | Fault code not found |

---

## Configuration

Edit `MicroFault_conf.h` to customize behavior:

- `MICROFAULT_FAULTCODE_BIT_WIDTH` - Bit width for fault codes (8, 16, or 32)
- `MICROFAULT_MAXNUMBER_OF_FAULTS` - Maximum concurrent faults
- `MICROFAULT_NUMBER_OF_USER_FAULTS` - Initial user fault count
- `MICROFAULT_LEVEL_ORDER_REVERSED` - Severity ordering (0=normal, 1=reversed)

---

## Complete Usage Example

```c
#include "MicroFault.h"

// Define your faults
MicroFault_FaultTableEntry_t system_faults[] = {
    {MICROFAULT_LEVEL_MINOR, 0x10},      // Warning
    {MICROFAULT_LEVEL_GENERAL, 0x20},    // Error
    {MICROFAULT_LEVEL_SERIOUS, 0x30},    // Critical
};

void init_fault_system(void) {
    MicroFault_Init();
    MicroFault_ResgisterFault(system_faults, 3);
}

void timer_isr(void) {
    MicroFault_FaultInfo_t info;
    
    if (MicroFault_TimerHandler(&info) == MICROFAULT_STATUS_OK) {
        if (info.level == MICROFAULT_LEVEL_SERIOUS) {
            // Emergency shutdown
            system_shutdown();
        }
    }
}

void main(void) {
    init_fault_system();
    
    // Simulate a fault
    MicroFault_SetFault(0x20);
    
    // Later, clear it
    MicroFault_ClearFault(0x20);
    
    // Or clear everything
    MicroFault_ClearALL();
}
```

---

## License

Copyright (c) 2025
