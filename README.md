## Special Section Solutions for Deitel's "C How to Program" Book
Solutions to other questions will be uploaded.

### Changelog
> 16 Jun 2024

**Simpletron Simulator V2**
* Memory of Simpletron increased. 7.30.a
* Added remainder (MOD) and exponentiation (POW) instructions. 7.30.b/c
* Program values completely modified from decimal to hexadecimal. 7.30.d
* Range of overflow control algorithms changed to 4 digit(hex).
* Type of operand variable changed from int to size_t because it is used as array index.
* Minor optimizations.

**Simpletron Simulator V1**
* Range of overflow control algorithms changed to 4 digit(dec).
* Type of operand variable changed from int to size_t because it is used as array index.
* Minor optimizations.
---

> 14 Jun 2024

**Simpletron Simulator V1**
* Bugs fixed - Overflow detection of ADD and MULTIPLY operations.
* Bug fixed - "End of Computer Dump" message not printing when function called w/o error.
* Optimized - "if conditions" of overflow detection algorithms rewrited for short circuit evaluation.
---

> 13 Jun 2024 - 2nd Update

**Simpletron Simulator V1**
* Added "accumulator" overflow/underflow controls for arithmetic operations.
* Added "divide by zero" error detection and warning for DIVIDE operation.
* Added range control for invalid operation codes.
* Dump function parameters simplified for readability, less code.
* Types of register variables changed from "short int" to "int" (Preparation for Simpletron V2).
* Updated comments and print messages.

**README.md**
* Created this changelog.
---

> 13 Jun 2024

**Simpletron Simulator V1**
* Added dump function.
* Updated all funcions for modularity (All register variables defined in main and functions calling by referance).
* Updated comments.
* Renamed file from "Simpletron Simulator V1.c" to "Simpletron Simulator V1(Exercise 7.29).c".
---

> 12 Jun 2024


* Uploaded "Simpletron V1 Instructions for Exercise 7.28a/b/c.txt"
---

> 11 Jun 2024


* Project created.
* Uploaded "Simpletron Simulator V1.c"
