#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

#define LEN 40
#define OLEN 10
#define LLEN 60
#define MLEN 1000
#define OPLEN 5000
#define space ' '
#define tab '\t'
#define nul '\0'
#define nxtl '\n'
#define iscolon ':'
#define atoi_(x) (x - 48)
#define isdigit(p) (p >= '0' && p <= '9')
#define isalpha(p) ((p >= 'a' && p <= 'z') || (p >= 'A' && p <= 'Z'))
#define isalnum(p) (isalpha(p) || isdigit(p))

typedef enum{
	MEM, PLS, MINS, MUL, PLSARR, MINSARR, MULARR, CO, FP, CPAREN, OPAREN, NL, RET, BR, SBRNM, LBNM, END, IMG, IMG8, RSH, LSH, R32, R64, ARW, UNKN
}Wrdt; Wrdt wrd;

typedef struct{
	char sbnm[LEN];
	unsigned char prcnt[LEN]; // for parameter count
}VSbr; VSbr v[LEN];//for declared sbr

int sbrcnt = 0; //for total count of sbr
unsigned char csbr[LLEN][LEN]; // for called sbr
int cs = 0; // for called sbr count

typedef struct {
    char lbnm[LLEN];
    int set;  // 1 for declared, 0 for called only
} VLabels;
VLabels *labels = NULL;  // Dynamic array for labels
int lbcnt = 0;    // Count the number of labels

unsigned char output[OPLEN];
int out = 0;
char reg1[OLEN]; char reg2[OLEN]; char reg3[OLEN]; char IMG_val[OLEN]; char IMG8_val[OLEN]; char oprtr_val[OLEN];//for translation to store values
/*typedef enum {
	STR, MOV, LDR, LSL, LSR
}INST_TYPE;
INST_TYPE inst[LEN];*/
char inpt[MLEN] = {"( $sb12 \n ( $sb12 #32 \n  :tant \n W17 *<- #32 \n X7 <- X8 - X2 \n CB W19 >= W21 :sard \n W12 <- W8 << #31 \n b :sandy \n  :sard \n X21 -<- X19 \n mem X21 #40 <- W19 \n :sd12 \n ret \n mem X17 #16 <- W25 \n :sandy \n  X26 <- mem X21 #32 \n b $sd12 \n b :tant \n X21 <- #32 \n :sarT17 \n X2 <- X3 + X4 \n X21 <- X27 * X1 \n W17 <- W8 >> #32 \n X23 *<- #24 \n ) \n  ( $sd12 #32 \n  mem X21 #40 <- W19 \n :may17 \n X21 <- mem X25 #24 \n b :tk38 \n  X26 <- mem X21 #32 \n :june28  X2 < X5 :may17 \n X21 <- #32 \n X2 <- X3 + X4 \n W24 <- W19 \n W17 <- W8 >> #32 \n X23 *<- #24 \n  :tk38 \n ) \n )"};
int g = 0; //global variable for inpt
void next_w();

bool valid_nm(char t[])
{       bool out = false;     
       	int i = 2;
       if (isalpha(t[1])){
           while(t[i] != nul){
                    if (!isalnum (t[i])){
			  printf("Invalid lbnm/sbrnm\n");
			  exit(1);
               	    } 
		       i++;
	   }
	       out = true;             
       }
       return out;
}

bool reg_num(char num[]){
        bool out = false;
        if(num[2] == '\0'){
                if(0 <= atoi_(num[1]) <= 9)
                        out = true;
        }
        else if(num[3] == '\0'){
                if(((10*atoi_(num[1])) + atoi_(num[2])) < 29)
                        out = true;
        }
        return out;
}

bool is_img8(char t[]){
	   int num = 0;
	   int i = 1;
	   bool out = false;
	if (t[0] == '#'){
            while (t[i] != nul){
                if (!isdigit (t[i])){
		  printf("Invalid Img8 value\n");
		  exit(1);
		}
                num = num * 10 + (t[i] - '0');
                i += 1;
            }
            if (num % 8 == 0) 
                out = true;
        }
        return out;
}

