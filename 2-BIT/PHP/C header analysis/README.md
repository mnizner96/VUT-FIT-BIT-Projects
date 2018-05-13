# Description
This project is my implementation of C header analysis in PHP programming language. The goal is search for header files of C programming language in the directory strcuture and then analyze their content.
The result of the analyze is written to the XML document. My script is using regular expressions, functions for recursively transit the directory structure and utilities for creating the XML documents.

# Usage
--help  prints help <br>
--input=file/dir  input file or directory with C source code <br>
--output=filename output file in XML format with UTF-8 encoding <br>
--prety-xml=k number of k spaces at the begining of the new line <br>
--no-inline script skips the inline functions <br>
--max-par=n script skips the functions that have more than n parameters <br>
--no-duplicates script skips all functions that have same name as first function that was found with this name <br>
--remove-whitespace script temoves all excess spaces

# Example
python5.6 cha.php --input=test.h --output=out.xml
