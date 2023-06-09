#include <string>

#include "VMTranslator.h"
#include "helper.h"

using namespace std;

string VMTranslator::moduleName = "defMod"; // default module name

string VMTranslator::fnLabelEncode(string name) {
	return "FUNC."+moduleName+"."+name;
}

string VMTranslator::fnLabelEncode(string modName, string name) {
	return "FUNC."+modName+"."+name;
}

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
string VMTranslator::vm_push(string segment, int offset)
{
    string trans = "";
    trans += "// push " + segment + to_string(offset) + "\n";
    if (segment == "constant") {
        trans += "@" + to_string(offset) + "\n"; // load index into A
        trans += "D=A\n"; // move it to D
        trans += "@SP\n"; // load 0 into A (M[0] contains stack pointer)
        trans += "A=M\n"; // load stack pointer
        trans += "M=D\n"; // put D onto stack
        trans += "@SP\n"; // load stack pointer address into A
        trans += "M=M+1\n"; // increment stack pointer
    } else if (segment == "static") {
        trans += "@" + to_string(offset + 16) + "\n";
        trans += "D=M\n";
        trans += "@SP\n";
        trans += "A=M\n"; 
        trans += "M=D\n";
        trans += "@SP\n";
        trans += "M=M+1\n";
    } else {
        trans += "@" + to_string(offset) + "\n"; // get value into D
        trans += "D=A\n";
        
        if (segment == "this") {
            trans += "@THIS\n";
            trans += "A=M+D\n";
        } else if (segment == "that") {
            trans += "@THAT\n";
            trans += "A=M+D\n";
        } else if (segment == "argument") {
            trans += "@ARG\n";
            trans += "A=M+D\n";
        } else if (segment == "local") {
            trans += "@LCL\n";
            trans += "A=M+D\n";
        } else if (segment == "temp") {
            trans += "@5\n";
            trans += "A=A+D\n";
        } else if (segment == "pointer") {
            trans += "@3\n";
            trans += "A=A+D\n";
        }

        trans += "D=M\n";
        trans += "@SP\n"; // put it onto the stack
        trans += "A=M\n";
        trans += "M=D\n";
        trans += "@SP\n"; // increment the stack pointer
        trans += "M=M+1\n";
    }
    
    return trans;
}

/** Generate Hack Assembly code for a VM pop operation */
string VMTranslator::vm_pop(string segment, int offset)
{
    string trans = "";
    trans += "// pop " + segment + to_string(offset) + "\n";
    if (segment == "static") {
        trans += "@SP\n"; // pop value into D
        trans += "AM=M-1\n";
        trans += "D=M\n";
        trans += "@" + to_string(16 + offset) + "\n";
        trans += "M=D\n";
    } else {
        trans += "@" + to_string(offset) + "\n"; // get address into R13
        trans += "D=A\n";

        if (segment == "this") {
            trans += "@THIS\n";
            trans += "D=M+D\n";
            trans += "@R13\n";
            trans += "M=D\n";
            trans += "@SP\n"; // pop value into D
            trans += "AM=M-1\n";
            trans += "D=M\n";
            trans += "@R13\n"; // address back in A (no touchy D)
            trans += "A=M\n";
            trans += "M=D\n";
        } else if (segment == "that") {
            trans += "@THAT\n";
            trans += "D=M+D\n"; 
            trans += "@R13\n";
            trans += "M=D\n";
            trans += "@SP\n"; // pop value into D
            trans += "AM=M-1\n";
            trans += "D=M\n";
            trans += "@R13\n"; // address back in A (no touchy D)
            trans += "A=M\n";
            trans += "M=D\n";
        } else if (segment == "argument") {
            trans += "@ARG\n";
            trans += "D=M+D\n"; 
            trans += "@R13\n";
            trans += "M=D\n";
            trans += "@SP\n"; // pop value into D
            trans += "AM=M-1\n";
            trans += "D=M\n";
            trans += "@R13\n"; // address back in A (no touchy D)
            trans += "A=M\n";
            trans += "M=D\n";
        } else if (segment == "local") {
            trans += "@LCL\n";
            trans += "D=M+D\n"; 
            trans += "@R13\n";
            trans += "M=D\n";
            trans += "@SP\n"; // pop value into D
            trans += "AM=M-1\n";
            trans += "D=M\n";
            trans += "@R13\n"; // address back in A (no touchy D)
            trans += "A=M\n";
            trans += "M=D\n";
        } else if (segment == "pointer") {
            trans += "@3\n";
            trans += "D=A+D\n"; 
            trans += "@R13\n";
            trans += "M=D\n";
            trans += "@SP\n"; // pop value into D
            trans += "AM=M-1\n";
            trans += "D=M\n";
            trans += "@R13\n"; // address back in A (no touchy D)
            trans += "A=M\n";
            trans += "M=D\n";
        } else if (segment == "temp") {
            trans += "@5\n";
            trans += "D=A+D\n"; 
            trans += "@R13\n";
            trans += "M=D\n";
            trans += "@SP\n"; // pop value into D
            trans += "AM=M-1\n";
            trans += "D=M\n";
            trans += "@R13\n"; // address back in A (no touchy D)
            trans += "A=M\n";
            trans += "M=D\n";
        }
    }

    return trans;
}

