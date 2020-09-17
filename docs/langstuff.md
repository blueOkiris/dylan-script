# Language Definition

## Basic data types:

 - 8 Types: Num (num), Boolean (bool), List (list), String (str), Tuple (tup), Instruction (inst), Function (func), Container, None (none)

 - Num

   * 'double' in other languages

   * Auto-*rounded* to integer for modulus and array accessing

   * Also floor/ceil/round functions builtin

   * Can be defined by /0x[A-F0-9]+/ or /0b[01]+/ or /0c./

   * Used with builtin math ops

 - Boolean
   
   * true or false

   * Used with builtin bool ops

 - List

   * Dynamic array

   * Concatenate, remove, slice, and access

   * Can hold any number of *same type*

 - String

   * List\<Num\> *but*

   * Can only append integer types (chars)

   * Prints as string instead of `[ , , , ... ]`

   * Defined with `''`

 - Tuple

   * List with only two elements

   * Can hold any two elements (even diff types)

 - Instruction
   
   * Compiled bytecode instructions

   * Can be created from string and executed. __That's it__

 - Function

   * Typed arguments, return type, and list of instructions

   * Can be called as a function

 - Container

   * C-struct, *but*

   * default values and of course methods since functions can be stored

   * Can be member accessed

- None
  
  * null

## Modules

 - Like python, each file is a module

 - 'import' + \<module name\>

 - Must import everything (at least for now since easy)

## Instances

 - Statically typed, but weakly typed

 - In other words, var a "= 'hello'

   * Sets a to be a string

   * Sets a as a mutable type

   * a cannot be changed to any other type, though its value can be copied and *cast* as const

   * Like var/auto in C#/C++

 - Hold type, value, and mutability

 - Can be *variables*, var, or *constants*, const. I like this better than default variable that *is* constant and a mutable variable. That's just nonsense, Rust. Come on.

 - Default pass by value. Can be passed by reference with ref keyword like C#

## ':' <type> Constructions

In containers, members are defined with a type name (strong typing). This also goes for arguments and return types in functions (though return types are denoted with an arrow)

You can also give default values.

That said, in containers, mutability is *not* given and matches the mutability of the container (as the container itself is a singular mass of data). It *is* given in functions

Finally, in defining instances of container types, you provide  `<member> := <start-val>`. If a member has a default value, then you don't need to provide it.

Example:

```
cont myContainer {
    a : num := 0
    b : num
    c : str
    d : list<num> := [ 0 1 2 ]
}

func myFunction(const a : num := 0, var b : num, ref var c : str) -> str {
    const myContInst := myContainer { a := 1 b := 2 c := 3 }
    return 'hi'
}
```

## Other Things

 - Most other stuff will come as consequence, or can be assumed to be C-like or python-like

 - For instance, comments will be C-like

 - This document is not comprehensive, but instead a place for me to work out ideas, so take it with a grain of salt.

 - Also, commas and semi-colons and whitespace and stuff aren't needed because (hopefully) my lexer and parser will be good enough
