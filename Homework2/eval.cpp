//
//  eval.cpp
//  Homework2
//
//  Created by Edmund Zhi on 4/23/21.
//

#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

int precedence(char ch) {
    if (ch == '!') {
        return 3;
    }
    else if (ch == '&') {
        return 2;
    }
    else if (ch == '^') {
        return 1;
    }
    else {
        return 9;
    }
}

int evaluate(string infix, string& postfix, bool& result) {
    // convert to postfix and check for syntax
    string converted = "";
    stack<char> operatorStack;
    char prev = '&';
    int openCounter = 0;
    int closedCounter = 0;
    for (int i = 0; i < infix.size(); i++) {
        char ch = infix[i];
        switch (ch) {
            // case operand
            case 'T':
            case 'F':
                converted += ch;
                break;
            case ' ':
                continue;
            case '(':
                if (prev == ')' || prev == 'T' || prev == 'F') {
                    // improper syntax
                    return 1;
                }
                operatorStack.push(ch);
                openCounter++;
                break;
            case ')':
                // closed parenthesis must follow one of these, cannot follow an operator
                if (prev != ')' && prev != 'T' && prev != 'F') {
                    return 1;
                }
                while(!operatorStack.empty() && operatorStack.top() != '(') {
                    char toInsert = operatorStack.top();
                    converted += toInsert;
                    operatorStack.pop();
                }
                if (operatorStack.empty()) {
                    return 1;
                }
                operatorStack.pop(); // removes the '('
                closedCounter++;
                break;
            // case operators
            // separate case for unary operator '!'
            case '!':
                // exclamation points must precede, not succeed, one of these three digits
                if (prev == ')' || prev == 'T' || prev == 'F') {
                    // improper syntax
                    return 1;
                }
                operatorStack.push(ch);
                break;
            case '&':
            case '^':
                // operators must follow one of these 3 characters
                if (prev != ')' && prev != 'T' && prev != 'F') {
                    return 1;
                }
                while(!operatorStack.empty() && operatorStack.top() != '(' && (precedence(ch) <= precedence(operatorStack.top()))) {
                    char toInsert = operatorStack.top();
                    converted += toInsert;
                    operatorStack.pop();
                }
                operatorStack.push(ch);
                break;
            default:
                // There are characters in the string that do not belong
                return 1;
        }
        prev = ch;
    }
   
    // postfix expression must end with closed parenthesis or operand
    if (prev != ')' && prev != 'T' && prev != 'F') {
        return 1;
    }
    // number of closed parenthesis must equal open parenthesis
    if (closedCounter != openCounter) {
        return 1;
    }
    while (!operatorStack.empty()) {
        char toInsert = operatorStack.top();
        converted += toInsert;
        operatorStack.pop();
    }
    if (converted.size() == 0) {
        // nothing happened, bad string
        return 1;
    }

    // converted is now finished, apply converted to postfix
    postfix = converted;
    
    // Evaluate
    //Initialize the operand stack to empty
    stack<bool> operandStack;
    // for each character ch in the postfix string
    for (int i = 0; i < postfix.size(); i++) {
        char ch = postfix[i];
        // if ch is an operand
        if (ch == 'T') {
            //push the value that ch represents onto the operand stack
            operandStack.push(true);
        }
        // two possible operands
        else if (ch == 'F') {
            operandStack.push(false);
        }
        else { // ch is an operator
            // set operand2 to the top of the operand stack
            bool operand2 = operandStack.top();
            // pop the stack
            operandStack.pop();
            if (ch == '!') { // ch is a unary operator
                operandStack.push(!operand2);
            }
            else { // ch is a binary operator
                // set operand1 to the top of the operand stack
                bool operand1 = operandStack.top();
                //  pop the stack
                operandStack.pop();
                // apply the operation that ch represents to operand1 and operand2, and push the result onto the stack
                if (ch == '&') {
                    operandStack.push(operand1 && operand2);
                }
                else if (ch == '^') {
                    operandStack.push(operand1 != operand2);
                }
                else {
                    // error has occured
                    return 1;
                }
            }
        }
    }
    // When the loop is finished, the operand stack will contain one item, the result of evaluating the expression
    if (!(operandStack.size() == 1)) {
        return 1;
    }
    // no detected syntax errors
    result = operandStack.top();
    return 0;
}

