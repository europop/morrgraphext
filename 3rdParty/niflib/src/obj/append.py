import os
import glob
import sys
import stat

current_dir = '.'
in_file = "*.cpp"
match = "\tNIFLIB_API static const Type TYPE;\n"
add_after = "\tNIFLIB_API static NiObject * Create();\n"
append = """
NiObject * {NAME}::Create() {
	return new {NAME};
}
"""

os.chdir( current_dir )
files = glob.glob( in_file )

for current_file in files:
    #print current_dir + os.sep + current_file

    #f = file(current_dir + os.sep + current_file, 'r')
    #lines = f.readlines()
    #f.close()
    
    #i = lines.index( match )
    #lines.insert( i + 1, add_after )

    #f = file(current_dir + os.sep + current_file, 'w')
    #f.writelines(lines)
    #f.close()

    f = file(current_dir + os.sep + current_file, 'a' )

    obj = current_file.split('.')
    obj = obj[0]

    print obj

    s = append.replace('{NAME}', obj)

    f.write(s)

    f.close()

    
    
