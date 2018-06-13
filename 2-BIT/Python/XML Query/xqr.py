#!/usr/bin/python
#-*- coding: UTF-8 -*-
#author: Martin Nizner
#FIT VUT IPP proj 2
import sys, re, string, os
import xml.dom.minidom as xml;

def write_check(attribute_value, relation_string, relation_number, lesser, greater, contains, equals, neg):
	if relation_number != '':
		try:
			number = float(attribute_value);
		except:
			number = '';
		if number != '':
			if lesser == 1:
				if not number < relation_number:
					if neg == 0:
						write = 0;
					else:
						write = 1;	
			elif greater == 1:
				if not number > relation_number:
					if neg == 0:
						write = 0;
					else:
						write = 1;	
			elif equals == 1:
				if not number == relation_number:
					if neg == 0:
						write = 0;
					else:
						write = 1;
		else:
			if neg == 0:
				write = 0;
			else:
				write = 1;						
	elif relation_string != '':
		string = attribute_value;	
		if lesser == 1:
			if not string < relation_string:
				if neg == 0:
					write = 0;
				else:
					write = 1;	
		elif greater == 1:
			if not string > relation_string:
				if neg == 0:
					write = 0;
				else:
					write = 1;	
		elif equals == 1:
			if not string == relation_string:
				if neg == 0:
					write = 0;
				else:
					write = 1;	
		elif contains == 1:
			if string.find(relation_string) == -1:
				if neg == 0:
					write = 0;
				else:
					write = 1;
	return write;

def getDataToParse(cond):
	if cond != '':
		try:
			relation_number = int(cond);
			if contains == 1:
				sys.stderr.write("Error with query13\n");
				sys.exit(80);
			return relation_number	
		except:
			relation_string = cond;
			error = 1;
			if relation_string.startswith('"') and relation_string.endswith('"'):
				error = 0;	
				relation_string = relation_string[1:-1];	
			if error ==1:
				sys.stderr.write("Error with query14\n");
				sys.exit(80);
				return relation_string;

