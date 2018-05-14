# Description
This program implements client application in C programming language that is querying DNS system and realizing the translation of domain names and IP addresses.

# Usage
<strong> -h </strong> prints help <br>
<strong> -s server </strong> required parameter, DNS server(IPv4 address) that will be used for querying <br>
<strong> -T timeout </strong> timeout for query in seconds <br>
<strong> -t type </strong> type of record (A, AAAA), default value is A <br>
<strong> -i </strong> iterative type of resolution<br>
<strong> name </strong> Domain name or IP address(PTR record) to translate 
# Convention of usage
./ipk-lookup [-h] <br>
./ipk-lookup -s server [-T timeout] [-t type] [-i] name <br>
# Examples
  ./ipk-lookup -s 8.8.8.8 www.fit.vutbr.cz <br>
  ./ipk-lookup -s 8.8.8.8 -t CNAME www4.fit.vutbr.cz. <br>
  ./ipk-lookup -s 8.8.8.8 -t PTR 2001:67c:1220:8b0::93e5:b013 <br>
  ./ipk-lookup -s 8.8.8.8 -t PTR -i 147.229.13.238 <br>
