This is Project1 for COMP30023 Computer Systems.
This is a muti-threading server using pthread, handling http request. 

Usage:
To run test:
bash test_sciprt.sh my_server [port_number]

To test othe files:
wget http://[ip_address]:[port_number]/[file_name]

To limit transfer rate:
wget --limit-rate=[amount] http://[ip_address]:[port_number]/[file_name]
For full infomation about wget, please see: https://www.gnu.org/software/wget/manual/wget.html

To run the server alone:
./my_server [port_number] [directory_of_file]


