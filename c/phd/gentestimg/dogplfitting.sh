cat gpldo.txt fitgnuplot2.txt > gplsolve.txt
cat gplsolve.txt | gnuplot > gplans.txt 2>&1
grep '^v' gplans.txt | grep '=' | tail -1 | after '= ' | before ' ' > v.txt
grep '^w' gplans.txt | grep '=' | tail -1 | after '= ' | before ' ' > w.txt
grep '^WSSR' gplans.txt | tail -1 | between ':' | before 'delta' | sed 's/ //g' > wssr.txt
# cat wssr.txt
