# C++20 Embedded ISR Abstraction Design Specification

## 1. Overview
High-performance, zero-overhead abstraction for Hardware Interrupt Service Routines (ISR) using **Policy-based Design** and **C++20 Concepts**.
The design must eliminate all runtime branches and function call overheads by utilizing **Class Non-Type Template Parameters (NTTP)** to pass stateless lambdas directly as compile-time constants.

## 2. Requirements & Constraints

*   **Language Standard**: C++20 (Required for passing lambdas as template arguments).
*   **Performance**:
    *   Absolute Zero-Overhead.
    *   All policies must be resolved at compile-time.
    *   All execution methods must be forced inline (`__attribute__((always_inline))` or equivalent).
*   **Validation**:
    *   Missing event definitions in the configuration struct must be detected via `static_assert` and `concept` immediately after the definition.
*   **Structure**:
    *   **Hardware Class**: A template class taking a `Config` type.
    *   **Configuration**: A user-defined struct using `using` aliases to map Events to Policies.
    *   **Policies**: `Direct`, `Defer`, `Ignore`.

## 3. Core Component Definitions

### 3.1. Policy Templates
Policies define how an event is handled. They must receive the action (Lambda) as a template parameter.

*   `Ignore`: Performs no operation. Code must be eliminated by the compiler.
*   `Direct<auto Action>`: Executes the lambda immediately (inline).
*   `Defer<auto Action>`: Offloads the lambda to a scheduler (e.g., converts to function pointer).

### 3.2. Hardware Concept
The `HardwareConfig` concept validates that the configuration struct provides `execute()` static methods for all required events (e.g., `Event1`, `Event2`, `Event3`).

### 3.3. Usage Pattern
The user defines a struct (e.g., `MyConfig`) containing strictly `using` declarations for each event. Logic is written inline using stateless lambdas.

## 4. Reference Implementation

```cpp
#include <concepts>
#include <iostream>

// --- Environment Mock ---
using TaskFunc = void(*)();
void sched_defer(TaskFunc func); // Implemented externally

// --- 1. Policy Definitions ---

// Policy: Do nothing (Optimizer removes this completely)
struct Ignore {
    static __attribute__((always_inline)) void execute() {}
};

// Policy: Execute immediately inline
// Uses C++20 NTTP to accept a lambda as a compile-time constant
template<auto Action>
struct Direct {
    static __attribute__((always_inline)) void execute() {
        Action();
    }
};

// Policy: Defer execution to scheduler
template<auto Action>
struct Defer {
    static __attribute__((always_inline)) void execute() {
        // Stateless lambda converts to function pointer via unary +
        sched_defer(+Action);
    }
};

// --- 2. Concept Definition ---

// Ensures the Config struct has all required Event definitions
template<typename T>
concept HardwareConfig = requires {
    { T::Event1::execute() } -> std::same_as<void>;
    { T::Event2::execute() } -> std::same_as<void>;
    { T::Event3::execute() } -> std::same_as<void>;
};

// --- 3. Hardware Driver Implementation ---

template<HardwareConfig Config>
struct Hardware1 {
    static __attribute__((always_inline)) void irq_handler() {
        // Example: Reading a status register
        volatile uint32_t status = 0b111; // Mock

        // Compile-time expansion:
        // If EventX is Ignore, the branch/call is optimized away.
        // If EventX is Direct/Defer, the lambda body is inlined.
        if (status & 0b001) Config::Event1::execute();
        if (status & 0b010) Config::Event2::execute();
        if (status & 0b100) Config::Event3::execute();
    }
};

// --- 4. User Configuration Example ---

struct MyConfig {
    // Event1: High Priority (Inline Logic)
    using Event1 = Direct<[]{
        // Write logic here directly
        std::cout << "Event1: Immediate\n";
    }>;

    // Event2: Not used
    using Event2 = Ignore;

    // Event3: Low Priority (Deferred)
    using Event3 = Defer<[]{
        std::cout << "Event3: Deferred\n";
    }>;
};

// VALIDATION: Ensure all events are defined immediately
static_assert(HardwareConfig<MyConfig>, "MyConfig is missing required Event definitions.");

// --- 5. Instantiation ---
using MyHardware = Hardware1<MyConfig>;

void ISR_Handler() {
    MyHardware::irq_handler();
}
```

## 5. Coding Guidelines for LLM
*   **Do not** use `std::function` or runtime function pointers for policies.
*   **Do not** use virtual functions or inheritance for polymorphism.
*   **Always** use `static __attribute__((always_inline))` (or `[[gnu::always_inline]]`) for policy execution methods to prevent function call overhead.
*   **Ensure** that `static_assert` checks against the Concept are placed immediately after the `Config` struct definition to provide immediate feedback on missing events.
*   **Lambdas** passed to `Direct` or `Defer` must be stateless (empty capture `[]`).