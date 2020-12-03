# Coding Style

## File Layout

1. Headers
2. Macros
3. Types
4. Global Variables
5. Functions
6. main

## Leading whitespace

* **DO** use 4 spaces as tab.

## Blocks

* `{` on own line.
* `}` also on own line except on `do while` statements.

*Example:*

```C++
if (cond)
{
    // ...
}
else
{
    // ...
}

while (cond)
{
    // ...
}

do
{
    // ...
} while (cond);

class FooBar
{
    FooBar();
};

namespace bazz
{

} // namespace bazz

extern "C"
{
    // ...
}

void do_foo()
{
    // ...
}
```

## Comments

* Avoid comment that paraphrases the code. Comments should answer the why and the code the how.

## Functions

* Return type and modifiers on the same line as the function name.
* The arguments is adjacent to the function name.
* Use `snake_case` for their name.

*Example:*

```C++
static void foo()
{
    bar;
}

foo();
```

## Variables

* Also use `snake_case` for their name.
* In declaration of pointer the * is adjacent to their name, not the type.

## Keywords

* Use a space after `if`, `for`, `while`, `switch`.
* Do not use a space after the opening ( and before the closing ).
* Preferably use () with `sizeof`.
* Use `nullptr` when necessary otherwise use `NULL`.
* Always use curly braces after `if`, `for`, `while`, `switch` statements.

**DON'T**
```C++
if (foo)
    bar;
```

**DO**
```C++
if (foo)
{
    bar;
}
```

## Switch

* Don't indent cases another level.
* Switch case should NOT falltrough.
* But if you have no other choise (very unlikely) add a `/* FALLTROUGH */` comment.

*Example:*

```C++
switch(foo)
{
case bar:
    printf("foo");
    /* FALLTROUGH */

case mitzvah:
    printf("barmitzvah\n");
    break;
}
```

## Headers

* Include standard libraries first, then include project header, then include local headers (separate them with a new line).
* Use `.h` as extension.
* If you want to declare a new header use cross-defines, guard must be `FOLDER_FILE_H`

*Example:*

```C++
#ifndef CPINTI_DEBUG_H
#define CPINTI_DEBUG_H

#include <cstdlib>

#endif
```

## User Defined Types

* Don't use `type_t` naming. (If you see one in the code base (on non-posix standard libraries), please report it :sweat_smile:)
* Don't typedef builtin types.
* Use `CamelCase` for typedef'd types.

## Line length

* Keep lines between 60 and 80 characters long.

## Main function

* Always put `argc` and `argv` as arguments (**NEVER** put nothing or void as arguments).
* If you don't need `argc` and/or `argv` use the macro `__unused`.
* Use double pointer for `argv`.

*Example:*

```C++
int main(int argc, char **argv)
{
    __unused(argc);
    __unused(argv);
}
```

## Handling Errors

* `return -1` when there is an error
* `return 0` when there is not

## Formatting

* You can use the command `clang-format -r` for formatting your code automaticly

# Authors

- [@Keyboard-Slayer](https://github.com/Keyboard-Slayer)
- [@sleepy-monax](https://github.com/sleepy-monax)