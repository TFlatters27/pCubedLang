import sys

##########################
# CONSTANTS
##########################
DIGITS = '0123456789'

##########################
# CONSTANTS
##########################
class Error:
    def __init__(self, pos_start, pos_end, name, data):
        self.pos_start = pos_start
        self.pos_end = pos_end
        self.name = name
        self.data = data
        
    def as_string(self):
        result = f"{self.name}: {self.data} \n"
        result += f"File {self.pos_start.fn}, line {self.pos_start.ln + 1}"
        return result
    
class IllegalCharError(Error):
    def __init__(self, pos_start, pos_end, data):
        super().__init__(pos_start, pos_end, "Illegal Character", data)
        
##########################
# POSITION
##########################

class Position():
    def __init__(self, idx, ln, col, fn, fc):
        self.idx = idx
        self.ln = ln
        self.col = col
        self.fn = fn
        self.fc = fc
        
    def advance(self, current_char):
        self.idx += 1
        self.col += 1
        
        if(current_char == '\n'):
            self.ln += 1
            self.col = 0
            
        return self
    
    def copy(self):
        return Position(self.idx, self.ln, self.col, self.fn, self.fc)
    
    
##########################
# TOKENS
##########################

TK_INT = "TK_INT"
TK_FLOAT = "TK_FLOAT"

TK_PLUS = "PLUS"
TK_MINUS = "MINUS"
TK_MUL = "MUL"
TK_DIV = "DIV"
TK_LBRA = "LBRA"
TK_RBRA = "RBRA"

class Token:
    def __init__(self, type_, value = None):
        self.type = type_
        self.value = value
        
    def __repr__(self):
        if self.value: return f"{self.type}:{self.value}"
        return f"{self.type}"
    
##########################
# LEXER
##########################

class Lexer:
    def __init__(self, fn, text):
        self.fn = fn
        self.text = text
        self.pos = Position(-1,0,-1, fn, text)
        self.current_char = None
        self.advance()
    
    def advance(self):
        self.pos.advance(self.current_char)
        self.current_char = self.text[self.pos.idx] if self.pos.idx < len(self.text) else None    
    
    def make_tokens(self):
        tokens = []
        while self.current_char != None:
            if(self.current_char in ' \t'):
                self.advance()
            elif (self.current_char in DIGITS):
                tokens.append(self.make_number())  
            elif (self.current_char == '+'):
                tokens.append(Token(TK_PLUS))
                self.advance()
            elif (self.current_char == '-'):
                tokens.append(Token(TK_MINUS))
                self.advance()
            elif (self.current_char == '*'):
                tokens.append(Token(TK_MUL))
                self.advance()
            elif (self.current_char == '/'):
                tokens.append(Token(TK_DIV))
                self.advance()
            elif (self.current_char == '('):
                tokens.append(Token(TK_LBRA))
                self.advance()
            elif (self.current_char == ')'):
                tokens.append(Token(TK_RBRA))
                self.advance()
            elif (self.current_char == "Q"):
                sys.exit("Exited successfully")    
            else: 
                pos_start = self.pos.copy()
                char = self.current_char
                self.advance()
                return [], IllegalCharError(pos_start, self.pos, "'" + char + "'")
            
        return tokens, None
    
    def make_number(self):
        number_str = ""
        point_count = 0
        
        while self.current_char != None and self.current_char in DIGITS + '.':
            if(self.current_char == '.'):
                if(point_count == 1): break
                point_count += 1
                number_str += '.'
            else:
                number_str += self.current_char
            
            self.advance()
                
        if(point_count == 0):
            return Token(TK_INT, int(number_str))
        else:
            return Token(TK_FLOAT, float(number_str))
            
##########################
# RUN
##########################     
def run(fn, text):
    lexer = Lexer(fn, text)
    tokens, error = lexer.make_tokens()
    
    return tokens, error 