int main()
{
    string pf;
    bool answer;
    assert(evaluate("T^ F", pf, answer) == 0  &&  pf == "TF^"  &&  answer);
    assert(evaluate("T^", pf, answer) == 1);
    assert(evaluate("F F", pf, answer) == 1);
    assert(evaluate("TF", pf, answer) == 1);
    assert(evaluate("()", pf, answer) == 1);
    assert(evaluate("()T", pf, answer) == 1);
    assert(evaluate("T(F^T)", pf, answer) == 1);
    assert(evaluate("T(&T)", pf, answer) == 1);
    assert(evaluate("(T&(F^F)", pf, answer) == 1);
    assert(evaluate("T|F", pf, answer) == 1);
    assert(evaluate("", pf, answer) == 1);
    assert(evaluate("F  ^  !F & (T&F) ", pf, answer) == 0
           &&  pf == "FF!TF&&^"  &&  !answer);
    assert(evaluate(" F  ", pf, answer) == 0 &&  pf == "F"  &&  !answer);
    assert(evaluate("((T))", pf, answer) == 0 &&  pf == "T"  &&  answer);
    assert(evaluate("T", pf, answer) == 0 && pf == "T" && answer == true);
    assert(evaluate("(F)", pf, answer) == 0 && pf == "F" && answer == false);
    assert(evaluate("T ^(F)", pf, answer) == 0 && pf == "TF^" && answer == true);
    assert(evaluate("T ^ !F", pf, answer) == 0 && pf == "TF!^" && answer == false);
    assert(evaluate("!(T&!!  !!F)", pf, answer) == 0 && pf == "TF!!!!&!" && answer == true);
    assert(evaluate("!T&F ", pf, answer) == 0 && pf == "T!F&" && answer == false);
    assert(evaluate("T^F&F", pf, answer) == 0 && pf == "TFF&^" && answer == true);
    assert(evaluate("T&! (F^T&T^F  )^!!!(F&T&F)", pf, answer) == 0 && pf == "TFTT&^F^!&FT&F&!!!^" && answer == true);
    assert(evaluate("TS DF:L", pf, answer) == 1);
    assert(evaluate("( F^T&!  F)", pf, answer) == 0 && pf == "FTF!&^" && answer == true);
    assert(evaluate("(( F))&((! T))", pf, answer) == 0 && pf == "FT!&" && answer == false);
    assert(evaluate("T^!F&T^T^(!F&T^!!T&F)", pf, answer) == 0 && answer == false);
    assert(evaluate("!!(F^T&F)", pf, answer) == 0 && answer == false);
    assert(evaluate("F^!T&T^!F", pf, answer) == 0 && answer == true);
    assert(evaluate("T&(F^!T&  T^!F)&!!(F^T& !F)", pf, answer) == 0 && answer == true);
    assert(evaluate("T^ F", pf, answer) == 0 && pf == "TF^"  &&  answer);
    assert(evaluate("T^", pf, answer) == 1);
    assert(evaluate("F F", pf, answer) == 1);
    assert(evaluate("TF", pf, answer) == 1);
    assert(evaluate("()", pf, answer) == 1);
    assert(evaluate("()T", pf, answer) == 1);
    assert(evaluate("T(F^T)", pf, answer) == 1);
    assert(evaluate("T(&T)", pf, answer) == 1);
    assert(evaluate("(T&(F^F)", pf, answer) == 1);
    assert(evaluate("T|F", pf, answer) == 1);
    assert(evaluate("T&()F", pf, answer) == 1);
    assert(evaluate("T(&)F", pf, answer) == 1);
    assert(evaluate("T&F^T()", pf, answer) == 1);
    assert(evaluate("T&(F&T))&((T&T)", pf, answer) == 1);
    assert(evaluate("((T))(F)", pf, answer) == 1);
    assert(evaluate("F  ^  !F & (T&F) ", pf, answer) == 0
           && pf == "FF!TF&&^" && !answer);
    assert(evaluate(" F  ", pf, answer) == 0 && pf == "F" && !answer);
    assert(evaluate("((((T))))", pf, answer) == 0 && pf == "T"  &&  answer);
    assert(evaluate("(((T^F)()))", pf, answer) == 1);
    assert(evaluate("T^ F", pf, answer) == 0 && pf == "TF^" && answer);
    assert(evaluate("T^", pf, answer) == 1);
    assert(evaluate("F F", pf, answer) == 1);
    assert(evaluate("TF", pf, answer) == 1);
    assert(evaluate("()", pf, answer) == 1);
    assert(evaluate("()T", pf, answer) == 1);
    assert(evaluate("T(F^T)", pf, answer) == 1);
    assert(evaluate("T(&T)", pf, answer) == 1);
    assert(evaluate("(T&(F^F)", pf, answer) == 1);
    assert(evaluate("T&(F^F))", pf, answer) == 1);
    assert(evaluate("T&(F^F)", pf, answer) == 0 && !answer && pf == "TFF^&");
    assert(evaluate("     T   &     (    F   ^   F   )   ", pf, answer) == 0 && !answer && pf == "TFF^&");
    assert(evaluate("T&(F^(T^(F&T)))", pf, answer) == 0 && answer && pf == "TFTFT&^^&");
    assert(evaluate("!T&(F^(T^(F&T)))", pf, answer) == 0 && !answer && pf == "T!FTFT&^^&");
    assert(evaluate("!T&!(T^F)", pf, answer) == 0 && !answer && pf == "T!TF^!&");
    assert(evaluate("((T))", pf, answer) == 0 && answer && pf == "T");
    assert(evaluate("((F))", pf, answer) == 0 && !answer && pf == "F");
    assert(evaluate("(!)", pf, answer) == 1);
    assert(evaluate("(^)", pf, answer) == 1);
    assert(evaluate("(&)", pf, answer) == 1);
    assert(evaluate("(T!)", pf, answer) == 1);
    assert(evaluate("(T^)", pf, answer) == 1);
    assert(evaluate("(T&)", pf, answer) == 1);
    assert(evaluate("(&T)", pf, answer) == 1);
    assert(evaluate("(^T)", pf, answer) == 1);
    assert(evaluate("((T)", pf, answer) == 1);
    assert(evaluate("(T))", pf, answer) == 1);
    assert(evaluate("T|F", pf, answer) == 1);
    assert(evaluate("", pf, answer) == 1);
    assert(evaluate("    ", pf, answer) == 1);
    assert(evaluate("!", pf, answer) == 1);
    assert(evaluate("^", pf, answer) == 1);
    assert(evaluate("&", pf, answer) == 1);
    assert(evaluate("TF", pf, answer) == 1);
    assert(evaluate("FT", pf, answer) == 1);
    assert(evaluate("F  ^  !F & (T&F) ", pf, answer) == 0 && pf == "FF!TF&&^" && !answer);
    assert(evaluate(" F  ", pf, answer) == 0 && pf == "F" && !answer);
    assert(evaluate("((T))", pf, answer) == 0 && pf == "T" && answer);
    assert(evaluate("", pf, answer) == 1);
    assert(evaluate("(sdf)", pf, answer) == 1);
    assert(evaluate("()sdf", pf, answer) == 1);
    assert(evaluate("!", pf, answer) == 1);
    assert(evaluate("(", pf, answer) == 1);
    assert(evaluate("!!!", pf, answer) == 1);
    assert(evaluate("TF", pf, answer) == 1);
    assert(evaluate("T(F", pf, answer) == 1);
    assert(evaluate("T(F)", pf, answer) == 1);
    assert(evaluate("()()", pf, answer) == 1);
    assert(evaluate("&!", pf, answer) == 1);
    assert(evaluate("^!", pf, answer) == 1);
    assert(evaluate("&()", pf, answer) == 1);
    assert(evaluate("!()", pf, answer) == 1);
    assert(evaluate("F && F", pf, answer) == 1);
    assert(evaluate("T", pf, answer) == 0);
    assert(evaluate("!T", pf, answer) == 0);
    assert(evaluate("(!(T))", pf, answer) == 0);
    assert(evaluate("!(((T&F)))", pf, answer) == 0);
    assert(evaluate("!T", pf, answer) == 0);
    assert(evaluate("!( T & !F ^ T)", pf, answer) == 0);
    assert(evaluate("T & !!F", pf, answer) == 0);
    assert(evaluate("F", pf, answer) == 0);
    assert(evaluate("T", pf, answer) == 0 && pf == "T" && answer);
    assert(evaluate("(!(T))", pf, answer) == 0 && pf == "T!" && !answer);
    assert(evaluate("!(((T&F)))", pf, answer) == 0 && pf == "TF&!" && answer);
    assert(evaluate("!T", pf, answer) == 0 && pf == "T!" && !answer);
    assert(evaluate("F", pf, answer) == 0 && pf == "F" && !answer);
    assert(evaluate("!(T&!F) ^!T& !F", pf, answer) == 0 && pf == "TF!&!T!F!&^" && !answer);
    assert(evaluate("!!!!T", pf, answer) == 0 && pf == "T!!!!" && answer);
    assert(evaluate("!!T", pf, answer) == 0);
    assert(evaluate("!!!!T", pf, answer) == 0);
    assert(evaluate("!!(T) & !F ^ T", pf, answer) == 0);
    assert(evaluate("!!!!T", pf, answer) == 0 && pf == "T!!!!" && answer);
    assert(evaluate("T & !!F", pf, answer) == 0 && pf == "TF!!&" && !answer);
    assert(evaluate("!( T & !F ^ T)", pf, answer) == 0 && pf == "TF!&T^!" && answer);
    assert(evaluate("!!( T & !F ^ T)", pf, answer) == 0 && pf == "TF!&T^!!" && !answer);
    assert(evaluate("!!!( T & !F ^ T)", pf, answer) == 0 && pf == "TF!&T^!!!" && answer);
    assert(evaluate("!!(T) & !F ^ T", pf, answer) == 0 && pf == "T!!F!&T^" && !answer);
    assert(evaluate("(T&T)(T&T)", pf, answer) == 1);
    assert(evaluate("T!T", pf, answer) == 1);
    assert(evaluate("f&t", pf, answer) == 1);
    assert(evaluate("(((T&F)^((T)))", pf, answer) == 1);
    cerr<<"Passed all test cases!"<<endl;
}