/** Generate Hack Assembly code for a VM add operation */
string VMTranslator::vm_add()
{
    string trans = "";
    trans += "// add \n";
    trans += "@SP\n"; // pop first value into D
    trans += "AM=M-1\n";
    trans += "D=M\n";
    trans += "@SP\n"; // pop second value into M
    trans += "AM=M-1\n"; 
    trans += "M=D+M\n"; // push sum onto M
    trans += "@SP\n";
    trans += "M=M+1\n"; 
    return trans;    
}

/** Generate Hack Assembly code for a VM sub operation */
string VMTranslator::vm_sub()
{
    string trans = "";
    trans += "// sub \n";
    trans += "@SP\n"; // pop first value into D
    trans += "AM=M-1\n";
    trans += "D=M\n"; 
    trans += "@SP\n"; // pop second value into M
    trans += "AM=M-1\n"; 
    trans += "M=M-D\n"; // push difference onto M
    trans += "@SP\n";
    trans += "M=M+1\n"; 
    return trans;
}

/** Generate Hack Assembly code for a VM neg operation */
string VMTranslator::vm_neg()
{
    string trans = "";
    trans += "// neg \n";
    trans += "@SP\n"; // get (not pop) value into M
    trans += "A=M-1\n"; 
    trans += "M=-M\n"; // and negate it 
    return trans;
}

/** Generate Hack Assembly code for a VM eq operation */
string VMTranslator::vm_eq()
{
    string trans = "";
    static int count = 0;
    string label = to_string(count);
    count++;
    trans += "// eq " + label + "\n";
    trans += "@SP\n"; // pop first value into D
    trans += "AM=M-1\n";
    trans += "D=M\n";
    trans += "@SP\n"; // get second value into M
    trans += "A=M-1\n";
    trans += "D=M-D\n"; // D = older value - newer
    trans += "M=-1\n"; // tentatively put true on stack
    trans += "@eqTrue" + label + "\n"; // and jump to end if so
    trans += "D;JEQ\n";
    trans += "@SP\n"; // set to false otherwise
    trans += "A=M-1\n";
    trans += "M=0\n";
    trans += "(eqTrue" + label + ")\n";
    return trans;
}

/** Generate Hack Assembly code for a VM gt operation */
string VMTranslator::vm_gt()
{
    string trans = "";
    static int count = 0;
    string label = to_string(count);
    count++;
    trans += "// gt " + label + "\n";
    trans += "@SP\n"; // pop first value into D
    trans += "AM=M-1\n";
    trans += "D=M\n";
    trans += "@SP\n"; // get second value into M
    trans += "A=M-1\n";
    trans += "D=M-D\n"; // D = older value - newer
    trans += "M=-1\n"; // tentatively put true on stack
    trans += "@gtTrue" + label + "\n"; // and jump to end if so
    trans += "D;JGT\n";
    trans += "@SP\n"; // set to false otherwise
    trans += "A=M-1\n";
    trans += "M=0\n";
    trans += "(gtTrue" + label + ")\n";
    return trans;
}

