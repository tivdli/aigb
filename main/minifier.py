import htmlmin
import os
from jsmin import jsmin
dir = os.path.dirname(os.path.realpath(__file__))
readfileHTML = os.path.join(dir, "index.html")
writefileHTML = os.path.join(dir, "data", "index.html")
htmlfile= open(readfileHTML, "r")
text = htmlfile.read()
htmlfile.close()
out = (htmlmin.minify(text, remove_comments=True, remove_optional_attribute_quotes=True, reduce_boolean_attributes=True, remove_empty_space=True))
print(f"compressed HTML from {(len(text)/1024):.2f} kB to {(len(out)/1024):.2f} kB")
writefileHTML = open(writefileHTML, "w")
writefileHTML.write(out)
writefileHTML.close()

readfileJS = os.path.join(dir, "script.js")
writefileJS = os.path.join(dir, "data", "script.js")
JSfile= open(readfileJS, "r")
text = JSfile.read()
JSfile.close()
out = jsmin(text)
print(f"compressed JS from {(len(text)/1024):.2f} kB to {(len(out)/1024):.2f} kB")
writefileJS = open(writefileJS, "w")
writefileJS.write(out)
writefileJS.close()

