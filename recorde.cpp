#include <iostream>
#include "vector"
#include <fstream>
#include <algorithm>
#include "recordeDao.hpp"

class Recordes{
  std::vector<Recorde> lstRecordes;
  const std::string nomeArquivo{"recordes.txt"};
  public:
  void escreve_recordes_arquivos() {

  std::ofstream arquivo;
  arquivo.open(nomeArquivo.c_str());
  if (arquivo.is_open()) {
    arquivo << lstRecordes.size() << std::endl;
    for (Recorde rec : lstRecordes) {
      arquivo << rec.NomeJogador << std::endl;
      arquivo << rec.RecordeTempo << std::endl;
      arquivo << rec.dificuldade << std::endl;
    }
    arquivo.close();
  }    
}
std::vector<Recorde> ler_recordes() {

  std::ifstream arquivo;
  arquivo.open(nomeArquivo.c_str());
  lstRecordes.clear();
  if (arquivo.is_open()) {
    arquivo.clear();
    int size;
    arquivo >> size;
    arquivo.ignore();
    for (int i = 0; i < size; i++) {
      Recorde rec;
      std::getline(arquivo, rec.NomeJogador);
      std:: string aux;
      std::getline(arquivo, aux);
      rec.RecordeTempo = std::stod(aux);  
      arquivo >> rec.dificuldade;
      arquivo.ignore();
      lstRecordes.push_back(rec);
    }
    arquivo.close();
  }
  sort();
  return  lstRecordes;
}

void adicionaRecorde(Recorde rec){
  lstRecordes.push_back(rec);
}

void sort() {
  std::sort(lstRecordes.begin(), lstRecordes.end(), comparaRecorde);
}
};
