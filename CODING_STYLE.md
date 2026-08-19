# C Coding Style Guide

These rules define the formatting style for C code. They are intended to ensure consistency and improve readability for coding agents and human developers.

## 1. File Header & License

* Every file must start with a file header containing the full repository file path, the current copyright year, and the license statement wrapped in a multi-line comment block exactly as shown:
  ```c
  /*
   * drivers/block_cache.c
   * Copyright (C) <YEAR> <AUTHOR>
   * Project is distributed under the terms of the MIT License
   */
  ```
* The `<AUTHOR>` placeholder must contain the full name of the creator. If the author is unknown, use "Author".
* A single empty line must be placed immediately after this file header block, before the opening include guard directive.

## 2. Indentation Rules

* **Base Indentation**: Use **2 spaces** per indentation level. **Tabs are prohibited.**
* **Line Width Limit:** Strictly 80 characters maximum.
* **Switch Statement Formatting**:
  * `switch` keyword starts at current indentation level.
  * `case` labels are indented **1 level** deeper.
  * Code inside `case` follows label indentation.
  * Opening `{` on new line, closing `}` on own line aligned with `switch` statement.
* **Example**:
  ```c
  switch (value)
  {
    case 0:
      do_something();
      break;
    default:
      handle_error();
      break;
  }
  ```

## 3. Spacing Conventions

* **Control Flow Keywords**: Single space between keyword and opening parenthesis
  * Correct: `if (condition)`
  * Incorrect: `if(condition)`
* **Parentheses Content**: No spaces inside parentheses
  * Correct: `func(arg1, arg2)`
  * Incorrect: `func( arg1, arg2 )`
* **Pointer Asterisk (`*`)**:
  * For `const` pointers: space around `*`
    ```c
    const int * const ptr;
    ```
  * For non-const pointers: attach `*` to variable name
    ```c
    int *ptr;
    ```
  * Dereferencing: no space between `*` and variable
    ```c
    value = *ptr;
    ```

## 4. Naming Conventions

* **Void Pointers**: Use `object` for untyped self-referential pointers.
* **Typed Pointers**: Use descriptive names based on driver function.
  ```c
  struct TimerDriver *timer;
  struct MemoryMap *memory;
  ```
* **Macros**: `UPPER_CASE` with underscores (e.g., `MAX_BUFFER_SIZE`).
* **Structs/Unions/Enums**: `PascalCase` (e.g., `BlockCache`).
* **Functions**: `camelCase`. Must begin with a verb (e.g., `readBlock`, `initializeDriver`).
* **Variables**: `camelCase`. Must begin with a noun or adjective (e.g., `loopCounter`, `isDirty`).
* **Acronyms**: Treat all acronyms as standard words in `camelCase` and `PascalCase` patterns.
  * **Correct**: `getUsbStatus`, `DmaController`
  * **Incorrect**: `getUSBStatus`, `DMAController`
* **File Names**: Lowercase with underscores (`snake_case`) matching the module name (e.g., `block_cache.c`).
* **Explicit Type Casts for Void Pointers**: When converting from a generic parameter interface (`void *object`) to a concrete driver type, use an explicit type cast assignment on its own line at the very top of the function block.
  ```c
  /* Correct: Explicitly cast void * to the concrete structure type */
  struct BlockCache * const cache = (struct BlockCache *)object;
  ```

## 5. Bracing Style

* **General Blocks**: Opening `{` on new line, closing `}` on own line aligned with statement.
* **Multi-Line Condition Wrapping**: When wrapping long conditional expressions (`if`, `while`), place the logical operator (`&&`, `||`) at the start of the next line, indented by 4 spaces. The opening brace `{` must still occupy its own line.
  ```c
  if (cache->isValid
      && cache->cachedBlock == block)
  {
    return 0;
  }
  ```
* **Example**:
  ```c
  if (condition)
  {
    do_work();
  }
  else
  {
    handle_error();
  }

  struct MyStruct
  {
    int field;
  };

  int foo(void)
  {
    return 42;
  }
  ```

## 6. Bracing Style Amendment: Single-Line Omission

Braces (`{}`) can be omitted for simple control flow statements (`if`, `else`, `while`, `for`) if and only if the body consists of a single statement.

* **Rule**: The single statement must be placed on a new line and indented by 2 spaces.
* **Correct**:
  ```c
  if (res != 0)
    return res;
  ```
* **Incorrect**:
  ```c
  if (res != 0) return res; // Prohibited: statement must be on a new line
  ```

While braces (`{}`) can be omitted for single-statement bodies, they are **strictly mandatory** if the control flow condition (`if`, `else`, `while`, `for`) is wrapped across multiple lines.

* **Rule**: If a condition breaks across lines due to line-wrapping rules, the code block that follows must use opening and closing braces, even if it contains only a single statement.
* **Correct**:
  ```c
  if (cache->isValid
      && cache->cachedBlock == block)
  {
    return 0;
  }
  ```
* **Incorrect**:
  ```c
  if (cache->isValid
      && cache->cachedBlock == block)
    return 0; // Prohibited: condition is multi-line, braces must be present
  ```

## 7. Initialization Formatting

* **Array/Struct Initialization**: Pack adjacent braces (`}, {`) on single line.
* **Indentation**: 4 spaces per nesting level.
* **Inline Formatting**: No spaces after `{` or before `}` for single-line initializers.
* **Example**:
  ```c
  static const struct Config configs[] = {
      {.id = 1, .val = 10},
      {.id = 2, .val = 20}
  };

  int matrix[2][2] = {
      {1, 2},
      {3, 4}
  };
  ```

