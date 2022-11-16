#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>
#include <limits.h>

typedef struct {
  int palavras[4];
  int endBloco;
  int custo;
  int hits;
  int atualizado;
} BlocoMemoria;

typedef struct{
  int endBloco;
  int endPalavra;
} Endereco;

typedef struct{
  Endereco end1;
  Endereco end2;
  Endereco end3;
  int opcode;
} Instrucao;

/*
 RAM vai ter tamanho 1000
quantidade de palavras por bloco 4
tamanho da cash 1 = 8
cash 2 = 16
tamanho do programa 1000
*/

Instrucao *memoriaInstrucoes;
BlocoMemoria *cache1, *cache2, *cache3, *RAM;
int aux;
int tamanhoC1 = 8, tamanhoC2 = 16, tamanhoC3 = 32;

void montarRAM();
void montarCacheVazia(int, BlocoMemoria*);
void montarInstrucoesProgramaAleatorio();
void maquina();
BlocoMemoria buscarNasMemorias(Endereco);
void testaCache1Cache2(BlocoMemoria);
void testaCache2Cache3(BlocoMemoria);
void testaCache1Cache2Cache3(BlocoMemoria);
void montarInstrucaoGerador();

int main (){
  RAM = malloc (1000 * sizeof (BlocoMemoria));
  for (int i = 0; i < 1000; i++){
    RAM[i].atualizado = 0;
  }
  cache1 = malloc ( tamanhoC1 * sizeof (BlocoMemoria));
  for (int i = 0; i < tamanhoC1; i++){
    cache1[i].atualizado = 0;
  }
  cache2 = malloc (tamanhoC2 * sizeof (BlocoMemoria));
  for (int i = 0; i < tamanhoC2; i++){
    cache2[i].atualizado = 0;
  }
  cache3 = malloc (tamanhoC3 * sizeof (BlocoMemoria));
  for (int i = 0; i < tamanhoC3; i++){
    cache3[i].atualizado = 0;
  }

  montarRAM();
  montarCacheVazia(tamanhoC1, cache1);
  montarCacheVazia(tamanhoC2, cache2);
  montarCacheVazia(tamanhoC3, cache3);
  //montarInstrucoesProgramaAleatorio();
  montarInstrucaoGerador();
  maquina();
  free(RAM);
  free(cache1);
  free(cache2);
  free(cache3);
  return 0;
}

void montarRAM(){
  srand(time(NULL));

  for (int i = 0; i < 1000; i++){
    for (int j = 0; j < 4; j++){
      RAM[i].palavras[j] = rand() % 1000;
    }
    RAM[i].endBloco = i;
  }
}

void montarCacheVazia(int tamanho, BlocoMemoria* cache){
  for (int i = 0; i < tamanho; i++){
    for (int j = 0; j < 4; j++){
      cache[i].palavras[j] = 0;
    }
    cache[i].endBloco = -1;
  }
}

