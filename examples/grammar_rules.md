# P Cubed Language Grammar

## Comments

| **Syntax**         | **Explanation** |
|--------------------|-----------------|
| `# Comment`        | Single-line comment |
| `# Line 1`<br>`# Line 2`<br>`# Line 3` | Multiline comment |

## Variables & Assignments

| **Syntax**                  | **Explanation**             |
|-----------------------------|-----------------------------|
| `Identifier <- Expression`  | Assigns the value of an expression to a variable. |
| `CONSTANT IDENTIFIER <- Expression` | Assigns a constant value to a variable. |

### Examples

```p3
a <- 3
b <- a + 1
c <- "Hello"
```

```p3
CONSTANT PI <- 3.141
CONSTANT CLASS_SIZE <- 20
```

## Arithmetic Operators

| **Operator** | **Description**       | **Example**             |
|--------------|-----------------------|-------------------------|
| `+`          | Addition               | `a + b`                 |
| `-`          | Subtraction            | `a - b`                 |
| `*`          | Multiplication         | `a * b`                 |
| `/`          | Division               | `a / b`                 |
| `^`          | Exponentiation         | `a ^ b`                 |
| `DIV`        | Integer Division       | `9 DIV 5`<br>`5 DIV 2`  |
| `MOD`        | Modulo (Remainder)     | `9 MOD 5`<br>`5 MOD 2`  |

*Integer division divides one number by another and returns the quotient as an integer, rounded down if necessary. The modulo operator returns the remainder.*

## Relational & Boolean Operators

### Relational Operators

| **Operator** | **Description**       | **Example**             |
|--------------|-----------------------|-------------------------|
| `<`          | Less than              | `a < b`                 |
| `>`          | Greater than           | `a > b`                 |
| `=`          | Equal to               | `a = b`                 |
| `≠`          | Not equal to           | `a ≠ b`                 |
| `≤`          | Less than or equal to  | `a ≤ b`                 |
| `≥`          | Greater than or equal to | `a ≥ b`               |

### Boolean Operators

| **Operator** | **Description**       | **Example**                   |
|--------------|-----------------------|--------------------------------|
| `AND`        | Logical AND           | `(a = b) AND (c ≤ d)`          |
| `OR`         | Logical OR            | `(x < 1) OR (x > 9)`           |
| `NOT`        | Logical NOT           | `NOT (a < b)`                  |

## Iteration

### Indefinite (Condition Controlled) Iteration

| **Loop Type**       | **Syntax**                                                        | **Explanation**                  |
|---------------------|-------------------------------------------------------------------|----------------------------------|
| `REPEAT-UNTIL` Loop | `REPEAT`<br>`{statements}`<br>`UNTIL condition`                | Executes statements until condition is true. |
| `WHILE-ENDWHILE` Loop | `WHILE condition`<br>`{statements}`<br>`ENDWHILE`            | Executes statements while condition is true. |

### Examples

```p3
a <- 1
REPEAT
   OUTPUT a
   a <- a + 1
UNTIL a = 4
```

```p3
a <- 1
WHILE a < 4
   OUTPUT a
   a <- a + 1
ENDWHILE
```

### Definite (Count Controlled) Iteration

| **Loop Type**       | **Syntax**                                                        | **Explanation**                  |
|---------------------|-------------------------------------------------------------------|----------------------------------|
| `FOR-TO-[STEP]-ENDFOR` Loop | `FOR variable <- start TO end [STEP step]`<br>`{statements}`<br>`ENDFOR` | Loops from start to end, optionally with a step. |
| `FOR-IN-ENDFOR` Loop | `FOR variable IN collection`<br>`{statements}`<br>`ENDFOR`    | Iterates over elements in a collection. |

### Examples

```p3
FOR a <- 1 TO 3
   OUTPUT a
ENDFOR
```

```p3
length <- 0
FOR char IN message
   length <- length + 1
ENDFOR
```

## Selection

| **Selection Type** | **Syntax**                                                         | **Explanation**                  |
|--------------------|--------------------------------------------------------------------|----------------------------------|
| `IF-THEN-ELSE`     | `IF condition THEN`<br>`{statements}`<br>`ELSE`<br>`{statements}`<br>`ENDIF` | Executes statements based on a condition. |
| `IF-THEN-ELSEIF-ELSE` | `IF condition1 THEN`<br>`{statements}`<br>`ELSEIF condition2 THEN`<br>`{statements}`<br>`ELSE`<br>`{statements}`<br>`ENDIF` | Executes different statements based on multiple conditions. |
| `CASE OF OTHERWISE` | `CASE OF variable`<br>`value1 : {statements}`<br>`value2 : {statements}`<br>`OTHERWISE : {statements}`<br>`ENDCASE` | Selects among multiple options. |

