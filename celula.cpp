class Celula {
  bool visivel = false;
  bool is_Bomba = false;
  bool bandeira = false;
  unsigned int qtdBombasAdj = 0;
  
  public:             
      void tornarBomba(){
        is_Bomba = true;
      };
      void incrementaBombaAdj(){
        qtdBombasAdj++;
      };
      int getQtdBombas(){
        return qtdBombasAdj;
      };

      bool isVisivel(){
        return visivel;
      };
      bool isBomba(){
        return is_Bomba;
      };
      bool getBandeira(){
        return bandeira;
      }
      void alternarBandeira(){
        bandeira = !bandeira;
      }
      bool Revelar(){
        visivel = true;
        return is_Bomba;
      }
      Celula(){
        
      }
};