/** Generate Hack Assembly code for a VM lt operation */
string VMTranslator::vm_lt()
{
    string trans = "";
    static int count = 0;
    string label = to_string(count);
    count++;
    trans += "// lt " + label + "\n";
    trans += "@SP\n"; // pop first value into D
    trans += "AM=M-1\n";
    trans += "D=M\n"; 
    trans += "@SP\n"; // get second value into M
    trans += "A=M-1\n";
    trans += "D=M-D\n"; // D = older value - newer
    trans += "M=-1\n"; // tentatively put true on stack
    trans += "@ltTrue" + label + "\n"; // and jump to end if so
    trans += "D;JLT\n";
    trans += "@SP\n"; // set to false otherwise
    trans += "A=M-1\n";
    trans += "M=0\n"; 
    trans += "(ltTrue" + label + ")\n";
    return trans;
}

/** Generate Hack Assembly code for a VM and operation */
string VMTranslator::vm_and()
{
    string trans = "";
    trans += "// and\n";
    trans += "@SP\n"; // pop first value into D
    trans += "AM=M-1\n";
    trans += "D=M\n";
    trans += "@SP\n"; // get second value into M
    trans += "A=M-1\n";
    trans += "M=D&M\n"; // put result back on stack
    return trans;
}

/** Generate Hack Assembly code for a VM or operation */
string VMTranslator::vm_or()
{
    string trans = "";
    trans += "// or\n";
    trans += "@SP\n"; // pop first value into D
    trans += "AM=M-1\n";
    trans += "D=M\n"; 
    trans += "@SP\n"; // get second value into M
    trans += "A=M-1\n";
    trans += "M=D|M\n"; // put result back on stack
    return trans;
}

/** Generate Hack Assembly code for a VM not operation */
string VMTranslator::vm_not()
{
    string trans = "";
    trans += "// not\n";
    trans += "@SP\n"; // get (not pop) value into M
    trans += "A=M-1\n"; 
    trans += "M=!M\n"; // and negate it 
    return trans;
}

/** Generate Hack Assembly code for a VM label operation */
string VMTranslator::vm_label(string label)
{
    helper out;
	out.ins(	"("+label+")", "new label: "+label	);
	return out.str();
}

/** Generate Hack Assembly code for a VM goto operation */
string VMTranslator::vm_goto(string label)
{
    helper out;
    // __a += in +"\n";
    out.ins("@" + label,"jump to" + label);
    out.ins("0;JMP");

    return out.str();
}

/** Generate Hack Assembly code for a VM if-goto operation */
string VMTranslator::vm_if(string label)
{
    helper out;

	out.ins("@SP", "if-goto " + label);
	out.ins("AM=M-1");
	out.ins("D=M");
	out.ins("@"+label);
	out.ins("D;JNE");

	return out.str();
}

/** Generate Hack Assembly code for a VM function operation */
string VMTranslator::vm_function(string function_name, int n_vars)
{
    helper out;
    // string fLabel = "FUNC."+moduleName+"."+ function_name;
	string fLabel = fnLabelEncode(function_name); // manually use "def" as the module name

	out.ins( "("+fLabel+")", "function declaration: "+function_name+" "+to_string(n_vars) );
	out.ins(	"@SP	");
	out.ins(	"A=M	");

	out.comment("clearing the stack for function: "+function_name);
	for (int i=0; i<n_vars; i++) {
		out.ins(	"M=0"			);
		out.ins(	"A=A+1"		);
	}
	out.comment("finished clearing");

	out.ins(	"D=A"		);
	out.ins(	"@SP"		);
	out.ins(	"M=D"		);

	return out.str();
}

