import htmlmin
import os
from slimit import minify
dir = os.path.dirname(os.path.realpath(__file__))
readfile = os.path.join(dir, "index.html")
writefile = os.path.join(dir, "data", "index.html")
htmlfile= open(readfile, "r")
text = htmlfile.read()
htmlfile.close()
print(len(text)//1024)
#out = minify(text, mangle=True)
out = (htmlmin.minify(text, remove_comments=True, remove_optional_attribute_quotes=True, reduce_boolean_attributes=True, remove_empty_space=True))
print(len(out)//1024)
writefile = open(writefile, "w")
writefile.write(out)
writefile.close()


