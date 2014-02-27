#make 2>&1 | grep spark | cut -d "[" -f2 | cut -f1 -d ] | grep -v spark | tee ploum.txt
cat ploum.txt | grep spark | cut -d "[" -f2 | cut -f1 -d ] | grep -v spark | tee ploum2.txt
for a in `cat ploum2.txt`; do addr2line $a -e spark; done
