# Structured Text Tutorial
> Tutorial from http://www.plcacademy.com/structured-text-tutorial/

## Elementary data types
+ Integers
  IEC Data Type | Format | Range 
  - | - | -
  SINT | Short Integer |-128 ~ 127(-2^7 ~ 2^7-1)
  INT  |  Integer |-32768 ~ 32767(-2^15 ~ 2^15-1)
  DINT | Double Integer |-2^31 ~ 2^31-1
  LINT | Long Integer |-2^63 ~ 2^63-1
  USINT | Unsigned Short Integer |0 ~ 255(0 ~ 2^8-1)
  UINT | Unsigned Integer |0 ~ 2^16-1
  LDINT | Long Double Integer |0 ~ 2^32-1
  ULINT | Unsigned Long Integer |0 ~ 2^64-1
+ Floating points
  IEC Data Type | Format | Range
  - | - | -
  REAL | Real Numbers | ±10^±38
  LREAL | Long Real Numbers | 	±10^±308
+ Time
  IEC Data Type | Format | Range
  - | - | -
  TIME	| Duration of time after an event	| T#10d4h38m57s12ms or TIME#10d4h38m
  DATE	| Calendar date |	D#1989-05-22 or DATE#1989-05-22
  TIME_OF_DAY	| Time of day |	TOD#14:32:07 or TIME_OF_DAY#14:32:07.77
  DATE_AND_TIME	| Date and time of day |DT#1989-06-15-13:56:14.77 or DATE_AND_TIME#1989-06-15-13:56:14.77
+ Strings
  IEC Data Type | Format | Range
  - | - | -
  STRING | Character String | ‘My string’
+ Bit strings
  IEC Data Type | Format | Range
  - | - | -
  BOOL |	Boolean	| 1 bit
  BYTE | Byte |	8 bits
  WORD | Word | 16 bits
  DWORD |	Double Word |	32 bits
  LWORD	| Long Word	| 64 bits

##  Derived data types: 
The derived data types are your own custom data types. All the derived data types are built by making a construction of the keywords TYPE and END_TYPE. In between the keywords is the kind of derived data type you want to declare.
+ Structured data types
+ Enumerated data types
+ Sub-ranges data types
+ Array data types

## Operators
Operation | Symbol | Precedence
- | - | -
Parenthesization(括号) | (expression) |	Highest
Function Evaluation | MAX(A,B) 
Negation Complement | NOT
Exponentiation(求幂) | **
Multiply | *
Divide | /
Modulo | MOD
ADD | +
Substract | -
Comparison | <,>,<=,>=
Equality | =
Inequality | <>
Boolean AND | & or AND
Boolean Exclusive OR(异或) | XOR
Boolean OR | OR |Lowest

Types of Operators
1. Arithmetic Operators
   + +(add)
   + – (subtract/negate)
   + *(multiply)
   + ** (exponent)
   + / (divide)
   + MOD (modulo divide)
2. Relational Operators
   + = (equal)
   + < (less than)
   + <= (less than or equal)
   + ">" (greater than)
   + ">=" (greater than or equal)
   + <> (not equal)
3. Logical Operators
   + & or AND
   + OR
   + XOR
   + NOT
4. Bitwise Operators
   + & or AND
   + OR
   + XOR
   + NOT

## Assignment Statement
`A := B;`

## Conditional Statements
### IF Statements
```
IF [boolean expression] THEN
  <statement>;
ELSIF [boolean expression] THEN
  <statement>;
ELSE
  <statement>;
END_IF ;
```

### CASE Statements
```
CASE [numeric expression] OF
  result1: <statement>;
  resultN: <statemtent>;
ELSE
  <statement>;
END_CASE;
```
e.g
```
PROGRAM_STEP := 3;
CASE PROGRAM_STEP OF
  1: PROGRAM_STEP := PROGRAM_STEP+1;
  2: PROGRAM_STEP := PROGRAM_STEP+2;
  3: PROGRAM_STEP := PROGRAM_STEP+3;
ELSE
  PROGRAM_STEP := PROGRAM_STEP+10;
END_CASE;
```

## Iteration with Repeating Loops
### FOR
```
FOR count := initial_value TO final_value BY increment DO
  <statement>;
END_FOR;
```

### WHILE
```
WHILE [boolean expression] DO
  <statement>;
END_WHILE;
```
e.g
```
counter := 0;
WHILE counter < 10 DO
  counter := counter + 1;
  machine_status := counter * 10;
END_WHILE;
```

### REPEAT Loops
```
REPEAT
  <statement>;
UNTIL [boolean expression]
END_REPEAT;
```