bool is_img(char t[]){	
	   int i = 1;
	   bool out = false;
	if (t[0] == '#'){
            while (t[i] != nul){
                if (!isdigit (t[i])){
		  printf("Invalid Imgvalue\n");
		  exit(1);
		}
                i += 1;
            }
	    out = true;
	}
        return out;
}

char nw[LEN];
void next_w(){
    int i = 0;
    while (inpt[g]== space || inpt[g]== tab)
        g++;
    while(inpt[g] != space && inpt[g] != tab && inpt[g] != nul && i < LEN -1){
        nw[i] = inpt[g];
        g++;
        nw[i+1] = nul;
        i++;
    }
    i++;
    if(nw[0] == 'm' && nw[1] == 'e' && nw[2] == 'm' && nw[3] == nul)
        wrd = MEM;
    else if(nw[0] == '+' && nw[1] == nul)
        wrd = PLS;
    else if(nw[0] == '-' && nw[1] == nul)
        wrd = MINS;
    else if(nw[0] == '*' && nw[1] == nul)
        wrd = MUL;
    else if(nw[0] == 'F' && nw[1] == 'P' && nw[2] == nul)
        wrd = FP;
    else if(nw[0] == '<' && nw[1] == '<' && nw[2] == nul)
        wrd = LSH;
    else if(nw[0] == '>' && nw[1] == '>' && nw[2] == nul)
        wrd = RSH;
    else if(nw[0] == '<' && nw[1] == '-' && nw[2] == nul)
        wrd = ARW;
    else if(nw[0] == '+' && nw[1] == '<' && nw[2] == '-' && nw[3] == nul)
        wrd = PLSARR;
    else if(nw[0] == '-' && nw[1] == '<' && nw[2] == '-' && nw[3] == nul)
        wrd = MINSARR;
    else if(nw[0] == '*' && nw[1] == '<' && nw[2] == '-' && nw[3] == nul)
        wrd = MULARR;
    else if(inpt[g-1] == nul)
        wrd = END;
    else if(inpt[g-1] == nxtl)
        wrd = NL;
    else if(nw[0] == '$' && valid_nm(nw))
    	wrd = SBRNM;
    else if(nw[0] == ':' && valid_nm(nw))
        wrd = LBNM;
    else if(nw[0] == 'C' && nw[1] == 'B' && nw[2] == nul)
        wrd = LBNM;
    else if(nw[0] == 'b' && nw[1] == nul)
        wrd = BR;
    else if((nw[0] == '<' || nw[0] == '>') && nw[1] == '=' && nw[2] == nul)
        wrd = CO;
    else if((nw[0] == '<' || nw[0] == '>') && nw[1] == nul)
        wrd = CO;
    else if(nw[0] == '=' && nw[1] == '=' && nw[2] == nul)
        wrd = CO;
    else if(nw[0] == '!' && nw[1] == '=' && nw[2] == nul)
        wrd = CO;
    else if(nw[0] == 'r' && nw[1] == 'e' && nw[2] == 't' && nw[3] == nul)
        wrd = RET;
    else if( nw[0] == ')' && nw[1] == nul)
        wrd = CPAREN;
    else if( nw[0] == '(' && nw[1] == nul)
        wrd = OPAREN;
    else if (nw[0] == 'X' && reg_num(nw))
	 wrd = R64;
    else if (nw[0] == 'W' && reg_num(nw))
	 wrd = R32;
    else if(is_img8(nw))
            wrd = IMG8;
    else if(is_img(nw))
            wrd = IMG;
    else
        wrd = UNKN;
}

void validate(Wrdt c){
    if(wrd != c)
        exit(1);
}

void resizeVLabels(size_t newSize) {
    labels = realloc(labels, newSize * sizeof(VLabels));
    if (labels == NULL) { 
        printf("Realloc failed"); 
        exit(1); 
    }
}

void set(){
   unsigned char s[70] = "STR FP,[SP,#-8]!\nSTR LR,[SP,#-8]!\nMOV FP,SP\n";
        for(int i=0; s[i]!= nul; i++)
       		 output[out++] = s[i];
}

