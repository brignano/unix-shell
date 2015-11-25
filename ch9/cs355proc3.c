#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#define BUFFERSIZE 4096
#define COPYMODE 0644

int main(int ac, char *av[])
{
    int  in_fd, out_fd, n_chars;
    char buf[BUFFERSIZE];
						
    /* check arguments */
   if (ac == 6){
    	printf("Too many Arguments\n");
        return 1;
    }
      int x=1;
      int bool =0;
      if(strchr(av[1],'-'))
      {
	if(strchr(av[1],'e')||strchr(av[1],'n')||strchr(av[1],'s')
           )
           {
             x=2;
             bool=1;
           }
           }
    /* open files */
  
     for(;x<ac;x++)
        {
        
    if ((in_fd=open(av[x], O_RDONLY)) == -1) {
    
        perror("Cannot open source file");
        return 1;
    }
    printf("%s\n",av[x]);
	int i=0;
	int j=1; 
    /* copy files */
    while ((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0)
        if (write(out_fd, buf, n_chars) != n_chars) {
        for(i=0; i<n_chars;i++)
        {
        
        if(bool==1)
        {
        if((strchr(av[1],'s'))&& (buf[i]=='\n'))
          {	
  			while(isspace(buf[i+1])){i++;}
  			buf[i]='\n';
              
          }
          
          
         if((strchr(av[1],'n')))
          {	
           if(j==1)
                   {
                    printf("1\t");
                       j++;
                   }
                    
                   else if(buf[i-1]=='\n')
                   {
                 
                   printf("%d\t",j);
              j++;  
        
                   } 
          }
          
          if((strchr(av[1],'e'))&& (buf[i+1]=='\n'))
          {	
          printf("$"); 
              
          }
          
       printf("%c",buf[i]);
       
        }
        else
        {printf("%c",buf[i]);}
        }
    }
    
                
    if (n_chars == -1) {
        perror("read error");       
        return 1;
    }
	
    /* close files	*/
    if (close(in_fd) == -1 || close(out_fd) == -1) {
        //perror("Error closing file(s)	");    
    }
   }     	
    return 0;
}