## 8. Line Wrapping Rules

* **Break Points**: Wrap lines before operators. Place all operators at the beginning of the next line, with the sole exception of the ternary operator (?), which remains at the end of the line.
* **Indentation Levels**:
    * Level 1: +4 spaces
    * Level 2: +8 spaces
    * Subsequent levels: +4 spaces per nesting level
* **Example**:
  ```c
  long_variable_name = very_long_function_call(
      arg1,
      arg2,
      arg3
  );

  complex_expression = part_one
      + part_two
          + deeply_nested_part;

  // Ternary operator exception
  result = condition ?
      if_true
          : if_false;
  ```

## 9. Code Separation

* **Logical Blocks**: Separate with single empty line.
* **No Visual Dividers**: Only empty lines allowed.

## 10. Integer Sizing and Arithmetic

* **Type Safety for Driver Math**: Never use plain `int` for buffer indices, block counts, or offset sizes. Always use explicit fixed-width or standard types (`size_t`, `uint32_t`, `uintptr_t`). Never mix signed and unsigned integers in loops or comparison conditions.
  ```c
  /* Correct */
  for (size_t i = 0; i < length; ++i)
  {
    cache->buffer[i] = data[i];
  }
  ```

## 11. Commenting Guidelines

* **Comment Style**: Only C-style comments (`/* ... */`)
* **Single-Line Comments**: Start with `/*`, capitalize first word, no trailing period
*  **Multi-Line Comments**: Use standard block comment formatting. The first word of the first line is capitalized. A period **must** be placed at the end of the final sentence.
  ```c
  /*
   * This function handles the serial protocol.
   * It manages buffering and flow control.
   * Ensure the baud rate matches the peripheral.
   */
  ```

## 12. Unused Parameters

* **Completely Unused**: If a parameter is never used in any build configuration, omit the name entirely (C23 standard).
  ```c
  void callback(void *ignored) // Old style
  void callback(void *)        // New style (C23)
  ```
* **Debug-Only Usage**: If a variable is used only in debug builds and unused in release builds, mark it with the `[[maybe_unused]]` attribute.
  ```c
  void debug_func([[maybe_unused]] int debug_val)
  {
  #ifdef DEBUG
    log_value(debug_val);
  #endif
  }
  ```

## 13. Const Correctness

* **Pointers**: Apply `const` to pointers that do not modify the data they point to within the function scope.
* **Other Types**: Apply `const` to any variable (e.g., `int`, `char`, struct instances) that is not modified after initialization.
  ```c
  void process(const int *data, const struct Config *config)
  {
    const int limit = 100;
    // ...
  }
  ```

## 14. Immutable Local Pointers

Any local pointer variable whose address target is not modified after its initialization must be explicitly declared as a constant pointer (`* const`).

* **Rule**: If the pointer always points to the same memory location throughout its lifetime, place `const` after the `*` symbol. If the data it points to is also immutable, both `const` positions must be applied.
* **Correct**:
  ```c
  /* Pointer target is immutable */
  struct BlockCache * const cache = object;
  /* Target address and underlying data are both immutable */
  const struct BlockCacheConfig * const cfg = config;
  ```
* **Incorrect**:
  ```c
  struct BlockCache *cache = object;
  const struct BlockCacheConfig *cfg = config;
  ```

## 15. Containers and CRC

* **Containers**: When utilizing basic data structures, prefer types defined in the `xcore` library (e.g., specific list, queue, or array implementations provided by `xcore`). Do not implement custom structures if an `xcore` equivalent exists.
* **CRC Calculations**: Use the CRC utility functions provided by the `xcore` library rather than implementing custom CRC algorithms.

## 16. OOP Approach in C

When implementing object-oriented style in C using the `xcore` framework:

* **Encapsulation**: Represent objects as `struct` containing state variables.
* **Methods**: Implement methods as standalone functions that accept a pointer to the object struct as the first argument (simulating `this`/`self`).
* **Virtual Tables**: Use function pointers within the struct (vtable) to allow for polymorphism where required by the driver architecture.
* **Initialization**: Always provide an explicit `init` function to construct the object, ensuring all internal pointers are nulled or initialized safely.
* **Defensive Null Pointer Verification**: Public-facing API methods and framework-exposed virtual functions must check all incoming object pointers against `NULL` before dereferencing. Internal helper functions (`static`) may omit this check if validated upstream.
  ```c
  if (object == NULL)
    return -1;
  ```
* **Explicit Memory Clearing**: Every `init` constructor function must explicitly initialize the underlying fields.
  ```c
  /* Correct: uninitialized field cleared in the constructor */
  cache->device = NULL;
  ```
* **Example**:
  ```c
  /* Example OOP pattern in C using xcore conventions */
  extern const struct InterfaceClass * const SerialDriver;

  struct SerialDriver
  {
    struct Interface base;

    void (*callback)(void *);
    void *callbackArgument;

    int id;
  };

  const struct InterfaceClass * const SerialDriver =
      &(const struct InterfaceClass){
      .size = sizeof(struct BufferingProxy),
      .init = interfaceInit,
      .deinit = interfaceDeinit,

      .setCallback = interfaceSetCallback,
      .getParam = interfaceGetParam,
      .setParam = interfaceSetParam,
      .read = interfaceRead,
      .write = interfaceWrite
  };
  ```
