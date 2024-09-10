#ifndef TEZINSKI_GRAF_H
#define TEZINSKI_GRAF_H
#include <algorithm>
#include <list>
#include <string>
#include <utility>
#include <vector>

using namespace std;

class DisjunktniSkupoviLista {
 private:
  vector<list<int>> skupovi;

 public:
  DisjunktniSkupoviLista(int n) {
    for (int i = 0; i < n; ++i) {
      skupovi.push_back({i});
    }
  }

  int Nadji(int element) {
    for (int i = 0; i < skupovi.size(); ++i) {
      auto it = find(skupovi[i].begin(), skupovi[i].end(), element);
      if (it != skupovi[i].end()) {
        return i;
      }
    }
    return -1;
  }

  void Spoji(int skup1, int skup2) {
    if (skup1 != skup2) {
      skupovi[skup1].merge(skupovi[skup2]);
      skupovi.erase(skupovi.begin() + skup2);
    }
  }
};

class DisjunktniSkupovi {
  int n;
  vector<int> roditelj;

 public:
  DisjunktniSkupovi(int broj_cvorova) : n(broj_cvorova) {
    for (int i = 0; i < n; i++)
        roditelj.push_back(i);
  }

  int Nadji(int cvor) {
    if (roditelj[cvor] == cvor) return cvor;
    roditelj[cvor] = Nadji(roditelj[cvor]);
    return roditelj[cvor];
  }

  void Spoji(int i, int j) { roditelj[roditelj[i]] = roditelj[j]; }
};

class TezinskiGraf {
  int n;
  vector<vector<int>> matrica_susjedstva;
  vector<pair<double, double>> paroviGradova;

 public:

  TezinskiGraf(int broj_cvorova);
  void citajIzFajla(const string &imeFajla);
  int udaljenostIzmedjuTacaka(pair<double, double> prva, pair<double, double> druga);
  void dodajGradove();
  int stepenCvora(int cvor, const vector<pair<int, int>> &rutaGradova);
  bool sadrziCiklus(const vector<pair<int, int>> &rutaGradova, const DisjunktniSkupovi &ds);
  bool imaCiklusDFS(int trenutni, int roditelj, vector<int> &posjecen, vector<vector<int>> &matricaSusjedstva);
  pair<double, vector<pair<int, int>>> TSPVerzijaJedan();
  pair<double, vector<pair<int, int>>> TSPVerzijaDva();
  pair<double, vector<pair<int, int>>> TSPVerzijaTri();

};

#endif  // TEZINSKI_GRAF_H
