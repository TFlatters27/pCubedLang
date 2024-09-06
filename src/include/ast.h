#ifndef AST_H
#define AST_H

#include <stdlib.h>

// Enum representing the different types of AST nodes.
enum ast_type
{
    AST_COMPOUND, // For compound statements (e.g., blocks of code)
    AST_NOOP,     // No operation

    AST_INTEGER,   // For integer literals
    AST_REAL,      // For real literals
    AST_CHARACTER, // For character literals
    AST_STRING,    // For string literals
    AST_BOOLEAN,   // For boolean literals
    AST_ARRAY,     // For array literals

    AST_ASSIGNMENT,    // For variable assignments
    AST_VARIABLE,      // For variable references
    AST_RECORD_ACCESS, // For accessing a field in a record
    AST_ARRAY_ACCESS,  // For accessing an array element
    AST_INSTANTIATION, // For object instantiation or function calls
    AST_ARITHMETIC_EXPRESSION,
    AST_BOOLEAN_EXPRESSION,

    AST_RECORD,          // For record creation
    AST_SUBROUTINE,      // For subroutine definitions
    AST_RETURN,          // For RETURN statements in subroutines
    AST_OUTPUT,          // For OUTPUT statements
    AST_DEFINITE_LOOP,   // For count-controlled loops (FOR loops)
    AST_INDEFINITE_LOOP, // For condition-controlled loops (REPEAT & WHILE loops)
    AST_SELECTION,       // For selection control blocks (IF-ELSE-ELSE IF statements)
};

// Structure representing an element (field) in a record.
typedef struct AST_RECORD_ELEMENT_STRUCT
{
    char *element_name;      // Name of the element (e.g., "name", "age")
    struct AST_STRUCT *type; // Type of the element (e.g., AST_STRING, AST_ARRAY)
} ast_record_element_;

// Structure representing an Abstract Syntax Tree (AST) node.
typedef struct AST_STRUCT
{
    enum ast_type type;         // Type of AST node
    struct SCOPE_STRUCT *scope; // Scope the AST node belongs to

    /* AST_COMPOUND */
    struct AST_STRUCT **compound_value; // List of statements (if this node is a compound statement)
    size_t compound_size;               // Number of statements in the compound

    /* AST_LITERAL */
    int int_value;                      // Integer value (if this node is an integer literal)
    float real_value;                   // Real value (if this node is a floating-point literal)
    char char_value;                    // Character value (if this node is a character literal)
    int boolean_value;                  // Boolean value (if this node is a boolean literal)
    char *string_value;                 // String value (if this node is a string literal)
    struct AST_STRUCT **array_elements; // List of elements (if this node is an array literal)

    /* AST_ASSIGNMENT */
    struct AST_STRUCT *lhs; // Left-hand side value to assign (variable or record access)
    struct AST_STRUCT *rhs; // Right-hand side value to assign (expression being assigned)
    int constant;           // Whether the assignment is a constant

    /* AST_VARIABLE */
    char *variable_name; // Name of the variable (if this node is a variable reference)

    /* AST_RECORD_ACCESS */
    char *field_name; // The field being accessed (e.g., "passed")

    /* AST_ARRAY_ACCESS */
    struct AST_STRUCT **index; // List of indices (for multidimensional array access)

    /* AST_INSTANTIATION */
    char *class_name; // Name of the class
    struct AST_STRUCT **arguments; // List of arguments (for function calls/instantiations)

    /* AST_EXPRESSION */
    struct AST_STRUCT *left;  // Left operand (if this node is a binary operation)
    struct AST_STRUCT *right; // Right operand (if this node is a binary operation)
    char *op;           // Operator as a string (e.g., "+", "-", "*", "DIV", "MOD")

    /* AST_RECORD */
    char *record_name;                     // Name of the record (e.g., "Student")
    ast_record_element_ **record_elements; // List of element declarations (e.g., fields like "name", "age")

    /* AST_SUBROUTINE */
    char *subroutine_name;          // Name of the subroutine
    struct AST_STRUCT **parameters; // List of parameters (AST_VARIABLE nodes)
    struct AST_STRUCT **body;       // Body of the subroutine (List of AST nodes)

    /* AST_RETURN */
    struct AST_STRUCT *return_value; // Return value (if any) of the subroutine

    /* AST_OUTPUT */
    int output_expressions_size;
    struct AST_STRUCT **output_expressions; // List of expressions to be output

    /* AST_DEFINITE_LOOP */
    struct AST_STRUCT *loop_variable;   // Variable being iterated
    struct AST_STRUCT *start_expr;      // Start expression (if FOR <var> <- start TO end [STEP step])
    struct AST_STRUCT *end_expr;        // End expression (if FOR <var> <- start TO end [STEP step])
    struct AST_STRUCT *step_expr;       // Step expression (if any)
    struct AST_STRUCT *collection_expr; // Collection to iterate over (if FOR <var> IN collection)

    /* AST_INDEFINITE_LOOP */
    struct AST_STRUCT *condition;  // Condition to evaluate for the loop (e.g., "a < 4")
    struct AST_STRUCT **loop_body; // Body of the loop (list of statements)

    /* AST_SELECTION */
    struct AST_STRUCT *if_condition;        // Condition for IF block
    struct AST_STRUCT **if_body;            // List of AST nodes representing the IF body
    struct AST_STRUCT **else_if_conditions; // List of conditions for ELSE IF blocks
    struct AST_STRUCT ***else_if_bodies;    // List of AST node lists representing ELSE IF bodies
    struct AST_STRUCT **else_body;          // List of AST nodes representing the ELSE body
} ast_;

// Function to initialize an AST node of a given type.
ast_ *init_ast(enum ast_type type);

// Function to convert an AST type to a string representation.
const char *ast_type_to_string(enum ast_type type);

#endif // AST_H
