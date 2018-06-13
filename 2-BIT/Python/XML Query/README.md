# Description
This project is my implementation of XML Query in Python programming language. The goal is filter out the input XML file. The filtration is based on specified input query that is similarly to SELECT command of the SQL programming language.
Filtered content is saved to the XML output file. My script is using regular expressions and utilities for access to the XML data.
# Usage
<strong>--help</strong> prints help <br>
<strong>--input=filename</strong> input file in XML format<br>
<strong>--output=filename</strong>output file in XML format<br>
<strong>--query='dotaz'</strong> input query in SQL language<br>
<strong>--qf=filename</strong> input query is saved in the <strong>filename</strong><br>
<strong>-n</strong> script doesn't generate XML header<br>
<strong>--root=element</strong> the root element that envelops the results<br>
# Example
python3.6 xqr.py --input=in.xml --output=out.xml --qf=SELECT book FROM library
