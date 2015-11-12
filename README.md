# AIM OF THE PROJECT
Forecast visbility using at IGI airport

imd.sql.txt is the backup of the database
decoding contains program to decode input
Input-Output stores all initial, intermediate and final input/output files
sql contains all sql related source code files

# Setup instructions

1. Install these software packages<br>
`(mysql-devel` or `libmysqlclient-devel),  (mysql` or `mariadb) mysql-server cmake`

2. Install soci library. Get soci library from [here](http://soci.sourceforge.net/)<br>
```
cmake . -DCMAKE_INSTALL_PREFIX=/usr
```<br>
```
make clean
```<br>
```
make
```<br>
```
sudo make install
```

2. Start sql server<br>
`sudo service maraidb start` or `sudo service mysql start`

3. If root account password is not set, then run <br>
```
mysql -u root 
```
<br>
```
SET PASSWORD FOR 'root'@'localhost' = PASSWORD('root');
```
<br>
```
flush privileges; 
```
<br>
```
quit
```
<br>


4. Push data into database<br>
```
sudo mysql < imd.sql.txt
```
