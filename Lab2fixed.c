#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>

typedef struct node{
  char name[64];
  char type;
  struct node *child, *parent, *sibling;
}NODE;


NODE *root, *cwd, *start;
char line[128];                        // User input
char command[16], pathname[64];        
char dname[64], bname[64];


char *name[128];      // Pointers to token strings in pathname[]
int n;                // Number of tokens in pathname


char *cmd[] = {"mkdir", "rmdir", "ls", "cd", "pwd", "creat", "rm", "reload",
	       "save", "menu", "quit", NULL};



NODE *makeNODE(char *NODEname, char NODEtype);

int findCmd(char *cmand);

int tokenize(char *pathname);


NODE *search_child(NODE *parent, char *name);


NODE *path2node(char *pathname);


void initialize();

void pwdFILE(FILE *fp, NODE *currentNODE);

//void printNODE(FILE *fp, NODE *currentNODE);

void printTree(FILE *fp, NODE *currentNODE);

void save(char *filename);

int dbname(char *pathname);

int mkdir(char *filename);

int rmdir(char *filename);

int creat(char *filename);

int rm(char *filename);

int cd(char *filename);

int ls(char *filename);

void pwdr(NODE *parentNODE);

int pwd();

int reload(char *filename);

void deleteTree(NODE *NODEpointer);

void menu();

int main()
{

  int index = 12;
  char myLine[128], cmand[16], pthname[64];
  int inputCheck;
  initialize();

  while (index != 10){
    printf("Input a command line : ");
    fgets(myLine, 128, stdin);
    myLine[strlen(myLine) - 1] = 0;
    inputCheck = sscanf(myLine, "%s %s", cmand, pthname);
    if (inputCheck > 0){
      index = findCmd(cmand);

      switch(index) {
      case 0 :     // mkdir
	if (inputCheck == 2){
	  mkdir(pthname);
	  printf("\n\n");
	}
	else {
	  printf("mkdir command requires a pathname\n\n");
	}
	break;
	
      case 1 : //rmdir
	if (inputCheck == 2){
	  rmdir(pthname);
	  printf("\n\n");
	}
	else {
	  printf("rmdir command requires a pathname\n\n");
	}
	break;
	
      case 2: //ls
	if (inputCheck == 2){
	  ls(pthname);
	  printf("\n\n");
	}
	else {
	  ls("");  // lists current directory
	  printf("\n\n");
	}
	break;

      case 3 : //cd
	if (inputCheck == 2){
	  cd(pthname);
	  printf("\n\n");
	}
	else {
	  cd(""); // changes to home directory
	  printf("\n\n");
	}
	break;

      case 4 : //pwd
	if (inputCheck == 1){
	  pwd(pthname);
	  printf("\n\n");
	}
	else {
	  printf("pwd command does not use a pathname\n\n");
	}
	break;

      case 5: //creat
	if (inputCheck == 2){
	  creat(pthname);
	  printf("\n\n");
	}
	else {
	  printf("creat command requires a pathname\n\n");
	}
	break;

      case 6: //rm
	if (inputCheck == 2){
	  rm(pthname);
	  printf("\n\n");
	}
	else {
	  printf("rm command requires a pathname\n\n");
	}
	break;

      case 7 : //reload
	if (inputCheck == 2){
	  reload(pthname);
	  printf("\n\n");
	}
	else {
	  printf("reload command requires a filename\n\n");
	}
	break;

      case 8 : //save
	if (inputCheck == 2){
	  save(pthname);
	  printf("\n\n");
	}
	else {
	  printf("save command requires a filename\n\n");
	}
	break;

      case 9 : //menu
	if (inputCheck == 1){
	  menu();
	  printf("\n\n");
	}
	else {
	  printf("menu command has no pathname\n\n");
	}
	break;

      case 10 : //quit
	if (inputCheck == 1){
	  printf("Saving the file system to Lab2outfile and quitting\n\n");
	}
	else {
	  printf("quit command has no pathname\n\n");
	  inputCheck = 12; // keeps going because command was bad
	}
	break;

      default :
	printf("Command not recognized. Type \"menu\" for options\n\n");
      }
    }
    else {
      printf("You must enter at least a command\n\n\n");
    }
  }

  /* while(strcmp(command, "quit") != 0){
    printf("Input command: ");
    fgets(line, 128, stdin);
    line[strlen(line) - 1] = 0;
    sscanf(line, "%s %s", command, pathname);
    index = findCmd(command);
    switch(index){
      
    }
    }*/

  // reload("Lab2outfile");

  // creat("file4");

  /* mkdir("/dir1");
  mkdir("dir2");

  creat("file1");
  creat("/dir1/file2");

  ls("dir1");
  ls("");
  //printf("Exit?\n?");
  ls("/dir1/file2");

  rmdir("dir1");

  pwd();
  
  //printf("Error 1?\n");
  
  rmdir("/dir2");

  rm("file2");

  //printf("Error 2?\n");


  cd("dir1");
  //printf("Hello!");

  pwd();
  
  ls("");

  rm("file2");

  ls("");

  cd("");

  pwd();
  rmdir("dir1");

  ls("");

  mkdir("dir3");
  mkdir("dir3/dir4");

  cd("dir3/dir4");

  pwd();*/

  
  save("Lab2outfile");

  deleteTree(root);
  
  return 0;
}


