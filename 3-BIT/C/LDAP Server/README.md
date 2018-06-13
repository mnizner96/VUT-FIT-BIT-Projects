# Description
This program implements simple LDAP server that is recieving and analyzing messages from client and then sending answer to the client.
Program is implemented by using non-blocking sockets and TCP/IP protocol. Server is parallel. 

# Usage
<strong> -p port </strong> server port (default 389) <br>
<strong> -f file </strong> path to the <strong> file </strong> in CSV format

# Convention of usage
./myldap {-p port} -f file

# Examples

## My LDAP server application <br>
./myldap -p 5200 -f list.csv

## LDAP Client
ldapsearch -h localhost -p 5200 -x uid="xnizne00" <br>
ldapsearch -h localhost -p 5200 -x uid="*n*z" <br>
ldapsearch -h localhost -p 5200 -x cn="*i*M" 
