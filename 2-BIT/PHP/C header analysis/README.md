# Description
This project is my implementation of C header analysis in PHP programming language. The goal is search for header files of C programming language in the directory structure and then analyze their content.
The result of the analyze is written to the XML document. My script is using regular expressions, functions for recursively transit the directory structure and utilities for creating the XML documents.

# Usage
<strong> --help </strong>  prints help <br>
<strong>--input=file/dir </strong>  input file or directory with C source code <br>
<strong>--output=filename</strong> output file in XML format with UTF-8 encoding <br>
<strong>--prety-xml=k</strong> number of <strong>k</strong> spaces at the begining of the new line <br>
<strong>--no-inline</strong> script skips the inline functions <br>
<strong>--max-par=n</strong> script skips the functions that have more than <strong>n</strong> parameters <br>
<strong>--no-duplicates</strong> script skips all functions that have same name as first function that was found with this name <br>
<strong>--remove-whitespace</strong> script temoves all excess spaces

# Example
php5.6 cha.php --input=test.h --output=out.xml
