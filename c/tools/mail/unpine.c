#include <joeylib.h>

FILE *instream;
bool eofreached=false;
FILE *outstream;

#define BUFSZ 40000
char currentline[BUFSZ];
int len;

#define MAXMAILLEN 160000000
char currentmail[MAXMAILLEN];
int maillen;

int skipLine(char *mail, int i) {
	while (mail[i]!='\n')
		i++;
	return i+1;
}

bool readline() {

	len=0;
	int c;
	while ( ( c = fgetc(instream) ) != EOF && (char)c != '\n' ) {
		currentline[len]=c;
		len++;
		if (len>=BUFSZ) {
			fprintf(stderr,"Error line buffer exceeded.\n");
			exit(1);
		}
	}
	currentline[len]='\0';

	// fprintf(stderr,"Read line >%10s<\n",currentline);

	if (c==EOF)
		eofreached=true;

	return true;

}

bool copylinetomail() {

	if (maillen+len > MAXMAILLEN) {
		fprintf(stderr,"Error mail buffer exceeded.\n");
		maillen=maillen-1234;
		maillen+=len+1;
		exit(1);
		return false;
	}	else {
		strcpy(&(currentmail[maillen]),currentline);
		maillen+=len;
		currentmail[maillen++]='\n';
		currentmail[maillen]='\0';
		return true;
	}

}

int contentStart;

String getHeader(String h) {
	h=Sconc(h,": ");
	for (int i=0;i<contentStart;i=skipLine(currentmail,i)) {
		if (Sstarts(&currentmail[i],h)) {
			free(h);
			// printf("Found mid line: >%s<\n",&currentmail[i]);
			// printf("Found mid line: >%i<\n",Sinstr(currentmail,"\n",i+1)-i);
			// String mid = Smid(currentmail,i+1,Sinstr(currentmail,"\n",i+1)-i);
			// String id = Safter(mid,": ");
			String id = Smid(currentmail,i+Slen(h)+1,Sinstr(currentmail,"\n",i+1)-i-Slen(h)-1);
			// free(mid);
			return id;
		}
	}
	free(h);
	return NULL;
}

String readmail() {

	maillen=0;
	int contentLen;

	// Find size of content
	// printf(">> %s\n",currentline);
	while (!Sstarts(currentline,"Content-Length: ")) {
		copylinetomail();
		readline();
	}

	String p=Sfrom(currentline,17);
	sscanf(p,"%i",&contentLen);
	// printf("Content-Length: %i\n",contentLen);
	// printf("*** %s\n*** %s\n*** %i\n",currentline,p,contentLen);

	// Find beginning of content
	// n=n+1;
	while (len>0) {
		copylinetomail();
		readline();
	}

	copylinetomail();
	contentStart=maillen+1;
	readline();

	// bool lastempty=true;
	// // Read through content
	// while (maillen<contentStart+contentLen && !(lastempty && Sstarts(currentline,"From - "))) {
		// // printf("%i %s\n",len,currentline);
		// // contentLen=contentLen-len-1;
		// copylinetomail();
		// readline();
		// lastempty=(len==0);
	// }

	// Read through content
	// while (maillen<contentStart+contentLen && !Sstarts(currentline,"From - ")) {
	while (maillen<contentStart+contentLen) {
		// printf("%i %s\n",len,currentline);
		// contentLen=contentLen-len-1;
		copylinetomail();
		readline();
	}

	// Find start of next
	bool snn=false;
	while (!Sstarts(currentline,"From - ")) {
		// printf(".");
		// snn=true;
		copylinetomail();
		readline();
	}
	if (snn)
		printf("\n");

	if (contentStart+contentLen!=maillen) {
		printf("Content-Length: %i\n",contentLen);
		printf("  Found length: %i\n",maillen-contentStart);
		// printf("  Read total:   %i\n",maillen);
		printf(" How much more: %i\n",maillen-contentStart-contentLen);
	} else {
		// printf("Happy mail: %i\n",maillen);
	}

	String messageID=getHeader("Message-id");
	if (messageID == NULL)
		messageID=getHeader("Message-ID");
	if (messageID == NULL)
		messageID=getHeader("message-id");
	if (messageID == NULL)
		messageID=getHeader("Message-Id");
	if (messageID == NULL)
		messageID=getHeader("message-ID");
	// if (messageID != NULL) {
		// printf("Message-id: %s\n",messageID);
	// }

	return messageID;

}

void writemail() {
	fputs(currentmail,outstream);
}

void main(int argc,String *argv) {

	ArgParser a=ArgParser(argc,argv);
	boolean writesepmails=a.argexists("-sep","write seperate files");
	boolean writenodups=a.argexists("-nodups","write new file excluding duplicates");
	bool writeToOneFile = ( writenodups );
	String fname=a.arg("filename of mailbox");
	String ofname;
	if (writeToOneFile)
		ofname=a.arg("output filename for new mailbox");
	a.done();

	// ls=readlinesfromfile(fname);

	// String destdir=Sformat("%s.ems",fname);
	// system(Sformat("mkdir %s",destdir));
	// system(Sformat("rm %s/*",destdir));

	instream=fopen(fname,"r");
	if (writeToOneFile)
		outstream=fopen(ofname,"w");

	readline();
	int i=1;
	String lastMessageID=NULL;
	int lastMailLen;
	while (true) {
		String messageID=readmail();
		if (writeToOneFile) {
			bool ok=true;
			if (lastMessageID!=NULL && messageID!=NULL) {
				if (writenodups && Seq(messageID,lastMessageID)) {
					ok=false;
					printf("Skipping duplicate message %i %i.\n",lastMailLen,maillen);
				}
			}
			if (ok)
				writemail();
		}
		if (lastMessageID!=NULL)
			free(lastMessageID);
		lastMessageID=messageID;
		lastMailLen=maillen;
		// List<String> nls;
		// for (int k=i;k<j;k++)
		// nls.add(ls.num(k));
		// if (writesepmails) {
		// writelinestofile(nls,Sformat("%s/%s",destdir,getnextfilename("txt")));
		// }
	}

	fclose(instream);
	if (writeToOneFile)
		fclose(outstream);

}
