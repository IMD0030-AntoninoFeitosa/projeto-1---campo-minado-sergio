#include <iostream>
#include "celula.cpp"
#include "configuracao.cpp"
#include <vector>
#include <random>

class Mapa {
  std::vector < std::vector < Celula >> campo;
  bool fimJogo {false};
  bool mapaPopulado {false};
  Configuracao configuracao;
  int qtdReveladas;
  const int adjacentes[8][2] {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};



  public:
    void inicializaMapa() {

      for (int y = 0; y < configuracao.qtdLinhas; y++) {
        std::vector < Celula > linha;
        for (int x = 0; x < configuracao.qtdColunas; x++) {
          linha.push_back(*(new Celula()));
        }
        campo.push_back(linha);
      }
  
    }
  Configuracao RetornaConfiguracao(){
    return configuracao;
  }
  void popularMapa(unsigned int x, unsigned int y){
    int qtdBombasGeradas = 0;
    std::random_device radomDevice;
    std::default_random_engine eng(radomDevice());
    
    if(configuracao.dif == Dificuldade::avancado){
      int xSorteado = 0; int ySorteado = 0;
      std::uniform_int_distribution<int> gerador(-1, 1);      
      do{
        xSorteado = gerador(eng);
        ySorteado = gerador(eng);
      }while(xSorteado == 0 && ySorteado == 0);
      campo[y+ySorteado][x+xSorteado].tornarBomba();  
      qtdBombasGeradas++;
    }
    
    std::uniform_int_distribution<int> geradorColunas(0, configuracao.qtdColunas - 1);   
    std::uniform_int_distribution<int> gradorLinhas(0, configuracao.qtdColunas - 1); 
    
    do{
      int xSorteado = geradorColunas(eng);
      int ySorteado = gradorLinhas(eng);
      if(!campo[ySorteado][xSorteado].isBomba() && !aoRedorIntermediario(x,y,xSorteado,ySorteado)){
        campo[ySorteado][xSorteado].tornarBomba();
        
        qtdBombasGeradas++;
      }
    }while(qtdBombasGeradas < configuracao.qtdBombas);

    incluirNumeros();
    mapaPopulado = true;
  }
  bool aoRedorIntermediario(int x, int y,int xSorteado, int ySorteado){
        if(configuracao.dif != Dificuldade::intermediario)
          return false;
  if(x == xSorteado && y == ySorteado)
    return true;
        for (int i = 0; i < 8; i++) {
            int xR = adjacentes[i][0] + x;
            int yR = adjacentes[i][1] + y;          
          if(posicaoValida(xR,yR) && xR == xSorteado && yR == ySorteado)
            return true;
        }
        return false;
  }
  void incluirNumeros(){
      for (int y = 0; y < configuracao.qtdLinhas; y++) {
        for (int x = 0; x < configuracao.qtdColunas; x++) {
          for (int i = 0; i < 8; i++) {
            int xR = adjacentes[i][0] + x;
            int yR = adjacentes[i][1] + y;
             if(posicaoValida(xR,yR) && campo[yR][xR].isBomba())
              campo[y][x].incrementaBombaAdj();                     
          }          
        }
      }        
  }
  std::vector < std::vector < Celula >> retornaCampoImpressao() {
    return this -> campo;
  }
  int qtdBandeirasAoRedor(unsigned int x, unsigned int y){
        int qtdBandeiras = 0;
        for (int i = 0; i < 8; i++) {
          int xR = x + adjacentes[i][0];
          int yR = y + adjacentes[i][1];
          if(posicaoValida(xR,yR)){
            if(campo[yR][xR].getBandeira())
              qtdBandeiras++;
          }           
        }
      return qtdBandeiras;
  }
  void RevelarNumeroVisivel(unsigned int x, unsigned int y){
          if(posicaoValida(x,y) && campo[y][x].getQtdBombas() == qtdBandeirasAoRedor(x,y)){
              for (int i = 0; i < 8; i++) {
                int xR = x + adjacentes[i][0];
                int yR = y + adjacentes[i][1];
                if(posicaoValida(xR,yR) && !campo[yR][xR].getBandeira()){
                  if(campo[yR][xR].Revelar()){
                    fimJogo = true;
                    return;
                  };
                }           
            }            
          }     
  }
  void RevelarPosicao(unsigned int x, unsigned int y) {
    if(!mapaPopulado)
      popularMapa(x,y);
    if(campo[y][x].isVisivel() && campo[y][x].getQtdBombas() > 0)
      RevelarNumeroVisivel(x,y);
    else if (posicaoValida(x, y) && !campo[y][x].isVisivel() && !fimJogo && !campo[y][x].getBandeira()) {
      if (campo[y][x].Revelar()) {
        fimJogo = true;
        return;
      }
      if (campo[y][x].getQtdBombas() == 0) {


        for (int i = 0; i < 8; i++) {
          int xR = x + adjacentes[i][0];
          int yR = y + adjacentes[i][1];
          if(posicaoValida(xR,yR))
            RevelarPosicao(xR,yR);
        }
      }
    }
  }

  std::string verificarEstadoJogo(){
    if(fimJogo)
     return "Você revelou uma bomba! Fim de jogo!";
    int qtdReveladas = 0;
    for(std::vector < Celula > linha : campo){
      for(Celula coluna : linha){
        if(coluna.isVisivel())
          qtdReveladas++;
      }
    }
    if(qtdReveladas == (configuracao.qtdLinhas * configuracao.qtdColunas - configuracao.qtdBombas)){
     return "Parabéns! Você revelou todas as posições sem revelar bombas";
    }
   return "";
  }
  
  bool posicaoValida(int x, int y) {
    return x < configuracao.qtdColunas && y < configuracao.qtdLinhas && x >= 0 && y >= 0;
  }

  void alternarBandeira(int x, int y){
    campo[y][x].alternarBandeira();
  }
  
  Mapa(Configuracao configuracao) {
    this->configuracao = configuracao;
  }
};