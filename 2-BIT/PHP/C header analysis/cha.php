<?php
define("ER_OK",0);
define("ER_PARAMS",1);
define("ER_FOPEN",2);
define("ER_OUTPUT",3);
define("ER_FORMAT",4);

$longopts = array(
	"help::",
	"input:",
	"output:",
	"pretty-xml::",
	"no-inline",
	"max-par:",
	"no-duplicates",
	"remove-whitespace",
	);

$options = getopt(NULL, $longopts);

$dir=getcwd();
$files=array();
$output=0;                //pomocne premenne pre prepinace
$pretty = -1;
$functions_all=array();
$rem_spaces = 0;
$max_par = -1;
$onlymax = 0;
$no_inline = 0;
$no_duplicates = 0;
if ($options === false)
{
	exit(ER_PARAMS);
}
if (count($options) < (count($argv)-1))
{
	exit(ER_PARAMS);
}
foreach (array_keys($options) as $argument)  //kontrola parametrov
{
	if (count($options[$argument]) > 1)
	{
		exit(ER_PARAMS);
	}
	switch($argument)    //nastavovanie pomocnych premennych podla aktivity prepinacov
	{
		case "help":
			if (($options[$argument]) !== false)
			{
				exit(ER_PARAMS);
			}
			if (count($options) != 1)
				exit (ER_PARAMS);
			help();
			return (ER_OK);
			break;
		case "input":
			$dir = $options[$argument];
			break;
		case "output":
			$output = $options[$argument];
			break;
		case "pretty-xml":
			if (($options[$argument]) !== false)
				$pretty = (int) $options[$argument];
			else
				$pretty = 4;
			break;
		case "remove-whitespace":
			$rem_spaces = 1;
			break;
	    case "max-par":
	        $max_par = $options[$argument];
	        if ($max_par < 0)
	        {
	        	exit (ER_PARAMS);
	        }
	        $onlymax = 1;
	        break;
	    case "no-inline":
	    	$no_inline = 1;
	    	break;    
	    case "no-duplicates":
	    	$no_duplicates = 1;
	    	break; 	
		default:
			exit (ER_PARAMS);
	}

}
if (is_dir($dir))
{
		
	$path = realpath($dir);
	$Directory = new RecursiveDirectoryIterator($path);
	$Iterator = new RecursiveIteratorIterator($Directory);
	$Regex = new RegexIterator($Iterator, '/^.+\.h$/i', RecursiveRegexIterator::GET_MATCH);
	foreach ($Regex as $name => $object)  //rekurzivny priechod adresarmi
	{
		array_push($files, $name);
	}
	if ($dir === getcwd())
    {
        $fdir="./";
    }
    else 
    {
        $fdir=$dir;
        if (substr($fdir, -1) != "/")
        {
        	$fdir .= "/";
        }
        if (substr($fdir, 0, 1) == "/")
		{
		   	$char = ".";
            $char .= $fdir;
            $fdir = $char;
        }	
	}
}
else if (is_file($dir))
{
	array_push($files, $dir);
	$fdir = "";
}
else 
{
	$path = pathinfo($dir);
	if ($path['extension'] != NULL)
	{
		if ($path['extension'] !== "h")  //kontrola spravne zadanej pripony
        {   
            fwrite(STDERR,"Error formatu\n");
			exit (ER_FORMAT);
		}
	}
	else
	{
		write(STDERR,"Error neopravneny pristup do slozky\n");
		exit("ER_FOPEN");
	}
}
		//print_r($files);
foreach ($files as $file)
{                                          
	$found=file_get_contents($file);                     
	$found = preg_replace("/\/\*.*?\*\//s", "", $found);   //odstranovanie nadbytocnych informacii(komentare, makra) 
 	$found = preg_replace("/\/\/.*?\n/", "", $found); 
 	$found = preg_replace("/#.*?\n/", "", $found);  
	$found = preg_replace("/\".*?\"/", "", $found);

	//ziskanie funkcii s hlavickoveho suboru
	$matches=preg_match_all("/[a-zA-Z_][[:alpha:][:space:]]*?[[:graph:]]+?[[:space:]]+?[[:graph:]]+?[[:space:]]*?\([[:graph:][:space:]]*?\)[[:space:]]*?[{,;]/", $found, $functions); 
	if ($matches !== 0)
	{
		foreach($functions as $func)
		{
			foreach ($func as $item)
			{
				if ($onlymax == 1)  //kontrola maximalneho poctu parametrov
				{
				    preg_match_all("/[[:alnum:][:space:]_*\[\]]+[),]/", $item,$paramtest);
				    $cnt = count($paramtest[0]);
				    if ($cnt > $max_par)
				    {
				      	$key=array_search($item, $func);
				      	unset($func[$key]);
				    }
				}
				if ($no_inline == 1)  //kontrola inline funkcii
				{
				    $inline_func = preg_match_all("/inline/", $item);
				    if ($inline_func !== 0)
				    {
				        $key=array_search($item, $func);
				      	unset($func[$key]);
				    }
				}

			} 
			$functions_all[$file]=$func;
				    
        }
    }  
}

