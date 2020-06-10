#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

unsigned char ***S;


int usage(char* progName);
char* concat(const char *s1, const char *s2);
char **cli_opt(int argc, char *argv[]);
unsigned char *hex2uch(char *hex);
unsigned char chvalue(char h);
unsigned char *Expansion(unsigned char *R);//uns-char[6] Bsplit(B[4},i) = expandida
unsigned char *Bsplit(unsigned char *B, unsigned char indice);//uns-char[2] Bsplit(B[6},i) = [B_i_16, B_i_2345]
unsigned char *Perm(unsigned char *C);//uns-char[6] Perm(C[8])
void printuch(unsigned char uch);//printea un byte
void printucharr(unsigned char *uch, unsigned char size);//printea un arreglo de  bytes
unsigned char *fdes(unsigned char *mmsje, unsigned char *key);


int main(int argc, char *argv[]){
  char **clio = cli_opt(argc, argv);
  if (clio == NULL)
    return 0;
  unsigned char *E, **P, it1, it2, it3, *R;
  unsigned char *rhm, *key, *res;
  S = (unsigned char ***)malloc(sizeof(unsigned char **)*8);
  for (it1 = 0; it1 < 8; it1 += 1){
    S[it1] = (unsigned char **)malloc(sizeof(unsigned char *)*4);
    for (it2 = 0; it2 < 4; it2 += 1){
      S[it1][it2] = (unsigned char *)malloc(sizeof(unsigned char)*16);
    }
  }
  FILE *f = fopen("tablas.dat", "r");
  for (it1 = 0; it1 < 8; it1 += 1)
    for (it2 = 0; it2 < 4; it2 += 1)
      for (it3 = 0; it3 < 16; it3 += 1)
	      fscanf(f, "%hhu", &S[it1][it2][it3]);
    
  R= (unsigned char *)malloc(sizeof(unsigned char)*4);

  rhm=hex2uch(clio[0]), key=hex2uch(clio[1]);
  res=fdes(rhm,key);

  printf("mitad derecha del mensaje: ");
  for (it1=0;it1<4;it1+=1){
    printuch(rhm[it1]);printf(" ");
  }printf("\nllave:                     ");
  for (it1=0;it1<6;it1+=1){
    printuch(key[it1]);printf(" ");
  }printf("\nf(MDM,llave):              ");
  for (it1=0;it1<4;it1+=1){
    printuch(res[it1]);printf(" ");
  }printf("\n");
  return 0;
}

unsigned char *Expansion(unsigned char *R){
  unsigned char *res;
  res = (unsigned char *)calloc(6,sizeof(unsigned char));
  res[0] = (R[3]%2)*128 + (R[0]/32)*16 + ((R[0]%32)/8)*(4+1);
  res[1] = (((R[0]%8)/2)*64 + ((R[0]%2)*2 + (R[1]/128))*(16+4) + ((R[1]%128)/32));
  res[2] = ((R[1]%32)/8)*(64+16) + (R[1]%8)*2 + (R[2]/128);
  res[3] = (R[1]%2)*128 + (R[2]/32)*16 + ((R[2]%32)/8)*(4+1);
  res[4] = (((R[2]%8)/2)*64 + ((R[2]%2)*2+(R[3]/128))*(16+4) + ((R[3]%128)/32));
  res[5] = ((R[3]%32)/8)*(64+16) + (R[3]%8)*2 + (R[0]/128);
  return res;
}

unsigned char *fdes(unsigned char *mmsje, unsigned char *key){
  unsigned char *expandida, *P, *Bsp,B[6], B16[8], B2345[8], C[4], i;
  
  expandida=Expansion(mmsje);
  
  for (i=0; i<6; i+=1){//XOR
    B[i] = (expandida[i]^key[i]);
  }
  for (i=0; i<8; i+=1){//splits
    Bsp = Bsplit(B, i);
    B16[i] = Bsp[0];
    B2345[i] = Bsp[1];
  }
  for (i=0; i<4; i+=1){//sboxes
    C[i] = S[i*2][B16[i*2]][B2345[i*2]]*16 + S[i*2+1][B16[i*2+1]][B2345[i*2+1]];
  }
  P = Perm(C);
  return P;
}

//'110110'01/4=00'110110'
//110110'01'%4=000000'01'
//000000'01'*4=0000'01'00
/*
[(1, 1), (0, 2), (2, 16), (2, 8),
(3, 8), (1, 16), (3, 16), (2, 128)

,(0, 128), (1, 2), (2, 2), (3, 64),
(0, 8), (2, 64), (3, 2), (1, 64)

,(0, 64), (0, 1), (2, 1), (1, 4),
(3, 1), (3, 32), (0, 32), (1, 128)

,(2, 32), (1, 8), (3, 4), (0, 4),
(2, 4), (1, 32), (0, 16), (3, 128)]
*/
unsigned char *Perm(unsigned char *C){
  unsigned char *P= (unsigned char *)calloc(4,(sizeof(unsigned char)));

  P[0]= C[1]%2*128 +  C[0]/2%2*64 + C[2]/16%2*32 + C[2]/8%2*16
        +  
        C[3]/8%2*8 + C[1]/16%2*4 +  C[3]/16%2*2 + C[2]/128%2;
 
  P[1]= C[0]/128%2*128 + C[1]/2%2*64 + C[2]/2%2*32 + C[3]/64%2*16   
        +
        C[0]/8%2*8 + C[2]/64%2*4 + C[3]/2%2*2 + C[1]/64%2;

  P[2]= C[0]/64%2*128 + C[0]%2*64 + C[2]%2*32 + C[1]/4%2*16
        + 
        C[3]%2*8 + C[3]/32%2*4 + C[0]/32%2*2 + C[1]/128%2;

  P[3]= C[2]/32%2*128 + C[1]/8%2*64 + C[3]/4%2*32 + C[0]/4%2*16  
        +
        C[2]/4%2*8 + C[1]/32%2*4 + C[0]/16%2*2 + C[3]/128%2;

  return P;
}

