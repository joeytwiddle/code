 #!/bin/csh
 ftp -n remote.site << !
 user joe blow
 binary
 put newfile
 quit
 !
