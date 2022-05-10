#include <iostream>
#include "mapa.cpp"
#include <fstream>
Mapa *mapa;
const std::string ARQ_CONFIG = "config.cfg";
const std::string AJUDA = "ajuda.txt";
const std::string RECORDES = "recordes.txt";

void ImprimirMapa(){
  for(std::vector < Celula > linha : mapa->retornaCampoImpressao()){
    for(Celula coluna : linha){
      
      if(coluna.getBandeira())
        std::cout << " |>";
      else if(coluna.isBomba())
        std::cout << " B ";
      else if(coluna.isVisivel())
        std::cout << " " << coluna.getQtdBombas() << " ";       
      else
        std::cout << " X ";
    }
    std::cout << "\n";
  }
}
void exibirAjudaRecordes(bool ajuda){
  std::ifstream arquivo;
  arquivo.open (ajuda ? AJUDA.c_str() : RECORDES.c_str(), std::ifstream::in);
  if(arquivo.is_open()){
   while (! arquivo.eof() ) {
   std::string linha;
   getline(arquivo,linha);
   std::cout << linha << std::endl;
   }    
  } 
}
void gravarDificuldade(Dificuldade dif){
  std::ofstream arquivo;
  arquivo.open (ARQ_CONFIG.c_str(), std::ifstream::out);
  if(arquivo.is_open()){
    switch(dif){
      case Dificuldade::iniciante: arquivo << 'b'; break;
      case Dificuldade::intermediario: arquivo << 'i'; break;
      case Dificuldade::avancado: arquivo << 'a'; break;
    }
    arquivo.close();
  }
}
void inicializarMapa(){
  std::ifstream file;
  file.open (ARQ_CONFIG.c_str(), std::ifstream::in);  
  if(file.is_open()){
    char c;
    file >> c;
    switch(c){
      case 'b': mapa = new Mapa(*(new Configuracao(Dificuldade::iniciante))); break;
      case 'i': mapa = new Mapa(*(new Configuracao(Dificuldade::intermediario))); break;
      case 'a': mapa = new Mapa(*(new Configuracao(Dificuldade::avancado))); break;
      default:  mapa = new Mapa(*(new Configuracao(Dificuldade::iniciante)));
    }
  file.close();  
  mapa->inicializaMapa();
  }
}
bool lerArgumentos(int argc, char ** argv){
  if(argc > 1){
    std::string argumento = argv[1];
    if(argumento == "-h" || argumento == "--help"){
      exibirAjudaRecordes(true);
      return false;
    } else if(argumento == "-d" || argumento == "--difficulty"){
      if(argc > 2){
        std::string dif = argv[2];
        if(dif == "-b" || dif == "--beginner"){
          gravarDificuldade(Dificuldade::iniciante);
          return true;
        } else if(dif == "-i" || dif == "--intermediary"){
          gravarDificuldade(Dificuldade::intermediario);
          return true;
        } else if(dif == "-a" || dif == "--advanced"){
          gravarDificuldade(Dificuldade::avancado);
          return true;
        } else {
          std::cout << "Argumento desconhecido para dificuldade: " << argumento << std::endl;
          exibirAjudaRecordes(true);
          return false;
        }
      } else {
        std::cout << "É esperado uma seleção de dificuldade: " << argv[1] << std::endl;
        exibirAjudaRecordes(true);
        return false;
        
      }
    } else {
      std::cout << "Argumento desconhecido: " << argv[1] << std::endl;
      exibirAjudaRecordes(true);
      return false;
      
    }
  } else {
    return true;
  }
}
int main(int argc, char ** argv){
  if(lerArgumentos(argc,argv)){
    inicializarMapa();
    while(true){
    ImprimirMapa();
    char acao;
    int x = 0; int y = 0;
    std::cout << "Informe a ação que deseja tomar: ";
    std::cin >> acao;
    std::cout << "Informe o número da linha: ";
    std::cin >> y;
    std::cout << "Informe o número da coluna: ";
    std::cin >> x;   
    switch(acao){
      case 'B': mapa->alternarBandeira(x,y); break;
      case 'R': mapa->RevelarPosicao(x,y); break;
      default: std::cout << "Ação inválida! Tente ações válidas: 'B' para incluir bandeira e 'R' para revelar a posição\n";
    }
    std::string resultadoAcao = mapa->verificarEstadoJogo();
    if(resultadoAcao != ""){
      ImprimirMapa();
      std::cout << resultadoAcao;
      return 0;
    }
  }      
  }
  return 0;
}
  
