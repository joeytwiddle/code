ftp -n ftp.scene.org << !
user anonymous nobody@nowhere.com
cd /pub 
binary
prompt
mget *
quit
!