/** Generate Hack Assembly code for a VM call operation */
string VMTranslator::vm_call(string function_name, int n_args)
{
    helper out;
    string lID;


	out.comment("call "+function_name);
	out.comment("store callers pointers");
	// R13 = SP, temporarily store the stack address in R13
	out.ins(	"@SP"	 );
	out.ins(	"D=M"	 );
	out.ins(	"@R13" );
	out.ins(	"M=D"  );
	// *(SP) = @RET
	out.ins(	"@RET."+lID		);
	out.ins(	"D=A"	 );
	out.ins(	"@SP"	 );
	out.ins(	"A=M"	 );
	out.ins(	"M=D"	 );
	// increment stack pointer
	out.ins(	"@SP"	 );
	out.ins(	"M=M+1"	);
	// *(SP) = LCL
	out.ins(	"@LCL","store LCL on stack"  );
	out.ins(	"D=M" 	);
	out.ins(	"@SP" 	);
	out.ins(	"A=M" 	);
	out.ins(	"M=D" 	);
	// increment stack pointer
	out.ins(	"@SP"	 );
	out.ins(	"M=M+1"	);
	// *(SP) = ARG
	out.ins(	"@ARG","store ARG on stack"  );
	out.ins(	"D=M" 	);
	out.ins(	"@SP" 	);
	out.ins(	"A=M" 	);
	out.ins(	"M=D" 	);
	// increment stack pointer
	out.ins(	"@SP"	 );
	out.ins(	"M=M+1"	);
	// *(SP) = THIS
	out.ins(	"@THIS","store THIS on stack"  );
	out.ins(	"D=M" 	);
	out.ins(	"@SP" 	);
	out.ins(	"A=M" 	);
	out.ins(	"M=D" 	);
	// increment stack pointer
	out.ins(	"@SP"	 );
	out.ins(	"M=M+1"	);
	// *(SP) = THAT
	out.ins(	"@THAT","store THAT on stack"  );
	out.ins(	"D=M" 	);
	out.ins(	"@SP" 	);
	out.ins(	"A=M" 	);
	out.ins(	"M=D" 	);
	// increment stack pointer
	out.ins(	"@SP"	 );
	out.ins(	"M=M+1"	);

	out.comment("setup ARG and LCL for callee");
	// ARG = R13 - n_vars
	out.ins(	"@R13","update ARG"  );
	out.ins(	"D=M" 		);
	out.ins(	"@"+to_string(n_args) );
	out.ins(	"D=D-A" 	);
	out.ins(	"@ARG" 		);
	out.ins(	"M=D"			);
	// LCL = SP
	out.ins(	"@SP","update LCL"  );
	out.ins(	"D=M" 		);
	out.ins(	"@LCL" 		);
	out.ins(	"M=D" 		);
	out.ins(	"@"+fnLabelEncode(function_name) 	);
	out.ins(	"0;JMP"		);
	out.ins(	"(RET."+lID+")"	);

	return out.str();
}

/** Generate Hack Assembly code for a VM return operation */
string VMTranslator::vm_return()
{
    helper out;
	out.comment("returning to caller");
	// R13 = *(LCL - 5)
	out.ins(	"@LCL"	);
	out.ins(	"D=M"	  );
	out.ins(	"@5"	  );
	out.ins(	"A=D-A"	);
	out.ins(	"D=M"	  );
	out.ins(	"@R13"	);
	out.ins(	"M=D"	  );
	// *(ARG) = *(SP - 1)
	out.ins(	"@SP"		);
	out.ins(	"A=M-1"	);
	out.ins(	"D=M"	  );
	out.ins(	"@ARG"	);
	out.ins(	"A=M"	  );
	out.ins(	"M=D"	  );
	// SP = ARG + 1
	out.ins(	"D=A+1"	 );
	out.ins(	"@SP"	   );
	out.ins(	"M=D"	   );
	// THAT = *(LCL - 1), LCL = LCL - 1
	out.ins(	"@LCL"	  );
	out.ins(	"AM=M-1"	);
	out.ins(	"D=M"	  	);
	out.ins(	"@THAT"	  );
	out.ins(	"M=D"	  	);
	// THIS = *(LCL - 1), LCL = LCL - 1
	out.ins(	"@LCL"	  );
	out.ins(	"AM=M-1"	);
	out.ins(	"D=M"	  	);
	out.ins(	"@THIS"	  );
	out.ins(	"M=D"	  	);
	// ARG = *(LCL - 1), LCL = LCL - 1
	out.ins(	"@LCL"	  );
	out.ins(	"AM=M-1"	);
	out.ins(	"D=M"	  	);
	out.ins(	"@ARG"	  );
	out.ins(	"M=D"	  	);
	// LCL = *(LCL - 1)
	out.ins(	"@LCL"	  );
	out.ins(	"A=M-1"		);
	out.ins(	"D=M"	  	);
	out.ins(	"@LCL"	  );
	out.ins(	"M=D"	  	);
	// jump back
	out.ins(	"@R13", "jump back to caller"	);
	out.ins(	"A=M"			);
	out.ins(	"0;JMP"		);

	return out.str();
}