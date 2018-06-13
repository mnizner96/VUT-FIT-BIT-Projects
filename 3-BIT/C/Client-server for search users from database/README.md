# Description
This program implements client and also server application for search users in database. These applications communicate between them by using BSD sockets and TCP/IP protocol.

# Usage
## ipk-server
<strong> -p port </strong> port that will be used for server running
### Example 
./ipk-server -p <port>

## ipk-client
<strong> -h host </strong> IP address or fully qualified DNS name<br>
<strong> -p port </strong> destination port<br>
<strong> -n </strong> prints User ID Info <br>
<strong> -f </strong> prints home directory of user <br>
<strong> -l prefix</strong> prints list of all users or list based on entered prefix 

### Example

./ipk-client -h <host> -p <port> [-n|-f|-l] <login>