### Examples

```p3
IF a = 1 THEN
   OUTPUT "a is 1"
ELSE
   OUTPUT "a is not 1"
ENDIF
```

```p3
CASE OF grade
   'A' : OUTPUT "Excellent"
   'B' : OUTPUT "Good"
   'C' : OUTPUT "Fair"
   OTHERWISE : OUTPUT "Needs Improvement"
ENDCASE
```

## Arrays

| **Syntax**                     | **Explanation**                                             |
|--------------------------------|-------------------------------------------------------------|
| `identifier <- [element1, element2, ..., elementN]` | Initializes an array with elements.       |
| `array[index]`                 | Accesses the element at the specified index (1-based).      |
| `array[index] <- value`        | Assigns a value to the element at the specified index.      |

### Examples

```p3
marks <- [75, 85, 90, 100]
marks[2] <- 88
```

*This initializes an array `marks` with four elements and updates the second element.*

## Records

| **Syntax**                       | **Explanation**                                             |
|----------------------------------|-------------------------------------------------------------|
| `DECLARE RECORD`                 | Defines a record structure.                                  |
| `identifier.fieldname`           | Accesses a field in a record.                                |

### Examples

```p3
DECLARE RECORD Student
   Name : STRING
   Age : INTEGER
   Mark : INTEGER
ENDRECORD

student1 <- Student("Alice", 20, 85)
student1.Mark <- 90
```

*This example declares a `Student` record with fields `NAME`, `AGE`, and `MARKS`, and then initializes and updates the record.*

## Subroutines

| **Subroutine Type** | **Syntax**                                                         | **Explanation**                  |
|---------------------|--------------------------------------------------------------------|----------------------------------|
| `PROCEDURE`         | `PROCEDURE name (parameters)`<br>`{statements}`<br>`ENDPROCEDURE` | Defines a procedure (no return value). |
| `FUNCTION`          | `FUNCTION name (parameters) RETURNS type`<br>`{statements}`<br>`RETURN value`<br>`ENDFUNCTION` | Defines a function that returns a value. |

### Examples

```p3
PROCEDURE Greet(name)
   OUTPUT "Hello, " + name
ENDPROCEDURE

FUNCTION Add(a, b) RETURNS INTEGER
   RETURN a + b
ENDFUNCTION
```

## String Handling

| **Function**       | **Syntax**                               | **Explanation**                                      |
|--------------------|------------------------------------------|------------------------------------------------------|
| `LENGTH`           | `LENGTH(string)`                         | Returns the length of a string.                      |
| `SUBSTRING`        | `SUBSTRING(string, start, length)`       | Returns a substring from the string.                 |
| `CONCAT`           | `CONCAT(string1, string2)`               | Concatenates two strings.                            |
| `POSITION`         | `POSITION(substring, string)`            | Returns the position of a substring within a string. |

### Examples

```p3
name <- "Alice"
length <- LENGTH(name)          # Returns 5
greeting <- CONCAT("Hi, ", name) # Returns "Hi, Alice"
firstLetter <- SUBSTRING(name, 1, 1) # Returns "A"
position <- POSITION("ice", name) # Returns 3
```

## String and Character Conversion

| **Function**       | **Syntax**                               | **Explanation**                                      |
|--------------------|------------------------------------------|------------------------------------------------------|
| `CHAR_TO_CODE`     | `CHAR_TO_CODE(character)`                | Converts a character to its ASCII code.              |
| `CODE_TO_CHAR`     | `CODE_TO_CHAR(code)`                     | Converts an ASCII code to its character.

 |

### Examples

```p3
code <- CHAR_TO_CODE('A')  # Returns 65
char <- CODE_TO_CHAR(65)   # Returns 'A'
```

## Input & Output

| **Command**       | **Syntax**                               | **Explanation**                                      |
|-------------------|------------------------------------------|------------------------------------------------------|
| `INPUT`           | `INPUT variable`                         | Takes input and assigns it to a variable.            |
| `OUTPUT`          | `OUTPUT expression`                      | Displays the value of an expression.                 |

### Examples

```p3
INPUT name
OUTPUT "Hello, " + name
```

## Random Number Generation

| **Function**       | **Syntax**                               | **Explanation**                                      |
|--------------------|------------------------------------------|------------------------------------------------------|
| `RANDOM`           | `RANDOM(min, max)`                       | Returns a random integer between `min` and `max`.    |

### Examples

```p3
diceRoll <- RANDOM(1, 6)
```
