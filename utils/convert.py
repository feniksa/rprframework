import argparse, sys, re


# 
def make_name_input(s:str):
    r: str = ""
    words = s.split('_')
    for l in words:
        r += l.capitalize()

    if r.isnumeric():
        r = "Input" + r

    return r

def make_name_node(s:str):
    r: str = ""
    words = s.split('_')
    for l in words:
        r += l.capitalize()

    if r.isnumeric():
        r = "Node" + r

    return r

def make_name_context(s:str):
    r: str = ""
    words = s.split('_')
    for l in words:
        r += l.capitalize()

    if r.isnumeric():
        r = "Context" + r

    return r



# Initialize parser
parser = argparse.ArgumentParser()

# Adding optional argument
parser.add_argument("-o", "--output", help = "Output header file")
parser.add_argument("-i", "--input", help = "Input header file RadeonProRender.h")

# Read arguments from command line
args = parser.parse_args()

if not args.input:
    print("No required parameter: -i")
    sys.exit(-1)

if not args.output:
    print("No required parameter: -o")
    sys.exit(-1)


#enum class CameraType
#{
#	Perpective = RPR_CAMERA_MODE_PERSPECTIVE,
#	Ortho      = RPR_CAMERA_MODE_ORTHOGRAPHIC
#};


material_inputs = []
material_nodetypes = []
context_input_types = []

prog_material_input = re.compile('#define\s+RPR_MATERIAL_INPUT_([A-Za-z0-9_]+)\s+(0x[0-9abcdefABCDEF]+)')
prog_material_node = re.compile('#define\s+RPR_MATERIAL_NODE_([A-Za-z0-9_]+)\s+(0x[0-9abcdefABCDEF]+)')
prog_context = re.compile('#define\s+RPR_CONTEXT_([A-Za-z0-9_]+)\s+(0x[0-9abcdefABCDEF]+)')
with open(args.input) as file:
    for line in file:
        txt = line.rstrip()
   
        # RPR_MATERIAL_INPUT_
        matches = prog_material_input.match(txt)
        if matches:
            define_name = matches[1]
            hex_code = matches[2]
            material_inputs.append(define_name)

        # RPR_MATERIAL_NODE_
        matches = prog_material_node.match(txt)
        if matches:
            define_name = matches[1]
            hex_code = matches[2]
            material_nodetypes.append(define_name)

        # RPR_CONTEXT_
        matches = prog_context.match(txt)
        if matches:
            define_name = matches[1]
            hex_code = matches[2]
            context_input_types.append(define_name)

    
with open(args.output, "w") as fout:
    fout.write("#pragma once\n");
    fout.write("\n")
    fout.write("#include <RadeonProRender.h>\n")
    fout.write("\n")
    fout.write("namespace rprf\n")
    fout.write("{\n")

    fout.write("\tenum class ContextInputType {\n") # enum ContextInputType
    for n in context_input_types:
        fout.write("\t\t{case}\t\t = RPR_CONTEXT_{define},\n".format(case=make_name_context(n), define=n))
    fout.write("\t};\n") # end class ContextInputType
    
    fout.write("\n")


    fout.write("\tenum class MaterialInputType {\n") # enum MaterialInputType
    for n in material_inputs:
        fout.write("\t\t{case}\t\t = RPR_MATERIAL_INPUT_{define},\n".format(case=make_name_input(n), define=n))
    fout.write("\t};\n") # end class MaterilInputType
    
    fout.write("\n")

    fout.write("\tenum class MaterialNodeType {\n") # enum MaterialNodeType
    for n in material_nodetypes:
        fout.write("\t\t{case}\t\t = RPR_MATERIAL_NODE_{define},\n".format(case=make_name_node(n), define=n))
    fout.write("\t};\n") # end class MaterilNodeType

    fout.write("} // namespace\n")

