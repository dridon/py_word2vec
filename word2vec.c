/*
 * Author: Faiz Khan 
 * Based on news vector tutorial code provided by Google at https://code.google.com/p/word2vec/
 *
 */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>


void initialize();
void print_vector();

const long long max_size = 2000;         // max length of strings
const long long N = 40;                  // number of closest words that will be shown
const long long max_w = 50;              // max length of vocabulary entries

// Container type for word vectors
typedef struct {
  long long words; 
  long long size; 
  char *vocab;
  char **vocab2;
  float *vectors;

} word_vectors;


float *buff; // buffer used to return to python
word_vectors vectors; // word vectors table 

int main(int argc, char **argv){
  initialize();
  return 0;
}


// initializes the word vectors from the file, the file containing the data is required
void initialize(char *fname){
  FILE *f;
  char file_name[max_size];
  float len;
  long long a, b;

  strcpy(file_name, fname);
  printf("%s\n", file_name);
  f = fopen(file_name, "rb");
  if (f == NULL) {
    printf("Input file not found\n");
    return;
  }

  // get pre-trained vector parameters 
  fscanf(f, "%lld ", &vectors.words);
  fscanf(f, "%lld", &vectors.size);
  vectors.vocab2 = (char **)malloc((long long)vectors.words*sizeof(char*));
  for(a = 0; a < vectors.words; ++a) vectors.vocab2[a] = (char *) malloc(sizeof(char)*max_w);
  vectors.vectors = (float *)malloc(vectors.words * vectors.size * sizeof(float));
  if (vectors.vectors == NULL) {
    printf("Cannot allocate memory: %lld MB    %lld  %lld\n", (long long)vectors.words * vectors.size * sizeof(float) / 1048576, vectors.words, vectors.size);
    return;
  }

  printf("loading the vectors\n");
  // load the words and the vectors
  for (b = 0; b < vectors.words; b++) {
    a = 0;
    while (1) {
      vectors.vocab2[b][a] = fgetc(f);
      if (feof(f) || (vectors.vocab2[b][a] == ' ')) break;
      if ((a < max_w) && (vectors.vocab2[b][a] != '\n')) a++;
    }
    // vectors.vocab[b * max_w + a] = 0;
    vectors.vocab2[b][a] = 0;
    for (a = 0; a < vectors.size; a++) fread(&vectors.vectors[a + b * vectors.size], sizeof(float), 1, f);
    len = 0;
    
    // normalize the vectors to unit vectors 
    for (a = 0; a < vectors.size; a++) len += vectors.vectors[a + b *vectors.size] * vectors.vectors[a + b * vectors.size];
    len = sqrt(len);
    for (a = 0; a < vectors.size; a++) vectors.vectors[a + b * vectors.size] /= len;

    if(b%100000 == 0) printf("done %lld words ...\n", b);
  }
  fclose(f);
  buff= (float *) malloc(sizeof(float)*vectors.size);
}

// get the word vocabulary
char **get_vocab(){
    return vectors.vocab2;
}

// get the word count 
long long word_count(){
    return vectors.words;
}

// get the size of the vectors
long long vector_size(){ 
  return vectors.size; 
}

// get the vector at a specific index
float *get_vector(long long index){
    long long a = 0;
    if(index > vectors.words || index < 0) return NULL;

    for (a = 0; a < vectors.size; a++){
      buff[a] = vectors.vectors[a + index * vectors.size];
    }
    return buff;
}

// print a vector at the specific index
void print_vector(int index){
    int a; 
    printf("[");
    for (a = 0; a < vectors.size; a++) {
      printf("%lf, ", vectors.vectors[a + index* vectors.size]);
    }
    printf("]\n");
}