$xml = new DOMDocument('1.0', 'UTF-8');     //vytvorenie XML dokumentu
$xml->preserveWhiteSpace = false; 
$xml->formatOutput = true; 
$xml_functions = $xml->createElement("functions");
$xml->appendChild($xml_functions);
$xml_functions->setAttribute( "dir", $fdir);
foreach ($functions_all as $f=>$value)
{
	$dupl = array();
	if (is_file($dir))
		$h_file = $dir;
	else
		$h_file=substr($f, strlen($path)-strlen($f)+1);
	if ((substr($h_file, 0, 1) == "/") && (!(is_file($h_file))))
	{
		$char = ".";
        $char .= $h_file;
        $h_file = $char;
    }
    foreach($functions_all[$f] as $function)
    {
		//ziskanie nazvu funkcie
        preg_match("/[a-zA-Z_][[:alnum:][:space:]]*?[[:graph:]]+?[[:space:]]*?([[:alnum:]_]+?)[[:space:]]*?\(/", $function, $function_name);
        if ($no_duplicates != 0)   //kontrola duplikatov
        {
            if (in_array($function_name[1], $dupl))
              	continue;
        }
        array_push($dupl, $function_name[1]);
	    $xml_function = $xml->createElement("function");
	    $xml_function->setAttribute("file", $h_file);
		$xml_function->setAttribute("name", $function_name[1]);
		$var_args = preg_match("/\,[[:space:]]*?\.\.\.[[:space:]]*?\)/", $function);
	    if ($var_args !== 0)   //kontrola premenneho poctu parametrov
			$xml_function->setAttribute("varargs", "yes");
		else
			$xml_function->setAttribute("varargs", "no");
		//ziskanie navratoveho typu funkcie
		preg_match("/([a-zA-Z_][[:alnum:][:space:]]*?[[:graph:]]+?)[[:space:]]*?([[:alnum:]_]+?)[[:space:]]*?\(/", $function, $function_rettype);
		$function_rettype[1] = preg_replace('/\s/', ' ', $function_rettype[1]); 
		if ($rem_spaces == 0) //kontrola parametru pre odstranenie nadbytocnych medzier
		{
			$xml_function->setAttribute("rettype", $function_rettype[1]);
		}
		else
		{
			$function_rettype[1] = preg_replace('/\s+/', ' ',$function_rettype[1]); //odstranenie nadbytocnych medzier
			$function_rettype[1]=str_replace(" *","*",$function_rettype[1]);
			$xml_function->setAttribute("rettype", $function_rettype[1]);
		}
		//ziskanie parametrov funkcie
        preg_match_all("/[[:alnum:][:space:]_*\[\]]+[),]/", $function,$params);
        $count = count($params[0]);
        if ($count != 0)
        {
         	$number = 1;
         	foreach ($params[0] as $param)
         	{
         		//odstranenie nadbytocnych bielych znakov s parametrov
         		$param = preg_replace("/^[[:space:]]/","",$param);
         		$param = preg_replace("/[[:space:]]*[[:alnum:]_]*[[:space:]]*[),]/","",$param);
         		$param = preg_replace('/\s/', ' ', $param);
         		if ($param == NULL)
         			continue;
         		$xml_function_param=$xml->createElement("param");
		    	$xml_function->appendChild($xml_function_param);
         		$xml_function_param->setAttribute("number", $number);
         		if ($rem_spaces == 1)
         		{
              		$param = preg_replace('/\s+/', ' ',$param);
			  		$param=str_replace(" * ","*",$param);
         		}
         		$xml_function_param->setAttribute("type", $param);
         		$number = $number + 1;


         	}
		}	
		$xml_functions->appendChild($xml_function);
	}
}
if ($pretty == -1)  //kontrola parametru pretty-xml
{
    $xml->preserveWhiteSpace = false; 
    $xml->formatOutput = false; 
    $XmlOut=preg_replace("/\n/", "", $xml->saveXML());
}
else
{
    $xml->preserveWhiteSpace = true;
    $XmlOut=$xml->saveXML();
    $XmlOut=preg_replace_callback('/^( +)</m', function($a) use ($pretty) { 
    return str_repeat(' ',intval(strlen($a[1]) / 2) * $pretty).'<';  }, $XmlOut);
}
if ($output === 0) //kontrola output parametru
{
	echo $XmlOut;
}
else 
{
	if (@file_put_contents($output, $XmlOut) === FALSE)
	{
		exit (ER_OUTPUT);
	}
}
function help()
{
	fwrite (STDOUT, "Napoveda pre C Header Analysis\n");
	fwrite (STDOUT, "Tento skript pracuje s temito parametry:\n");
	fwrite (STDOUT, "--help   zobrazi napovedu\n");
	fwrite (STDOUT, "\n--input=fileordir    zadany vstupni soubor nebo adresar se zdrojovym kodem v jazyce C\n");
	fwrite (STDOUT, "\n--output=filename    Zadany vystupni soubor ve formatu XML v kodovani UTF-8\n");
	fwrite (STDOUT, "\n--pretty-xml=k       Skript zformatuje vysledny XML dokument tak, ze kazde nove zanoreni bude odsazeno o k mezer oproti predchozimu\n");
	fwrite (STDOUT, "                     a XML hlavicka bude od korenoveho elementu oddelena znakem noveho radku\n");
	fwrite (STDOUT, "\n--no-inline          Skript preskoci funkce deklarovane se specifikatorem inline\n");
	fwrite (STDOUT, "\n--max-par=n          Skript bude brat v uvahu pouze funkce, ktere maji n ci mene parametru\n");
	fwrite (STDOUT, "\n--no-duplicates      Pokud se v souboru vyskytne vice funkcii se stejnym jmenem, tak se do vysledneho XML souboru ulozi pouze prvni z nich\n");
	fwrite (STDOUT, "\n--remove-whitespace  Pri pouziti tohto parametru skript odstrani z obsahu atributu rettype a type vsechny prebytecne medzery\n");
}
return (ER_OK);

?>