NODE *makeNODE(char *NODEname, char NODEtype)
{
  NODE *newNODE = (NODE *)malloc(sizeof(NODE));
  strcpy(newNODE->name, NODEname);
  newNODE->type = NODEtype;
  newNODE->parent = 0;
  newNODE->sibling = 0;
  newNODE->child = 0;
}


int findCmd(char *cmand)
{
  int i = 0;
  while (cmd[i]){
    if (!strcmp(cmand, cmd[i]))
      return i;
    i++;
  }
  return -1;
}


int tokenize(char *pathname)
{
  char *s;
  int i;

  //printf("%s\n", pathname);

  /*if (pathname[strlen(pathname)] == '\0'){
    printf("Includes null.\n");
    }*/
  
  n = 0;
  s = strtok(pathname,"/");
  
  while(s){
    name[n] = s;
    s = strtok(0, "/");
    n++;
  }

  
  /*for (i=0; i<n; i++){
    printf("token[%d]=%s\n", i, name[i]);
    }*/
  // printf("n = %d\n", n);
}



NODE *search_child(NODE *parent, char *name)
{
  NODE *cp;
  cp = parent->child;

  if (cp == 0)
    return 0;

  while(cp){
    //printf("%s\n", cp->name);
    if (strcmp(cp->name, name) == 0){
      return cp;
    }
    cp = cp->sibling;
  }
  return 0;
}



NODE *path2node(char *pathname)
{
  int i;
  NODE *p;

  start = root;
  if (pathname[0] != '/')
    start = cwd;

  //printf("Here?\n");
  
  tokenize(pathname);

  if (n==0)             // if no token strings in pathname[]
    return start;
  
  p = start;
  for (i=0; i<n; i++){
    if (strcmp(name[i], "..") == 0){
      if (p != root){
	p = p->parent;
      }
      else {
	printf("Error. Directory / has no parent.\n");
	return 0;
      }
    }
    else {
      p = search_child(p, name[i]);
      if (p==0){
        printf("can't find %s\n", name[i]);
        return 0;
      }
    }
  }
  return p;
}



void initialize()
{
  root = (NODE*)malloc(sizeof(NODE));
  strcpy(root->name, "/");
  root->type = 'D';
  root->child = 0;
  root->parent = root;
  root->sibling = root;

  cwd = root;
}



void pwdFile(FILE *fp, NODE *currentNODE)
{
  if (currentNODE == root){
    fprintf(fp, "/");
    return;
  }
  else if(currentNODE->parent != root){
    pwdFile(fp, currentNODE->parent);
    fprintf(fp, "/%s", currentNODE->name);
  }
  else {
    pwdFile(fp, currentNODE->parent);
    fprintf(fp, "%s", currentNODE->name);
  }
	
    
}


void printTree(FILE *fp, NODE *currentNODE)
{
  if (currentNODE == 0)
    return;
  fprintf(fp, "%c        ", currentNODE->type);
  pwdFile(fp, currentNODE);
  fprintf(fp, "\n");

  printTree(fp, currentNODE->child);
  if (currentNODE != root){   // Since root->sibling = root
    printTree(fp, currentNODE->sibling);
  }
}


void save(char *filename)
{
  FILE *fp = fopen(filename, "w+");

  printTree(fp, root);

  fclose(fp); 
}



