#define YES 1
#define No  0

char *next_cmd();
char **splitline(char *);
void freelist(char **);
void *emalloc(size_t);
void *erealloc(void *, size_t);
int  execute(char **);
void fatal(char *, char *, int);
int  process();
int  pipe1(char**);
int  redirect(char**);
int  fork();

int  VLenviron2table(char **);
int  VLexport(char *);
char *VLlookup(char *);
void VLlist();
int  VLstore(char *, char *);
int  VLreference(char *);
char **VLtable2environ();
int  VLenviron2table(char **);
int  VLunset(char *);
