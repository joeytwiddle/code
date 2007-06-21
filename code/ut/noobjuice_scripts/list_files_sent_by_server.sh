rotate files_sent_by_server.txt
jzcat ./ut-server/Logs/* | grep "Sending" | betweenthe "'" | afterfirst "^\.\./" | grep -v "^$" | grep -v "^Sending" | removeduplicatelines -adj | removeduplicatelines | tee files_sent_by_server.txt