unsigned char *Bsplit(unsigned char *B, unsigned char indice){
  unsigned char *res = (unsigned char *)malloc(sizeof(unsigned char)*2);
  switch (indice){
    case 0:
      res[0] = ((B[0]/128)%2)*2+(B[0]/4)%2;
      res[1] = (B[0]%128)/8;
      break;
    case 1:
      res[0] = ((B[0]/2)%2)*2+(B[1]/16)%2;
      res[1] = (B[0]%2)*8+B[1]/32;
      break;
    case 2:
      res[0] = ((B[1]/8)%2)*2+(B[2]/64)%2;
      res[1] = (B[1]%8)*2+(B[2]/128)%2;
      break;
    case 3:
      res[0] = ((B[2]/32)%2)*2+B[2]%2;
      res[1] = (B[2]%32)/2;
      break;
    case 4:
      res[0] = ((B[3]/128)%2)*2+(B[3]/4)%2;
      res[1] = (B[3]%128)/8;
      break;
    case 5:
      res[0] = ((B[3]/2)%2)*2+(B[4]/16)%2;
      res[1] = (B[3]%2)*8+B[4]/32;
      break;
    case 6:
      res[0] = ((B[4]/8)%2)*2+(B[5]/64)%2;
      res[1] = (B[4]%8)*2+(B[5]/128)%2;
      break;
    case 7:
      res[0] = ((B[5]/32)%2)*2+B[5]%2;
      res[1] = (B[5]%32)/2;
      break;
  };
  return res;
}

char **cli_opt(int argc, char *argv[]){
  int option, is_str=1, it;
  char **ans = (char **)malloc(sizeof(char *)*2), c;
  ans[0] = (char *)calloc(9, sizeof(char));
  ans[1] = (char *)calloc(13, sizeof(char));
  FILE *fp;
  while((option = getopt(argc, argv, ":hsf")) != -1){
    switch(option){
      case 'h':
        usage(argv[0]);
        return NULL;
      case 's':
        is_str = 1;
        break;
      case 'f':
        is_str = 0;
        break;
    }
  }
  if (is_str){
    if (optind < argc)
      for (it=0; it<8; it+=1)
      ans[0][it] = argv[optind][it];
    else
      strcpy(ans[0],"F0AAF0AA");
    if (optind < argc - 1)
      for (it=0; it<12; it+=1)
        ans[1][it] = argv[optind+1][it];
    else
      strcpy(ans[1],"1B02EFFC7072");
  }
  else{
    if (optind >= argc)
      strcpy(ans[0],"F0AAF0AA");
    else{
      fp = fopen(argv[optind],"r");
      if(fp == NULL){
        perror("Error in opening file");
        return NULL;
      }
      for (it=0; it<8; it+=1)
        ans[0][it] = fgetc(fp);
      fclose(fp);
    }
    if (optind >= argc - 1)
      strcpy(ans[1],"1B02EFFC7072");
    else{
      fp = fopen(argv[optind+1],"r");
      if(fp == NULL){
        perror("Error in opening file");
        return NULL;
      }
      for (it=0; it<12; it+=1)
        ans[1][it] = fgetc(fp);
      fclose(fp);
    }
  }
  return ans;
}

int usage(char* progName){
  char *usage_mssg = 
  concat(
  progName," [opciones] [R_(i-1) [K_i]]\n"
  "R_(i-1) mitad derecha del mensaje en iteracion i                 (8 hex)\n"
  "K_i     sub-llave en iteracion i                                (12 hex)\n"
  "Options:                                                                \n"
  "-h                 Imprimir este mensaje de ayuda                       \n"
  "-s                 Input mensaje y llave en string R_(i-1), K_i. default\n"
  "-f                 Input mensaje y llave en el archivo R_(i-1), K_i     \n");
  printf("%s\n", usage_mssg);
  return 0;
}

char* concat(const char *s1, const char *s2){
  char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for thbitse null-terminator
  // in real code you would check for errors in malloc here
  strcpy(result, s1);
  strcat(result, s2);
  return result;
}

unsigned char *hex2uch(char *hex){
  unsigned char len=strlen(hex)/2, it, *uch=(unsigned char *)malloc(sizeof(unsigned char)*len);
  for (it=0; it<len; it+=1){
    uch[it]=16*chvalue(hex[it*2])+chvalue(hex[it*2+1]);
  }
  return uch;
}

unsigned char chvalue(char h){
  if('0'<=h & '9'>=h)
    return (unsigned char)(h - '0');
  return (unsigned char)(tolower(h) - 'a' + 10);
}

void printuch(unsigned char uch){// printeo de byte, bit a bit
  unsigned char it1, *st;
  st = (unsigned char *)malloc(sizeof(unsigned char)*8);
  for (it1=0; it1<8 && uch; it1+=1){
    st[7-it1]=uch%2;
    uch = uch/2;
  }
  for (; it1<8; it1+=1){
    st[7-it1]=0;
  }
  for (it1=0; it1<8; it1+=1){
    printf("%hhu", st[it1]);
  }
}

void printucharr(unsigned char *uch, unsigned char size){
  unsigned char i;
  for (i=0;i<size;i+=1){
  printuch(uch[i]);printf(" ");}printf("\n");
}