void montarInstrucoesProgramaAleatorio(){
  memoriaInstrucoes = malloc (10000 * sizeof (Instrucao));
  srand(time(NULL));

  for (int i = 0 ; i < 9999; i++){

    memoriaInstrucoes[i].opcode = rand () % 3;

    memoriaInstrucoes[i].end1.endBloco = rand () % 1000;
    memoriaInstrucoes[i].end1.endPalavra = rand () % 4;

    memoriaInstrucoes[i].end2.endBloco = rand () % 1000;
    memoriaInstrucoes[i].end2.endPalavra = rand () % 4;

    memoriaInstrucoes[i].end3.endBloco = rand () % 1000;
    memoriaInstrucoes[i].end3.endPalavra = rand () % 4;
  }

  memoriaInstrucoes[9999].opcode = -1;
  maquina();
}
BlocoMemoria buscarNasMemorias(Endereco endB){

  int custo = 0, confere = 0;
  BlocoMemoria cacheX;

  for (int i = 0; i < tamanhoC1; i++){
    if (cache1[i].endBloco == endB.endBloco){
      custo += 10;
      cache1[i].custo = custo;
      cache1[i].hits = 1;
      cacheX = cache1[i];
      aux = i;
      confere = 1;
      break;
    }
  }
  if (confere == 1){
    return cacheX;
  }
  else{
    for (int i = 0; i < tamanhoC2; i++){
      if (cache2[i].endBloco == endB.endBloco){
      custo += 110;
      cache2[i].custo = custo;
      cache2[i].hits = 2;
      cacheX = cache2[i];
      aux = i;
      confere = 2;
      break;
      }
    }
    if (confere == 2){
      testaCache1Cache2(cacheX);
      return cacheX;
    }
    else{
      for (int i = 0; i < tamanhoC3; i++){
        if (cache3[i].endBloco == endB.endBloco){
          custo += 1110;
          cache3[i].custo = custo;
          cache3[i].hits = 3;
          cacheX = cache3[i];
          aux = i;
          confere = 3;
          break;
        }
      }
      if (confere == 3){
        testaCache2Cache3(cacheX);
        testaCache1Cache2(cacheX);
        return cacheX;
      }
    else{
      custo += 101110;
      RAM[endB.endBloco].custo = custo;
      RAM[endB.endBloco].hits = 4;
      cacheX = RAM[endB.endBloco];
      testaCache1Cache2Cache3(cacheX);
      return cacheX;
      }
    }
  }
}

void testaCache1Cache2(BlocoMemoria cacheX){
  for (int i = 0; i < tamanhoC1; i++){
    if (cacheX.atualizado >= cache1[i].atualizado){
      cache2[aux] = cache1[i];
      cache1[i] = cacheX;
      aux = i;
      break;
    }
  }
}
void testaCache2Cache3(BlocoMemoria cacheX){
  for (int i = 0; i < tamanhoC2; i++){
    if (cacheX.atualizado >= cache2[i].atualizado){
      cache3[aux] = cache2[i];
      cache2[i] = cacheX;
      aux = i;
      break;
    }
  }
}
void testaCache1Cache2Cache3(BlocoMemoria cacheX){
  int aux1;
  int menor = INT_MAX;
      for (int i = 0; i < tamanhoC3; i++){
        if (cache3[i].atualizado < menor){
          menor = cache3[i].atualizado;
        }
      }
      for (int i = 0; i < tamanhoC3; i++){
        if (cache3[i].atualizado == menor){
          cache3[i] = cacheX;
          aux1 = i;
          break;
        }
      }
      menor = INT_MAX;
      for (int i = 0; i < tamanhoC2; i++){
        if (cache2[i].atualizado < menor){
          menor = cache2[i].atualizado;
        }
      }
      for (int i = 0; i < tamanhoC2; i++){
        if (cache2[i].atualizado == menor){
          cache3[aux1] = cache2[i];
          cache2[i] = cacheX;
          aux1 = i;
          break;
        }
      }
      menor = INT_MAX;
      for (int i = 0; i < tamanhoC1; i++){
        if (cache1[i].atualizado < menor){
          menor = cache1[i].atualizado;
        }
      }
      for (int i = 0; i < tamanhoC1; i++){
        if (cache1[i].atualizado == menor){
          cache2[aux1] = cache1[i];
          cache1[i] = cacheX;
          aux = i;
          break;
        }
      }
    
}

