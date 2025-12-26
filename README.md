# ItAdd!
ItAdd is an unstructured scripting "language" inspired by assembly and designed for math-like computation.

# How it Works

## Basic syntax
First, you can make a simple expression using one-or-two integers and an operand separated by spaces. For example, `1 + 1` is a valid ItAdd expression. You can also store numbers in one of 26 available variables (one for each letter of the alphabet) using assignment like so: `a = 12`. Now, you can use `a` instead of an integer anywhere in an expression and it will be replaced with `12` as it was assigned.

The currently supported functions are:
 * x + y (addition)
 * x - y (subtraction)
 * x * y (multiplication)
 * x / y (division)
 * x ^ y (exponention)
 * x ! (factorial)
 * x ? (termial / triangular number)
 * x % y (modulo)

Comments don't have any fancy syntax and any thing after the third token *(typically the second operand, or the function if it only takes one input)* is ignored, but it's good practice to make up your own little comment indicator to keep readability.

## ans
Now, what in god's name are we supposed to do with these expressions? Simple, there's a variable called `ans` that does the magic. `ans` cannot be directly assigned, only referenced. `ans` stores the **value of the last executed expression**.

For example, you can chain multiple expressions with `ans` to compute larger expressions that the simple *x operand y* formula can't. For example, here's `(3 + (2*5)) * ((2*2) * (2*2))`:

    2 * 2
    a = ans
    a * a
    a = ans

    2 * 5
    3 + ans
    ans * a

It's also worth mentioning that nothing will be automatically outputted to the terminal until after the last line is executed, in which the value of `ans` will be outputted before execution terminates.

## GOTO
You might be wondering, *"is there any control flow?"*, and the answer is "sorta". ItAdd's specification supports `goto` and labels. Labels work just like they do in most languages: it's a special tag ending with a colon that you put on the line before the code you want control to jump to upon calling. Also, execution will not automatically stop once it reaches another goto label, it will fall through and start executing code in the next label until a goto is executed.

A very basic `goto` statement could just be `GOTO myLabel`, but that's boring. It's basically useless if there's no conditional jumping. That's where the `IF` keyword comes into play. After `myLabel`, you can add `IF` and a very basic expression comparing a number to `ans`; this is called a `GOTO IF` statement. There's also `IF NOT` which jumps to your goto if the comparison is false. A `GOTO IF` statement could look something like this: `GOTO myLabel IF > 6`. This statement will jump to `myLabel` if `ans` is greater than 6. You can also use variables here. The supported comparison operators are:
 * `== x` (ans equal to x)
 * `!= x` (ans not equal to x)
 * `> x` (ans greater than x)
 * `< x` (ans less than x)
 * `>= x` (ans greater than or equal to x)
 * `<= x` (ans less than or equal to x)

For example, we will perform the expression we did earlier, and have a counter that we replace the first 2 with and loop as long as the result is less than a thousand:

    GOTO start

    loop:
    i + 1
    i = ans

    start:
    i * 2
    a = ans
    a * a
    a = ans

    2 * 5
    3 + ans
    ans * a

    GOTO loop IF < 1000

## I/O
ItAdd supports terminal I/O with the `@` and `~` keywords.

### Input
You can assign any variable to `@` to get input from the terminal. On the terminal side, the user sees: `[variable being assigned] - `. For example, if you wanted to calculate `([user input] * 2) % 3`, you could do:

    a = @
    a * 2
    ans % 3

If a user inputs something entirely invalid, `@` returns 0. If a user inputs a float, the decimal is truncated. All whitespace and invalid characters in the input is ignored.

### Output
You can output a variable on demand by trying to use the unary `~` operator on it. On the terminal, the user sees `[variable] = [value]\n`. For example, if you wanted to make a power of 2 counter:

    1 + 0 -- setting ans to 1

    loop:
    ans * 2
    ans ~
    GOTO loop IF > 0 -- check for overflow

# Notes
 * `ans` and all 26 variables are initialized to zero
 * Goto labels and blank lines are treated like `nop` instructions
 * Tokens are strictly whitespace-delimited
 * During goto label fall-through, goto labels are treated like a blank line
 * Goto labels cannot contain whitespace
 * `GOTO` statements without a condition are executed automatically and without question
 * `ans` and all variables are signed 64-bit integers
