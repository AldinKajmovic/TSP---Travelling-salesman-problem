
#include <iostream>
#include "tezinski_graf.h"
using namespace std;


int main() {
/*ispravi kada treba spojiti pocetni i krajnji cvor logiku*/
//provjeri i stepene, nema logike da su cvorovi predstavljeni kao brojevi wtf?
    TezinskiGraf G(29);
    G.dodajGradove();
    //auto ruta = G.TSPVerzijaJedan();
   auto ruta = G.TSPVerzijaDva();
    //auto ruta = G.TSPVerzijaTri();

   for (auto bridge : ruta.second)  cout << bridge.first << " -> " << bridge.second << endl;
cout<<"Ruta je : "<<ruta.first<<endl;




}
