#ifndef AST_H
#define AST_H

#include <stdlib.h>

// Enum representing the different types of AST nodes.
enum ast_type
{
    AST_NUM,            // For number literals
    AST_STRING,         // For string literals
    AST_BOOL,           // For boolean literals
    AST_CHAR,           // For character literals
    AST_REAL,           // For real literals
    AST_ASSIGNMENT,     // For variable assignments
    AST_INSTANTIATION,  // For object instantiation or function calls
    AST_COMPOUND,       // For compound statements (e.g., blocks of code)
    AST_NOOP,           // No operation
    AST_VARIABLE,       // For variable references
    AST_FUNCTION_CALL,  // For function calls
    AST_ARRAY,          // For array literals
    AST_BINARY_OP,      // For binary operations (e.g., +, -, *)
    AST_UNARY_OP        // For unary operations (e.g., -)
};

// Structure representing an Abstract Syntax Tree (AST) node.
typedef struct AST_STRUCT
{
    enum ast_type type;            // Type of AST node
    struct SCOPE_STRUCT *scope;    // Scope the AST node belongs to

    /* AST_LITERAL */
    int int_value;                 // Integer value (if this node is an integer literal)
    char *string_value;            // String value (if this node is a string literal)
    int bool_value;                // Boolean value (if this node is a boolean literal)
    char char_value;               // Character value (if this node is a character literal)
    float real_value;              // Real value (if this node is a floating point literal)

    /* AST_VARIABLE */
    char *variable_name;           // Name of the variable (if this node is a variable reference)

    /* AST_ASSIGNMENT */
    struct AST_STRUCT *value;      // Value to assign (expression being assigned)
    int constant;                  // Whether the assignment is a constant

    /* AST_INSTANTIATION / AST_FUNCTION_CALL */
    char *instantiated_type;       // Name of the type or function being instantiated/called
    struct AST_STRUCT **arguments; // List of arguments (for function calls/instantiations)
    size_t arguments_size;         // Number of arguments

    /* AST_COMPOUND */
    struct AST_STRUCT **compound_value; // List of statements (if this node is a compound statement)
    size_t compound_size;           // Number of statements in the compound

    /* AST_ARRAY */
    struct AST_STRUCT **elements;  // List of elements (if this node is an array literal)
    size_t elements_size;          // Number of elements in the array

    /* AST_BINARY_OP */
    struct AST_STRUCT *left;       // Left operand (if this node is a binary operation)
    struct AST_STRUCT *right;      // Right operand (if this node is a binary operation)
    char *binary_op;               // Operator as a string (e.g., "+", "-", "*", "DIV", "MOD")

    /* AST_UNARY_OP */
    struct AST_STRUCT *operand;    // Operand (if this node is a unary operation)
    char *unary_op;                // Operator as a string (e.g., "-")

} ast_;

// Function to initialize an AST node of a given type.
ast_ *init_ast(enum ast_type type);

// Function to convert an AST type to a string representation.
const char *ast_type_to_string(enum ast_type type);

#endif
