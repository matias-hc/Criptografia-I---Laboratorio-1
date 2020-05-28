#include <stdio.h>
#include <stdlib.h>

void printuc(unsigned char uc)
{
  unsigned char it1, *st;
  st = (unsigned char *)malloc(sizeof(unsigned char)*8);
  for (it1=0; it1<8 && uc; it1+=1)
  {
    st[7-it1]=uc%2;
    uc = uc/2;
  }
  for (; it1<8; it1+=1)
  {
    st[7-it1]=0;
  }
  for (it1=0; it1<8; it1+=1)
  {
    printf("%d", st[it1]);
  }
}
unsigned char pow2(unsigned char e){
  unsigned char res=1;
  while(e){
    res*=2;
    e-=1;
  }
  return res;
}

unsigned char *Expansion(unsigned char *R);

int main(int argc, char *argv[]){
  unsigned char ***S, *E, **P, it1, it2, it3, *R;
  S = (unsigned char ***)malloc(sizeof(unsigned char **)*8);
  for (it1 = 0; it1 < 8; it1 += 1){
    S[it1] = (unsigned char **)malloc(sizeof(unsigned char *)*8);
    for (it2 = 0; it2 < 8; it2 += 1)
      S[it1][it2] = (unsigned char *)malloc(sizeof(unsigned char));
  }
  //printf("%d\n", it3);
  FILE *f = fopen("tablas.dat", "r");
  for (it1 = 0; it1 < 8; it1 += 1)
    for (it2 = 0; it2 < 8; it2 += 1)
      for (it3 = 0; it3 < 8; it3 += 1)
	fscanf(f, "%d", &S[it1][it2][it3]);
    
  R= (unsigned char *)malloc(sizeof(unsigned char)*4);
  for(it1=0 ; it1<32 ; it1+=1){
    R[0]=0;
    R[1]=0;
    R[2]=0;
    R[3]=0;
    R[it1/8]=pow2(it1%8);
    E=Expansion(R);
    printf("\n");
    for(it2=0; it2<4 ; it2+=1){
      printuc(R[it2]);
      printf("\t");
    }
    printf("\n");
    for(it2=0; it2<6 ; it2+=1){
      printuc(E[it2]);
      printf("\t");
    }
    printf("\n");
  }
  printf("\n");
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
  unsigned char *expandida=Expansion(mmsje);
  //B=XOR(expandida,key);
  //for(i=0;i<8;i++)
  //  C[i]=S[i](B[i])
  //P(C)
}

//'110110'01/4=00'110110'
//110110'01'%4=000000'01'
//000000'01'*4=0000'01'00

/*
char[6] E(char[4] R)
  32 | 01 02 03 | 04 05 + 04 05 => res[0] = (R[3]%2)*128 + (R[0]/32)*16 + ((R[0]%32)/8)*(4+1)
  06 07 | 08 09 + 08 09 | 10 11 => res[1] = ((R[0]%8/2)*64 + ((R[0]%2)*2+(R[1]/128))*(16+4) + ((R[1]%128)/32)
  12 13 + 12 13 | 14 15 16 | 17 => res[2] = ((R[1]%32)/8)*(64+16) + (R[1]/8)*2 + (R[2]/128)
  16 | 17 18 19 | 20 21 + 20 21 => res[3] = (R[1]%2)*128 + (R[2]/32)*16 + ((R[2]%32)/8)*(4+1)
  22 23 | 24 25 + 24 25 | 26 27 => res[4] = ((R[2]%8/2)*64 + ((R[2]%2)*2+(R[3]/128))*(16+4) + ((R[3]%128)/32)
  28 29 + 28 29 | 30 31 32 | 01 => res[5] = ((R[3]%32)/8)*(64+16) + (R[3]/8)*2 + (R[0]/128)
  return res

  0 0 32 1  2  3  4  5  => res[0] = (R[3]%2)*32 + R[0]/8
  0 0 4  5  6  7  8  9  => res[1] = (R[0]%32)*2 + R[1]/128
  0 0 8  9  10 11 12 13 => res[2] = (R[0]%2)*32 + R[1]/8
  0 0 12 13 14 15 16 17 => res[3] = (R[1]%32)*2 + R[2]/128
  0 0 16 17 18 19 20 21 => res[4] = (R[1]%2)*32 + R[2]/8
  0 0 20 21 22 23 24 25 => res[5] = (R[2]%32)*2 + R[3]/128
  0 0 24 25 26 27 28 29 => res[6] = (R[2]%2)*32 + R[3]/8
  0 0 28 29 30 31 32 1  => res[7] = (R[3]%32)*2 + R[0]/128


f(char[4] R_i-1, char[6] K_i)
 char* ER = E(R_i-1)
 B = xor(ER, k_i)
 char Bi[8] = cada_6_bits(B)
 for i en rango 8:
  Sbox(i, Bi[i]) 
*/
