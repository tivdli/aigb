import htmlmin
import os

dir = os.path.dirname(os.path.realpath(__file__))
readfile = os.path.join(dir, "index.html")
writefile = os.path.join(dir, "site.txt")
htmlfile= open(readfile, "r")
text = htmlfile.read()
htmlfile.close()
print(len(text)//1024)
out = (htmlmin.minify(text, remove_comments=True, remove_optional_attribute_quotes=True, reduce_boolean_attributes=True, remove_empty_space=True))
print(len(out)//1024)
writefile = open(writefile, "w")
writefile.write(out)
writefile.close()