if __name__ == '__main__':
	opts = ' ' + ' '.join(sys.argv[1:]);
	filein = sys.stdin;
	fileout = sys.stdout;
	head = 1;
	root_element = '';
	select_element = '';
	from_element ='';
	from_element_root = '';
	from_attribute = '';
	where_element ='';
	where_attribute = '';
	negation = 0;
	contains = 0;
	greater = 0;
	lesser = 0;
	equals = 0;
	relation_string = '';
	relation_number = '';
	limit_n = '';
	split_word = 0;
	split_contains = 0;
	xml_from = 0;
	opt = re.compile(r" --help");    #kontrola help parametru
	if opt.search(opts) is not None:
		if opt.sub('',opts) is not '':
			sys.stderr.write('Error too much arguments\n')
			sys.exit(1);
		print ("Skript pro vyhodnoceni zadaneho dotazu jazyka SQL\n");
		print ("Skript pracuje s nasledovnymi parametrami\n");
		print ("--help             zobrazi napovedu\n");
		print ("--input=filename   zadan vstupny soubor ve formatu XML\n");	
		print ("--output=filename  zadan vystupny soubor ve formatu XML\n");
		print ("--query='dotaz'    zadan dotaz v dotazovacim jazyce\n");
		print ("--qf=filename      dotaz v dotazovacim jazyce v externim testovem souboru\n");
		print ("-n 		   negeneruje XML hlavicku na vystup skriptu\n");
		print ("--root=element	   jmeno paroveho korenoveho elementu obalujici vysledky\n");
		sys.exit(0);
	opt = re.compile(r" --input=(\S+)");    #kontrola input parametru
	input = opt.search(opts)
	if input is not None:
		opts = opt.sub('',opts);			
	opt = re.compile(r" --output=(\S+)");	#kontrola output parametru
	output = opt.search(opts);
	if output is not None:
		opts = opt.sub('',opts);
	opt = re.compile(r" --qf=(\S+)");		#kontrola qf parametru
	qfile = opt.search(opts);
	if qfile is not None:
		opts = opt.sub('', opts);
	opt = re.compile(r" -n");               #kontrola nohead parametru
	if opt.search(opts) is not None:
		head = 0;	
		opts=opt.sub('',opts);
	opt = re.compile(r" --root=(\S+)");     #kontrola root parametru
	root = opt.search(opts);  			
	if root is not None:		
		opts = opt.sub('',opts);				
	opt = re.compile(r" --query=(.*)");     #kontrola query parametru
	query=opt.search(opts);
	if query is not None:                   #nastavovanie pomocnych premennych na zaklade aktivnych parametrov
		opts = opt.sub('', opts);
	if opts is not '':
		sys.stderr.write("Error with arguments1\n");
		sys.exit(1);	
	if query is not None:
		querytoparse=query.group(1);	
	if query is not None and qfile is not None:
		sys.stderr.write("Error with arguments2\n");
		sys.exit(1);
	if query is None and qfile is None:
		sys.stderr.write("Error with arguments3\n");
		sys.exit(1);	
	if qfile is not None:
		queryfile = qfile.group(1);		
		try:
			filequery=open(queryfile,"r");
		except:
			sys.stderr.write("Error with query file\n");
			sys.exit(80);	
			querytoparse = filequery.read();
			filequery.close();					
	if input is not None:
		inputfile = input.group(1);
		try:
			filein = open(inputfile,"r");
		except:
			sys.stderr.write("Error with input file\n");
			sys.exit(2);			
	if output is not None:
		outputfile = output.group(1);
		try:
			fileout = open(outputfile,"w");
		except:
			sys.stderr.write("Error with output file\n");
			sys.exit(3);	
	if root is not None:
		root_element = root.group(1);	 
	querytoparse=re.sub('\s+',' ',querytoparse).strip();
	querylist = querytoparse.split();
	n_state = 1;
	for word in querylist:
		if n_state == 1:
			if word != "SELECT":     #stav SELECT
				sys.stderr.write("Error with query0\n")
				sys.exit(80);		
			else:
				n_state = 2;
				continue;
		if n_state == 2:
			select_element = word;    #stav SELECT element
			if select_element.startswith("<") or select_element.endswith(">"):
				sys.stderr.write("Error with query1\n");
				sys.exit(80);
			n_state = 3;	
			continue;	 
		if n_state == 3:         #stav FROM
			if word != "FROM":
				sys.stderr.write("Error with query2\n");
				sys.exit(80);
			else:
				n_state=4;
				continue;	
		if n_state == 4:
			if word == "WHERE":  #stav WHERE/ROOT/FROM element/FROM element.attribute/FROM .attribute
				n_state = 6;
			elif word == "ROOT": 
				from_element_root = word;
				n_state = 5;
			else:
				if word.find('.') != -1:  
					el_atr = word.split('.');
					if len(el_atr) == 2:
						if word.startswith('.'):
							for string in el_atr:
								from_attribute = string;
							if from_attribute.startswith('<') or from_attribute.endswith('>'):
								sys.stderr.write("Error with query3\n");
								sys.exit(80);	
						else:		
							i = 1;
							for string in el_atr:
								if (i > 1):
									from_attribute = string;
									if from_attribute.startswith('<') or from_attribute.endswith('>'):
										sys.stderr.write("Error with query4\n");
										sys.exit(80);
								else:
									from_element = string;
									if from_element.startswith('<') or from_element.endswith('>'):
										sys.stderr.write("Error with query5\n");
										sys.exit(80);		
								i = i + 1;			
					else:
						sys.stderr.write("Error with querry6\n");
						sys.exit(80);				 			
				else:
					from_element = word;
					if from_element.startswith('<') or from_element.endswith('>'):
						sys.stderr.write("Error with query7\n");
						sys.exit(80);
				n_state = 5;
			continue;
		if n_state == 5:   #stav WHERE/LIMIT
			if word == "WHERE":
				n_state = 6;
			elif word == "LIMIT":
				n_state = 10;
			else:			
				sys.stderr.write("Error with quer8\n");
				sys.exit(80);
			continue;
		if n_state == 6:
			if word == "NOT":  #stav NOT/WHERE element/WHERE element.attribute/WHERE .attribute
				negation = negation + 1;
			elif word.find('.') != -1:
				el_atr = word.split('.');
				if len(el_atr) == 2:
					if word.startswith('.'):
						j = 1;
						for string in el_atr:
							where_attribute = string;
							if (j > 1):
								if where_attribute.find('<') != -1:
									lesser = 1;
									n_state = 8;
									split_word = '<';
								if where_attribute.find('>') != -1:
									greater = 1;
									split_word = '>';
									n_state = 8;
								if where_attribute.find('=') != -1:
									split_word = '=';
									equals = 1;
									n_state = 8;
								if split_word != 0:	
									parse = where_attribute.split(split_word);
									if len(parse) == 2:
										z = 0;
										for cond in parse:
											if (z == 0):
												where_attribute = cond;
											else:
													try:
														relation_number=int (getDataToParse(cond));
													except:
														relation_string = getDataToParse(cond);
													n_state = 9;		
											z = z + 1;	
									else:
										sys.stderr.write("Error with query11\n");
										sys.exit(80);				
							j = j+1;
						if where_attribute.startswith('<') or where_attribute.endswith('>'):
							sys.stderr.write("Error with query12\n");
							sys.exit(80);		
					else:		
						i = 1;
						for string in el_atr:
							if (i > 1):
								where_attribute = string;
								if where_attribute.find('<') != -1:
									lesser = 1;
									n_state = 8;
									split_word = '<';
								if where_attribute.find('>') != -1:
									greater = 1;
									split_word = '>';
									n_state = 8;
								if where_attribute.find('=') != -1:
									split_word = '=';
									equals = 1;
									n_state = 8;
								if split_word != 0:	
									parse = where_attribute.split(split_word);
									if len(parse) == 2:
										z = 0;
										for cond in parse:
											if (z == 0):
												where_attribute = cond;
											else:
												try:
													relation_number= int (getDataToParse(cond));
												except:
													relation_string = getDataToParse(cond);
												n_state = 9;		
											z = z + 1;	
									else:
										sys.stderr.write("Error with query15\n");
										sys.exit(80);				
								if where_attribute.startswith('<') or where_attribute.endswith('>'):
									sys.stderr.write("Error with query16\n");
									sys.exit(80);
							else:
								where_element = string;
								if where_element.startswith('<') or where_element.endswith('>'):
									sys.stderr.write("Error with query17\n");
									sys.exit(80);		
							i = i + 1;			
				else:
					sys.stderr.write("Error with querry18\n");
					sys.exit(80);
				if n_state != 8 and n_state != 9:
					n_state = 7;	
			else:
				where_element = word;
				if where_element.find('<') != -1:
					lesser = 1;
					n_state = 8;
					split_word = '<';
				if where_element.find('>') != -1:
					greater = 1;
					split_word = '>';
					n_state = 8;
				if where_element.find('=') != -1:
					split_word = '=';
					equals = 1;
					n_state = 8;
				if split_word != 0:	
					parse = where_element.split(split_word);
					if len(parse) == 2:
						z = 0;
						for cond in parse:
							if (z == 0):
								where_element = cond;
							else:
								try:
									relation_number= int (getDataToParse(cond));
								except:
									relation_string = getDataToParse(cond);
								n_state = 9;	
							z = z + 1;	
					else:
						sys.stderr.write("Error with query21\n");
						sys.exit(80);			
				if where_element.startswith('<') or where_element.endswith('>'):
					sys.stderr.write("Error with query22\n");
					sys.exit(80);
				if n_state != 8 and n_state != 9:
					n_state = 7;			
			continue;		
		if n_state == 7:  #stav CONTAINS/</>/=
			if word == "CONTAINS":
				contains = 1;
			elif word == "<":
				lesser = 1;
			elif word == ">":
				greater = 1;
			elif word == "=":
				equals = 1;
			elif word.startswith('<'):
				split_contains = '<';
				lesser = 1;
			elif word.startswith('>'):
				split_contains = '>';
				greater = 1;
			elif word.startswith('='):
				split_contains = '=';
				equals = 1;
			elif word.startswith("CONTAINS"):
				split_contains = "CONTAINS";
				contains = 1;				 		
			else:
				sys.stderr.write("Error with query23");
				sys.exit(80);
			if split_contains != 0:
				parse = word.split(split_contains);
				if len(parse) == 2:
					z = 0;
					for cond in parse:
						if z != 0:
							try:
								relation_number= int (getDataToParse(cond));
							except:
								relation_string = getDataToParse(cond);
							n_state = 9;
						z = z+1;
				else:
					sys.stderr.write("Error with query26\n");
					sys.exit(80);
				n_state = 9;
			if n_state != 9:									
				n_state = 8;
			continue;
		if n_state == 8:
			try:   #stav celociselny literal/retezec
				relation_number = int(word);
				if contains == 1:
					sys.stderr.write("Error with query27\n");
					sys.exit(80);
			except:
				relation_string = word;
				error = 1;
				if relation_string.startswith('"') and relation_string.endswith('"'):
					error = 0;
				relation_string = relation_string[1:-1];	
				if error == 1:
					sys.stderr.write("Error with query28\n");
					sys.exit(80);	

			n_state = 9;
			continue;
		if n_state == 9:    #stav LIMIT
			if word == "LIMIT":
				n_state = 10;
			else:
				sys.stderr.write("Error with query29\n");
				sys.exit(80);
			continue;				
		if n_state == 10:  #stav LIMIT n
			try:
				limit_n = int(word);
			except:
				sys.stderr.write("Error with query30\n");
				sys.exit(80);
			if limit_n < 0:
				sys.stderr.write("Error with query\n");
				sys.exit(80);		
			n_state = 11;
			continue;
		if n_state == 11:
			sys.stderr.write("Error with query31\n");
			sys.exit(80);
	if n_state == 1 or n_state == 2 or n_state == 3 or n_state == 6 or n_state == 7 or n_state == 8:
		sys.stderr.write("Error with query32\n");
		sys.exit(80);
		

	try:
		xml_in = xml.parse(filein);    #nacitanie vstupneho XML suboru
	except:
		sys.stderr.write("Error with input\n");
		sys.exit(4);
	if head == 1:
		fileout.write('<?xml version="1.0" encoding="UTF-8"?>');  #generovanie hlavicky vystupneho XML suboru
	if root_element != '':
		fileout.write("<" + root_element + ">");	
	if from_element_root != '':
		xml_from = xml_in.documentElement;
	elif from_element!='' and from_attribute == '':
		xml_from = xml_in.getElementsByTagName(from_element)[0];
	elif from_element != '' and from_attribute != '':
		xml_from1 = xml_in.getElementsByTagName(from_element);
		for element in xml_from1:
			if element.hasAttribute(from_attribute):
				xml_from = element;
				break;		
	elif from_element == '' and from_attribute != '':
		xml_from1 = xml_in.documentElement;
		xml_from2 = xml_from1.getElementsByTagName("*");
		for node in xml_from2:
			if node.hasAttribute(from_attribute):
				xml_from = node;
				break;														
	if xml_from != 0:
		if select_element != xml_from.tagName:
			xml_select = xml_from.getElementsByTagName(select_element);
	else:
		xml_select = xml_in.getElementsByTagName(from_element);	
	if (negation == 0 or (negation % 2) == 0):
		neg = 0;
	else:
		neg = 1;
	write = 0;	
	limit = 0;		
	for node in xml_select:
		if neg == 0:
			write = 1;
		else:
			write = 0;		
		if where_attribute == '' and where_element != '':
			if where_element == node.tagName:
				if node.getElementsByTagName("*").length != 0:
					sys.stderr.write("Error with xml");
					sys.exit(4);
				if node.hasAttributes():
					sys.stderr.write("Error with xml");
					sys.exit(4);	
				attribute = node.firstChild;
				if not attribute.hasChildNodes():
					attribute_value = attribute.data;
					write = write_check(attribute_value, relation_string, relation_number, lesser, greater, contains, equals, neg)
			else:									
				node_element = node.getElementsByTagName(where_element);
				if node_element.length == 0:
					write = 0;
				set_write = 0;			
				for node_2 in node_element:
					if node_2.getElementsByTagName("*").length != 0:
						sys.stderr.write("Error with xml");
						sys.exit(4);
					if node_2.hasAttributes():
						sys.stderr.write("Error with xml");
						sys.exit(4);	
					attribute = node_2.firstChild;
					if not attribute.hasChildNodes():
						attribute_value = attribute.data;
						write = write_check(attribute_value, relation_string, relation_number, lesser, greater, contains, equals, neg);
						if neg == 0:				
							if write == 1:
								set_write = 1;
						else:
							if write == 0:
								set_write = 1;
						if neg == 0:				
							if set_write == 1:
								write = 1;
						else:
							if set_write == 1:
								write = 0;												
					else:
						sys.stderr.write("Error with xml\n");
						sys.exit(4);								
		elif where_element != '' and where_attribute != '':
			if node.tagName == where_element:
				if node.hasAttribute(where_attribute):
					attribute_value = node.getAttribute(where_attribute);
					write = write_check(attribute_value, relation_string, relation_number, lesser, greater, contains, equals, neg);	
				else:
					write = 0;								
			else:
				node_element = node.getElementsByTagName(where_element);
				if node.getElementsByTagName(where_element).length == 0:
					write = 0;
				set_write = 0;
				for node_2 in node_element:
					if node_2.hasAttribute(where_attribute):
						attribute_value=node_2.getAttribute(where_attribute);
						write = write_check(attribute_value, relation_string, relation_number, lesser, greater, contains, equals, neg)
						if neg == 0:				
							if write == 1:
								set_write = 1;
						else:
							if write == 0:
								set_write = 1;		
						if set_write == 1:
							if neg == 0:
								write = 1;
							else:
								write = 0;
					else:
						write = 0;
		elif where_element == '' and where_attribute != '':
			if node.hasAttribute(where_attribute):
				attribute_value = node.getAttribute(where_attribute);
				write = write_check(attribute_value, relation_string, relation_number, lesser, greater, contains, equals, neg)
				if (relation_number == '' and relation_string == ''):
					if neg == 0:
						write = 0;
					else:
						write = 1;	
			else:
				write = 0;														
		if write != 0:
			if limit_n != '' and limit_n == limit:
				break; 
			fileout.write(node.toxml());	
			limit = limit + 1;
	if root_element != '':
		fileout.write("</" + root_element + ">");	
	sys.exit(0);			
					

						



															
					





							

						