void maquina(){
  int PC = 0, opcode = 3, custo = 0, missC1 = 0, hitC1 = 0, missC2 = 0, missC3 = 0, hitC2 = 0, hitC3 = 0;
  BlocoMemoria dadoMemoriaEnd1;
  BlocoMemoria dadoMemoriaEnd2;
  BlocoMemoria dadoMemoriaEnd3;

  while (opcode != -1){
    Instrucao umaInstrucao = memoriaInstrucoes[PC];
    opcode = umaInstrucao.opcode;

    if (opcode != -1){

     dadoMemoriaEnd1 = buscarNasMemorias(umaInstrucao.end1);
     dadoMemoriaEnd2 = buscarNasMemorias(umaInstrucao.end2);
     dadoMemoriaEnd3 = buscarNasMemorias(umaInstrucao.end3);
      custo += dadoMemoriaEnd1.custo;
      custo += dadoMemoriaEnd2.custo;
      custo += dadoMemoriaEnd3.custo;

      if ( dadoMemoriaEnd1.hits == 1){
        hitC1++;
      }
      else if ( dadoMemoriaEnd1.hits == 2){
        missC1++;
        hitC2++;
      }
      else if( dadoMemoriaEnd1.hits == 3){
        missC1++;
        missC2++;
        hitC3++;
      }
      else if ( dadoMemoriaEnd1.hits == 4){
        missC1++;
        missC2++;
        missC3++;
      }

      if (dadoMemoriaEnd2.hits == 1){
        hitC1++;
      }
      else if (dadoMemoriaEnd2.hits == 2){
        missC1++;
        hitC2++;
      }
      else if (dadoMemoriaEnd2.hits == 3){
        missC1++;
        missC2++;
        hitC3++;
      }
      else if(dadoMemoriaEnd2.hits == 4){
        missC1++;
        missC2++;
        missC3++;
      }
      if ( dadoMemoriaEnd3.hits == 1){
        hitC1++;
      }
      else if (dadoMemoriaEnd3.hits == 2){
        missC1++;
        hitC2++;
      }
      else if (dadoMemoriaEnd3.hits == 3){
        missC1++;
        missC2++;
        hitC3++;
      }
      else if (dadoMemoriaEnd3.hits == 4){
        missC1++;
        missC2++;
        missC3++;
      }
      printf("Custo até o momento do programa em execucao: %d\n", custo);
      printf ("Hits e Misses até o momento - C1 hit: %d | C1 miss: %d | C2 hit: %d | C2 miss %d | C3 hit %d | C3 miss: %d\n ", hitC1, missC1, hitC2, missC2, hitC3, missC3);

      switch (opcode){
        case 0:{
          printf ("Nao há demanda por levar dados externos para as memorias.\n");
          break;
        }
        case 1:{
          int soma = dadoMemoriaEnd1.palavras[umaInstrucao.end1.endPalavra] + dadoMemoriaEnd2.palavras[umaInstrucao.end2.endPalavra];
          dadoMemoriaEnd3.palavras[umaInstrucao.end3.endPalavra] = soma;
          printf ("Somando: %d\n", soma);
          cache1[aux].atualizado++;
          break;
        }
        case 2:{
          int sub = dadoMemoriaEnd1.palavras[umaInstrucao.end1.endPalavra] - dadoMemoriaEnd2.palavras[umaInstrucao.end2.endPalavra];
          dadoMemoriaEnd3.palavras[umaInstrucao.end3.endPalavra] = sub;
          printf ("Subtraindo: %d\n", sub);
          cache1[aux].atualizado++;
          break;
        }
      }
      PC++;
      printf("PC: %d\n", PC);
    }
  }
  printf ("Custo total do programa: %d\n", custo);
  printf ("Hits e Misses - C1 hit: %d | C1 miss: %d | C2 hit: %d | C2 miss: %d | C3 hit: %d | C3 miss: %d\n", hitC1, missC1, hitC2, missC2, hitC3, missC3);
}

void montarInstrucaoGerador(){
  FILE *arquivo = fopen("instrucoes.txt", "r");

  memoriaInstrucoes = malloc (10000 * sizeof (Instrucao));
  for (int i = 0 ; i < 10000; i++){
    fscanf(arquivo, "%d ", &memoriaInstrucoes[i].opcode);
    fscanf(arquivo, "%d %d ", &memoriaInstrucoes[i].end1.endBloco, &memoriaInstrucoes[i].end1.endPalavra);
    fscanf(arquivo, "%d %d ", &memoriaInstrucoes[i].end2.endBloco, &memoriaInstrucoes[i].end2.endPalavra);
    fscanf(arquivo, "%d %d", &memoriaInstrucoes[i].end3.endBloco, &memoriaInstrucoes[i].end3.endPalavra);
    fscanf(arquivo, " ");

  }
  memoriaInstrucoes[9999].opcode = -1;
  fclose(arquivo);
}