void reset(){
    unsigned char r[70] = "MOV SP,FP\nLDR LR,[SP],#8\nLDR FP,[SP],#8\n\n";
        for(int i=0; r[i]!= nul; i++)
        	output[out++] = r[i];
}

void append(char a[]){
        for(int i=0; a[i] != nul; i++)
                output[out++] = a[i];
}

void verify_sbr(){
  	if (wrd == OPAREN){
	 	next_w();
	   	validate(SBRNM);
	   for(int i = 0; i<sbrcnt; i++){
	   	if (strcmp(v[i].sbnm,nw+1)==0) {
	        printf("FOUND MULTIPLE SBR WITH NAME :%s\n",nw+1);
		exit(1); 
	     	}
	   }
	   strcpy(v[sbrcnt].sbnm,nw+1);
	                  output[out++] ='_';
	        output[out++] ='_';
                append(v[sbrcnt].sbnm);
	        output[out++] =':';
     		output[out++] = nxtl;
     		   set();
     		output[out++] = nxtl;

	   next_w();
	   if (wrd == IMG || wrd == IMG8){
		   strcpy(v[sbrcnt].prcnt,nw+1);
	   next_w();
	   validate(NL);
	   }else
		   exit(1);
	}
	else{
	 printf("error in sbr\n");	
	 exit(1);
	}
}