int dbname(char *pathname)
{
  char temp[128];
  strcpy(temp, pathname);
  strcpy(dname, dirname(temp));
  strcpy(temp, pathname);
  strcpy(bname, basename(temp));
}

int mkdir(char *filename)
{

  NODE *p;
  char temp[128];
  
  dbname(filename);

  //printf("dname: %s\n", dname);

  if (strcmp(dname, ".") == 0){
    p = cwd;
  }
  else{
    strcpy(temp, dname);
    p = path2node(temp);
  }
  if (p != 0){
    if (p->type == 'D'){
      NODE *d = search_child(p, bname);
      
      if (d == 0){
       NODE *newNODE = makeNODE(bname, 'D');
       printf("Creating new directory %s in directory %s\n", bname, dname);
       newNODE->parent = p;
       if (p->child == 0){
   	p->child = newNODE;
       }
       else{
	NODE *childNODE = p->child;
	while(childNODE->sibling != 0){
	   childNODE = childNODE->sibling;
        }
        childNODE->sibling = newNODE;
       }
      }
      else{
	printf("Directory already exists\n");
	return 0;
      }
    }
    else{
      printf("Directory can only be made in another directory\n");
      return 0;
    }
  }
  else{
    printf("Parent directory does not exist\n");
    return 0;
  }
  

  return 1;

  
}


int rmdir(char *filename)
{
   NODE *p;
   NODE *d;

   char temp[128];
  
  dbname(filename);

  //printf("dname: %s\n", dname);

  if (strcmp(dname, ".") == 0){
    p = cwd;
  }
  else{
    strcpy(temp, dname);
    p = path2node(temp);
  }

  if (p != 0){
    d = search_child(p, bname);
    if (d != 0){
      if (d->type != 'D'){
	printf("%s is not a directory\n", d->name);
	return 0;
      }
      if (d == cwd){
        printf("You cannot remove the CWD\n");
	return 0;
      }
      if (d->child == 0){
	printf("Removing directory %s from parent %s\n", d->name, dname);
	if (p->child == d){
	  p->child = d->sibling;
	  free(d);
	}
	else {
          NODE *sib = p->child;
	  while (sib->sibling != d){
	    sib = sib->sibling;
          }
	  sib->sibling = d->sibling;
	  free(d);
	}
      }
      else {
	printf("Directory %s is not empty and cannot be removed\n", bname);
	return 0;
      }
    }
    else {
      printf("No directory %s under parent %s\n", bname, dname);
      return 0;
    }
  }
  else{
    printf("Path does not exist\n");
    return 0;
  }

  return 1;   // executed successfully
}




int creat(char *filename)
{
  NODE *p;
  char temp[128];
  
  dbname(filename);

  //printf("dname: %s\n", dname);

  if (strcmp(dname, ".") == 0){
    p = cwd;
  }
  else{
    strcpy(temp, dname);
    p = path2node(temp);
  }
  if (p != 0){
    if (p->type == 'D'){
      NODE *d = search_child(p, bname);
      
      if (d == 0){
       NODE *newNODE = makeNODE(bname, 'F');
       printf("Creating new file %s in directory %s\n", bname, dname);
       newNODE->parent = p;
       if (p->child == 0){
   	p->child = newNODE;
       }
       else{
	NODE *childNODE = p->child;
	while(childNODE->sibling != 0){
	   childNODE = childNODE->sibling;
        }
        childNODE->sibling = newNODE;
       }
      }
      else{
	printf("File already exists\n");
	return 0;
      }
    }
    else{
      printf("File can only be made in a directory\n");
      return 0;
    }
  }
  else{
    printf("Parent directory does not exist\n");
    return 0;
  }
  

  return 1;
}




int rm(char *filename)
{
   NODE *p;
   NODE *d;

   char temp[128];
  
  dbname(filename);

  //printf("dname: %s\n", dname);

  if (strcmp(dname, ".") == 0){
    p = cwd;
  }
  else{
    strcpy(temp, dname);
    p = path2node(temp);
  }

  if (p != 0){
    d = search_child(p, bname);
    if (d != 0){
      if (d->type != 'F'){
	printf("%s is not a file\n", d->name);
	return 0;
      }
      else{
	printf("Removing file %s from parent %s\n", d->name, dname);
	if (p->child == d){
	  p->child = d->sibling;
	  free(d);
	}
	else {
          NODE *sib = p->child;
	  while (sib->sibling != d){
	    sib = sib->sibling;
          }
	  sib->sibling = d->sibling;
	  free(d);
	}
      }
      
    }
    else {
      printf("No file %s under parent %s\n", bname, dname);
      return 0;
    }
  }
  else{
    printf("Path does not exist\n");
    return 0;
  }

  return 1;   // executed successfully
}



