#include <iostream>
#include "recordeDao.hpp"
int RetornaValorDificuldade(char dificuldade){
  switch(dificuldade){
    case 'b': return 0;
    case 'i': return 1;
    default: return 2;    
  }

}
bool comparaRecorde(Recorde r1, Recorde r2){
  if(r1.dificuldade == r2.dificuldade)
    return r1.RecordeTempo < r2.RecordeTempo;
 return RetornaValorDificuldade(r1.dificuldade) > RetornaValorDificuldade(r2.dificuldade);
  
}