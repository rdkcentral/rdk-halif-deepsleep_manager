# Deep Sleep Manager HAL Documentation

- [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
- [Description](#description)
- [Component Runtime Execution Requirements](#component-runtime-execution-requirements)
  - [Initialization and Startup](#initialization-and-startup)
  - [Threading Model](#threading-model)
  - [Process Model](#process-model)
  - [Memory Model](#memory-model)
  - [Power Management Requirements](#power-management-requirements)
  - [Asynchronous Notification Model](#asynchronous-notification-model)
  - [Blocking calls](#blocking-calls)
  - [Internal Error Handling](#internal-error-handling)
  - [Persistence Model](#persistence-model)
- [Non-functional requirements](#non-functional-requirements)
  - [Logging and debugging requirements](#logging-and-debugging-requirements)
  - [Memory and performance requirements](#memory-and-performance-requirements)
  - [Quality Control](#quality-control)
  - [Licensing](#licensing)
  - [Build Requirements](#build-requirements)
  - [Variability Management](#variability-management)
  - [Platform or Product Customization](#platform-or-product-customization)
- [Interface API Documentation](#interface-api-documentation)
  - [Theory of operation and key concepts](#theory-of-operation-and-key-concepts)
  - [Sequence Diagram](#sequence-diagram)

## Acronyms, Terms and Abbreviations

- `HAL`:    Hardware Abstraction Layer
- `CPE`:    Customer Premises Equipment
- `IR`:     InfraRed
- `CEC`:    Consumer Electronic Control
- `LAN`:    Local Area Network
- `STB`:    Set-top Box
- `RCU`:    Remote Control Unit
- `STR`:    Suspend To RAM
- `HDMI`:   High-Definition Multimedia Interface
- `GPIO`:   General Purpose Input/OutputManufacturers
- `A/V`:    Audio/Video
- `HDD`:    Hard Drive Disk

## Description

The diagram below describes a high-level software architecture of the module stack.

```mermaid
%%{ init : { "theme" : "forest", "flowchart" : { "curve" : "linear" }}}%%
flowchart TD
y[Caller]<-->x[Deep Sleep Manager HAL];
x[Deep Sleep Manager HAL]<-->z[SOC Drivers];
style y fill:#99CCFF,stroke:#333,stroke-width:0.3px,align:left
style z fill:#fcc,stroke:#333,stroke-width:0.3px,align:left
style x fill:#9f9,stroke:#333,stroke-width:0.3px,align:left
 ```

The Deep Sleep Manager `HAL` provides a set of `APIs` to initialize, set the deep sleep state and wake-up from deep sleep state.

Deep sleep is a power saving mode which turns off `STB` subsystems such as A/V, `HDMI`, front panels, `HDD` etc.

- The main purpose is to bring down the power consumption based on the actual usage. Also, the conditions for triggering and setting deep sleep mode will depend on the product requirements.
- It can also be triggered in other scenarios such as thermal shutdown, in case the temperature is above the threshold for a certain period of time.
- When the `STB` goes into deep sleep mode, it may be woken up for scheduled maintenance in a pre-determined time.

## Component Runtime Execution Requirements

### Initialization and Startup

`Caller` must initialize by calling `PLAT_DS_INIT()` before calling any other `API`.

### Threading Model

This interface is not required to be thread safe. Any `caller`, while invoking these `HAL` `APIs` must ensure calls are made in a thread safe manner.

### Process Model

This interface is not required to be thread safe. Any `caller`, while invoking these `HAL` `APIs` must ensure calls are made in a thread safe manner.

### Memory Model

The `caller` is responsible to pass message buffer and free it for transmit request.

### Power Management Requirements

The Deep sleep manager `HAL` is involved in the power management operation:

- Transitions to Deep Sleep state which puts system into halt
- Wake up the system from deep sleep state by IR/RF, Bluetooth remote or through LAN wake-up based on the platform requirements

### Asynchronous Notification Model

This interface is not required to support asynchronous notification.

### Blocking calls

The following `APIs` are the blocking calls of this module:

- PLAT_DS_SetDeepSleep()

   This `API` call puts the system into halt state

- PLAT_DS_DeepSleepWakeup()

   This `API` call brings the system out of the halt state

All other synchronous `API` calls must complete within a reasonable time period. Any call that can fail due to the lack of response from the connected device must have a timeout period and the function must return the relevant error code.

### Internal Error Handling

All the `APIs` must return error synchronously as a return argument. `HAL` is responsible for handling system errors (e.g. out of memory) internally.

### Persistence Model

There is no requirement for the interface to persist any setting information. `caller` is responsible to persist any settings related to the `HAL`.

## Non functional requirements

### Logging and debugging requirements

This interface is required to support DEBUG, INFO and ERROR messages. INFO and DEBUG must be disabled by default and enabled when required.

### Memory and performance requirements

This interface is required to not cause excessive memory and CPU utilization.

### Quality Control

- This interface is required to perform static analysis, our preferred tool is Coverity.
- Have a zero-warning policy with regards to compiling. All warnings are required to be treated as errors.
- Copyright validation is required to be performed, e.g.: Black duck, FossID.
- Use of memory analysis tools like Valgrind are encouraged to identify leaks/corruptions.
- `HAL` Tests will endeavour to create worst case scenarios to assist investigations.
- Improvements by any party to the testing suite are required to be fed back.

### Licensing

The `HAL` implementation is expected to released under the Apache License 2.0.

### Build Requirements

The source code must build into a shared library and must be named as `libiarmmgrs-deepsleep-hal.so`. The build mechanism must be independent of Yocto.

### Variability Management

Any changes in the `APIs` must be reviewed and approved by the component architects.

### Platform or Product Customization

This interface is not required to have any platform or product customizations.

## Interface API Documentation

`API` documentation will be provided by Doxygen which will be generated from the header files.

### Theory of operation and key concepts

The `caller` is expected to have complete control over the life cycle of the `HAL`.

- Initialize the `HAL` using function: `PLAT_DS_INIT()` before making any other `API` calls.  If `PLAT_DS_INIT()` call fails, the `HAL` must return the respective error code, so that the `caller` can retry the operation

- Deep sleep state can be controlled using the function `PLAT_DS_SetDeepSleep()`

- Any post-processing after wake up can be performed using the function `PLAT_DS_DeepSleepWakeup()`

- Reason for last wake up can be queried using the function `PLAT_DS_GetLastWakeupReason()`

- Reason for last wake up keycode can be queried using the function `PLAT_DS_GetLastWakeupKeyCode()`

- De-initialize the `HAL` using the function: `PLAT_DS_TERM()`

#### **Sequence Diagram**

```mermaid
%%{ init : { "theme" : "default", "flowchart" : { "curve" : "stepBefore" }}}%%
   sequenceDiagram
    participant Caller as Caller
    participant HAL as Deep Sleep Manager HAL
    participant Driver as HAL Device Control/Driver
    Caller->>HAL:PLAT_DS_INIT()
    HAL-->>Caller:return
    Caller->>HAL:PLAT_DS_SetDeepSleep()
    Note over HAL: Deep sleep is set using this call
    HAL->>Driver: Set Deepsleep
    Driver-->>HAL: Return
    HAL-->>HAL: Waiting for device to sleep.
    Note over HAL: After few seconds CPU will freeze for all modules here
    Driver-->>Driver: Wake up trigger. (IR, CEC, etc)
    Note over HAL: CPU resumes for all modules
    HAL-->>Caller:return
    Caller->>HAL:PLAT_DS_DeepSleepWakeup()
    Note over HAL: Set the platform status after deepsleep wake-up.
    HAL-->>Caller:return
    Caller->>HAL:PLAT_DS_GetLastWakeupReason()
    Note over HAL: Reason for last wake up is returned using this call
    HAL-->>Caller:return
    Caller->>HAL:PLAT_DS_GetLastWakeupKeyCode()
    Note over HAL: Reason for last wake up keycode is returned using this call
    HAL-->>Caller:return
    Caller ->>HAL:PLAT_DS_TERM()
    HAL-->>Caller:return
 ```