// precondition: no pathname is string ""
int cd(char *filename)
{
  char pthname[128];
  strcpy(pthname, filename);
  if (strcmp("",pthname) == 0){
    printf("Returning to home directory\n");
    cwd = root;
    return 1;
  }
  else {
    NODE *p = path2node(pthname);
    if (p != 0) {
      if (p->type == 'D'){
	printf("Entering directory %s\n", filename);
	cwd = p;
      }
      else{
	printf("%s is not a directory\n", filename);
	return 0;
      }
    }
    else {
      printf("Cannot find %s\n", filename);
      return 0;
    }
  }

  return 1; // executed successfully
}



// Precondition: no pathname is "" string
int ls(char *filename)
{
  NODE *p;
  NODE *d;

  char pthname[128];
  strcpy(pthname, filename);

  if (strcmp("", pthname) == 0){
    p = cwd;
    d = p->child;
    printf("**** ls: %s\n", p->name);
    while (d != 0){
      // printf("Bugfinder!\n");
      printf("%c        %s\n", d->type, d->name);
      d = d->sibling;
    }
    printf("\n\n");
  }
  else {
    //printf("Print 0\n");
    p = path2node(pthname);     // Searches for pathname
    //printf("Print 0.5\n");
    if (p != 0){
      //printf("Print 1\n");
      if (p->type == 'D'){
	//printf("Print 2\n");
	d = p->child;
	printf("**** ls: %s\n", filename);
	while (d != 0){
	  printf("%c        %s\n", d->type, d->name);
	  d = d->sibling;
	}
	printf("\n\n");
      }
      else{
	printf("ls command only works for directories\n");
	return 0;
      }
    }
    else{
      printf("Pathname %s does not exist\n", filename);
      return 0;
    }
  }

  return 1;
}





void pwdr(NODE *parentNODE){
  if (parentNODE == root){
    return;
  }
  else if (parentNODE->parent != root){
    pwdr(parentNODE->parent);
    strcat(pathname, "/");
    strcat(pathname, parentNODE->name);
  }
  else {
    pwdr(parentNODE->parent);
    strcat(pathname, parentNODE->name);
  }

}

int pwd()
{
  strcpy(pathname, "/");
  pwdr(cwd);
  printf("pathname to cwd: %s\n", pathname);
  
}


void deleteTree(NODE *NODEpointer)
{
  if (NODEpointer == 0){
    return;
  }
  deleteTree(NODEpointer->child);
  if (NODEpointer != root){
    deleteTree(NODEpointer->sibling);
  }

  free(NODEpointer);
}


int reload(char *filename)
{

  char myLine[200];
  FILE *fp = fopen(filename, "r");
  char type;
  char types[5];
  char pthname[128];
  
  deleteTree(root);
  initialize();

  if (fp){
    while (fgets(myLine, 150, fp)){
      myLine[strlen(myLine) - 1] = 0;
      sscanf(myLine, "%s %s", types, pthname);
      type = types[0];
      if (strcmp("/", pthname) != 0){
	if (type == 'D'){
	  mkdir(pthname);
	}
	else if (type == 'F'){
	  creat(pthname);
	}
      }
    }
    fclose(fp);
  }
  else {
    printf("File %s cannot be opened\n", filename);
  }
}




void menu()
{
  printf("*****Command options*****\n");
  printf("mkdir pathname : make a new directory for a given pathname\n");
  printf("rmdir pathname : remove the directory, if it is empty\n");
  printf("cd  [pathname] : change CWD to pathname, or to / if no pathname\n");
  printf("ls  [pathname] : list the directory contents of pathname or CWD\n");
  printf("pwd            : print the (absolute) pathname of CWD\n");
  printf("creat pathname : create a FILE node\n");
  printf("rm    pathname : remove the FILE node\n");
  printf("save  filename : save the current file system tree as a file\n");
  printf("reload filename: construct a file system tree from a file\n");
  printf("menu           : show a menu of valid commands\n");
  printf("quit           : save the file system tree, then terminate the program\n");
}
