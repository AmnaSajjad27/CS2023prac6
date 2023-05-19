#include <string>

#include "VMTranslator.h"

using namespace std;

/**
 * VMTranslator constructor
 */
VMTranslator::VMTranslator() {
    // Your code here
}

/**
 * VMTranslator destructor
 */
VMTranslator::~VMTranslator() {
    // Your code here
}

/** Generate Hack Assembly code for a VM push operation */
string VMTranslator::vm_push(string segment, int offset){
    return "";
}

/** Generate Hack Assembly code for a VM pop operation */
string VMTranslator::vm_pop(string segment, int offset)
{
    string translate = "";
    translate += "" + segment + to_string(offset) + "\n";
    if (segment == "constant")
    {
        // load index into A
        translate += "@" + to_string(offset) + "\n";
        // move to D
        translate += "D=A\n";
        // load 0 into A (M[0] = SP)
        translate += "@SP\n";
        // load SP
        translate += "A=M\n";
        // chuck D onto da stack 
        translate += "M=D\n";
        // update SP
        translate += "@SP\n";
        // increment P
        translate += "M=M+1\n"
    }
    else if (segment == "static")
    {
        translate += "@" + to_string(offset + 16) + "\n";
        translate += "D=M\n";
        translate += "@SP\n";
        translate += "A=M\n"; 
        translate += "M=D\n";
        translate += "@SP\n";
        translate += "M=M+1\n"; 
    }
    else 
    {
        translate += "@" + to_string(offset) + "\n"; // get value into D
        translate += "D=A\n";
        
        if (segment == "this") {
            translate += "@THIS\n";
            translate += "A=M+D\n";
        } else if (segment == "that") {
            translate += "@THAT\n";
            translate += "A=M+D\n";
        } else if (segment == "argument") {
            translate += "@ARG\n";
            translate += "A=M+D\n";
        } else if (segment == "local") {
            translate += "@LCL\n";
            translate += "A=M+D\n";
        } else if (segment == "temp") {
            translate += "@5\n";
            translate += "A=A+D\n";
        } else if (segment == "pointer") {
            translate += "@3\n";
            translate += "A=A+D\n";
        }

        translate += "D=M\n";
        translate += "@SP\n"; // put it onto the stack
        translate += "A=M\n";
        translate += "M=D\n";
        translate += "@SP\n"; // increment the stack pointer
        translate += "M=M+1\n";
    }

    return translate;
}

/** Generate Hack Assembly code for a VM add operation */
string VMTranslator::vm_add(){
    return "";
}

/** Generate Hack Assembly code for a VM sub operation */
string VMTranslator::vm_sub(){
    return "";
}

/** Generate Hack Assembly code for a VM neg operation */
string VMTranslator::vm_neg(){
    return "";
}

/** Generate Hack Assembly code for a VM eq operation */
string VMTranslator::vm_eq(){
    return "";
}

/** Generate Hack Assembly code for a VM gt operation */
string VMTranslator::vm_gt(){
    return "";
}

/** Generate Hack Assembly code for a VM lt operation */
string VMTranslator::vm_lt(){
    return "";
}

/** Generate Hack Assembly code for a VM and operation */
string VMTranslator::vm_and(){
    return "";
}

/** Generate Hack Assembly code for a VM or operation */
string VMTranslator::vm_or(){
    return "";
}

/** Generate Hack Assembly code for a VM not operation */
string VMTranslator::vm_not(){
    return "";
}

/** Generate Hack Assembly code for a VM label operation */
string VMTranslator::vm_label(string label){
    return "";
}

/** Generate Hack Assembly code for a VM goto operation */
string VMTranslator::vm_goto(string label){
    return "";
}

/** Generate Hack Assembly code for a VM if-goto operation */
string VMTranslator::vm_if(string label){
    return "";
}

/** Generate Hack Assembly code for a VM function operation */
string VMTranslator::vm_function(string function_name, int n_vars){
    return "";
}

/** Generate Hack Assembly code for a VM call operation */
string VMTranslator::vm_call(string function_name, int n_args){
    return "";
}

/** Generate Hack Assembly code for a VM return operation */
string VMTranslator::vm_return(){
    return "";
}