void verify_inst(){ 
      if (wrd == CPAREN){ 
        	output[out++] = nxtl;
	       	reset();
       	 next_w();
       	 validate(NL);
         output[out++] = nxtl;
	 	for(int i =0; i<lbcnt; i++){
                    if(labels[i].set == 0){
                        printf("Invalid VLabels call of label : %s\n",labels[i].lbnm);
			exit(1);
                    }
                }
                free(labels);
                labels = NULL;
                lbcnt = 0;
		sbrcnt++;
	 next_w();
      }	   
      else{ 
	   if (wrd == MEM){  
		char instr[LLEN] = "STR";
    		 next_w();
		validate(R64);
		strcpy(reg1,nw);
   		  next_w();
   	  	validate(IMG8);
		strcpy(IMG8_val,nw);
  		   next_w();
     		validate(ARW);
    		  next_w();
	     	if (wrd == R64 || wrd == R32){
		  append(instr);
		  output[out++] = ' ';
		  append(nw);
		  output[out++] = ',';
		  output[out++] = '[';
		  append(reg1);
		  output[out++] = ',';
		  append(IMG8_val);
		  output[out++] = ']';
		  output[out++] = nxtl;
		  next_w();
    	 	  validate(NL);
		  next_w();
		}else{
			exit(1);
		 }	
 	    }
	    else if (wrd == BR){
	   	    next_w();
	      	 if (wrd == LBNM){	       
        	    // Check if label exists in the labels list
       		     int labelFound = 0;
		     	for(int i =0; i<lbcnt; i++){
       			         if (strcmp(labels[i].lbnm, nw+1) == 0) {
                		    labelFound = 1;
              			  }
           		 }
          		  if (!labelFound) {
          		      // If label doesn't exist, create it as only called;
          		      resizeVLabels(lbcnt + 1);
  		              labels[lbcnt].set = 0;
        		        strcpy(labels[lbcnt].lbnm, nw+1);
           			     lbcnt += 1;
     			  }
			  output[out++]='B';
			  output[out++]=' ';
			  output[out++]='_';
			  output[out++]='_';
                	  append(v[sbrcnt].sbnm);
			  output[out++]='_';
			  append(nw+1);
			  output[out++]=':';
		  	  output[out++] = nxtl;
			  next_w();
			  validate(NL);
			  next_w();
	       }
	       else if (wrd == SBRNM){
		       strcpy(csbr[cs],nw+1);
		       cs++;
			  output[out++]='B';
			  output[out++]=' ';
			  output[out++]='_';
			  output[out++]='_';
                	  append(v[sbrcnt].sbnm);
			  output[out++]='_';
			  append(nw);
		       next_w();
			validate(NL);
		  	  output[out++] = nxtl;
		  next_w();
	       } else
		       exit(1);
	 }
	 else if (wrd == LBNM){   
	        int labelFound = 0;
		   for(int i =0; i<lbcnt; i++){
        	    if (strcmp(labels[i].lbnm, nw+1) == 0) {
        	        // If label already exists, do it as set
        	        labels[i].set = 1;
            		    labelFound = 1;
           		     break;
         	         }
      	           }
		   if (!labelFound && nw[0] == ':') {
       		         resizeVLabels(lbcnt + 1);
         	       strcpy(labels[lbcnt].lbnm, nw+1);
            		    labels[lbcnt].set = 1;
      		          lbcnt++;
                        }
		        if(nw[0] == ':'){
			output[out++] = '_';
        	        output[out++] = '_';
            		 append(v[sbrcnt].sbnm);
         	       output[out++] = '_';
       		         append(nw+1);
         	       output[out++] = ':';
       		         output[out++] = nxtl;
			}
			        // Process conditional branch
        	    next_w();
        	    if(wrd == R32 || wrd == R64){
			    char instr[] = "CMP";
			    append(instr);
			    output[out++] = ' ';
			    append(nw);
			    output[out++] = ',';
          	     	     Wrdt regType1 = wrd;
       		        	 next_w();
     		           validate(CO);
			    if (strcmp(nw,"<")==0)
					    strcpy(oprtr_val,"BLT");
			    else if(strcmp(nw,"<=")==0)
					    strcpy(oprtr_val,"BLE");
			    else if(strcmp(nw,">")==0)
					    strcpy(oprtr_val,"BGT");
			    else if(strcmp(nw,">=")==0)
					    strcpy(oprtr_val,"BGE");
			    else if(strcmp(nw,"==")==0)
					    strcpy(oprtr_val,"BE");
			    else if(strcmp(nw,"!=")==0)
					    strcpy(oprtr_val,"BNE");
    		             next_w();
  		             if (wrd == regType1 || wrd == IMG || wrd == IMG8){
			    	append(nw);
       		         	output[out++] = nxtl;
        		        next_w(); 
             			   if(wrd == LBNM){ // Handle target label
         			           int targetFound = 0;
 			                   for(int i=0; i<lbcnt; i++){
                		       		 if(strcmp(labels[i].lbnm, nw+1) == 0) 
							 targetFound = 1;
             				    }
               			     if(!targetFound){
              			          resizeVLabels(lbcnt + 1);
         	   			    strcpy(labels[lbcnt].lbnm, nw+1);
                   			     labels[lbcnt].set = 0;
               				         lbcnt++;
                   		     }
                  			   append(oprtr_val);
               			           output[out++]=' ';
               			           append(nw+1);
                     			     output[out++]=':';
                		           output[out++] = nxtl;
          		            next_w();
                  		    validate(NL);
                  			    next_w();
                 		}else
					exit(1);
               		   }else
				   exit(1);
            	    }
          	    else if (wrd == NL){
                        next_w();
              	   }else
			   exit(1);
         }
	 else if (wrd == RET){
	      	next_w();
		char instr[LLEN] = "RET";
		append(instr);
		validate(NL);
                output[out++] = nxtl;
		next_w();
		reset();
	 }
	 else if (wrd == R64 || wrd == R32 ){
          	 Wrdt regType = wrd;
		 strcpy(reg1,nw);   
	         next_w();
		 if (wrd == ARW){
		    	next_w();
			 if (wrd == regType ){
		 		strcpy(reg2,nw);   
		      		 next_w();
				if (wrd == NL){
					char instr[LLEN] = "MOV";
					append(instr);
					output[out++] = ' ';
					append(reg1);
					output[out++] = ',';
					append(reg2);
					output[out++] = nxtl;
					next_w();
			 	}
				else if (wrd == PLS || wrd == MINS || wrd == MUL){             
				       	switch (nw[0]){
                                                case '+':
                                                        strcpy(oprtr_val,"ADD");
                                                        break;
                                                case '-':
                                                        strcpy(oprtr_val,"SUB");
                                                        break;
                                                case '*':
                                                        strcpy(oprtr_val,"MUL");
                                                        break;
                                        	}
				     next_w();
				      if (wrd == regType || wrd == IMG || wrd == IMG8){
		 			(wrd == regType) ? strcpy(reg3,nw) : strcpy(IMG_val,nw);   
					append(oprtr_val);
					output[out++] = ' ';
					append(reg1);
					output[out++] = ',';
					append(reg2);
					output[out++] = ',';
					(wrd == regType) ? append(reg3) : append(IMG_val);
					 next_w();
				         validate(NL);
              				output[out++] = nxtl;
				 	   next_w();
				       }else
					       exit(1);
				}
				else if (wrd==RSH || wrd==LSH){
				     switch(nw[0]){
					     case '<':
						     strcpy(oprtr_val,"LSL");
						     break;
					     case '>':
						     strcpy(oprtr_val,"LSR");
						     break;
				     	}
					     next_w();
					append(oprtr_val);
					output[out++] = ' ';
					append(reg1);
					output[out++] = ',';
					append(reg2);
					output[out++] = ',';
				      if (wrd == IMG || wrd == IMG8){
					  wrd == IMG ? strcpy(IMG_val,nw) : strcpy(IMG8_val,nw);
					  wrd == IMG ? append(IMG_val) : append(IMG8_val);
					 next_w();
				         validate(NL);
					output[out++] = nxtl;
				 	   next_w();
				       }
				}
				else
					exit(1);
			 }
			 else if (wrd == IMG8 || wrd == IMG){
				char instr[LLEN] = "MOV";
				append(instr);
				output[out++] = ' ';
				append(reg1);
				output[out++] = ',';
				append(nw);
		      		 next_w();
		      		 validate(NL);
				output[out++] = nxtl;
				next_w();
			 }
		         else if (wrd == MEM){
				char instr[LLEN] = "LDR";
				append(instr);
				output[out++] = ' ';
				append(reg1);
				output[out++] = ',';
		  		output[out++] = '[';
    				 next_w();
				validate(R64);
				append(nw);
				output[out++] = ',';
   				  next_w();
   	 		 	validate(IMG8);
				append(nw);
		  		output[out++] = ']';
  				   next_w();
     				validate(NL);
				output[out++] = nxtl;
    				  next_w();
			 }
			 else
				exit(1);	 
		 }
		 else if (wrd==PLSARR || wrd==MINSARR || wrd== MULARR){
                                        switch (nw[0]){
                                                case '+':
                                                        strcpy(oprtr_val,"ADD");
                                                        break;
                                                case '-':
                                                        strcpy(oprtr_val,"SUB");
                                                        break;
                                                case '*':
                                                        strcpy(oprtr_val,"MUL");
                                                        break;
                                        	}

		     next_w();
		     if (wrd == regType || wrd == IMG || wrd == IMG8){
					append(oprtr_val);
					output[out++] = ' ';
					append(reg1);
					output[out++] = ',';
					append(reg1);
					output[out++] = ',';
					append(nw);
  			  	next_w();
     	 			validate(NL);
				output[out++] = nxtl;
      				next_w();
		     }else
			     exit(1);
		 }else
			 exit(1);
	    }
	   else{
		    exit(1);
             }		    
           verify_inst();
      }	   
}

void exist_sbr(){
    int i, j, found;
    for (i = 0; i < cs; i++) {
        found = 0;
        for (j = 0; j < sbrcnt; j++) {
            if (strcmp(csbr[i], v[j].sbnm) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
           printf("ERROR: Subroutine \"%s\" used but not defined  \n",csbr[i]);
            exit(1);
        }
    }
}

void main()
{  
     	// Allocate initial space for labels 
    labels = malloc(sizeof(VLabels));
    if (labels == NULL) {
        printf("Initial memory allocation failed.\n");
        exit(1);
    }
       	next_w();
   if (wrd == OPAREN){
	next_w();
   	validate(SBRNM);
	strcpy(csbr[cs],nw+1);
	cs++;
	next_w();
	validate(NL);
       	next_w();
    while (wrd != CPAREN)
    {
       verify_sbr();
       next_w();
       verify_inst();
    }
    validate(CPAREN);
    next_w();
   }
   exist_sbr();
     printf("%s\n",output);
}	
