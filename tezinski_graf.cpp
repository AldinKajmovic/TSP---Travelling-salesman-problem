#include <algorithm>
#include <climits>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include "tezinski_graf.h"
using namespace std;

TezinskiGraf::TezinskiGraf(int broj_cvorova) : n(broj_cvorova) {
  matrica_susjedstva.resize(n, vector<int>(n, 0));
  citajIzFajla("wi29.tsp");
}

int TezinskiGraf::udaljenostIzmedjuTacaka(pair<double, double> prva, pair<double, double> druga) {
  return sqrt((prva.first - druga.first) * (prva.first - druga.first) + (prva.second - druga.second) * (prva.second - druga.second));
}

void TezinskiGraf::citajIzFajla(const string& imeFajla) {
  ifstream fajl(imeFajla);
  if (!fajl.is_open()) {
    cerr << "Fajl se ne moze otvoriti: " << imeFajla << endl;
    return;
  }

  string red;
  while (getline(fajl, red)) {
    istringstream iss(red);
    double id;
    double x, y;
    if (iss >> id >> x >> y) {
      pair<double, double> par = {x, y};
      paroviGradova.push_back(par);
    }
  }
}

void TezinskiGraf::dodajGradove() {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i != j) {
        double udaljenost =
            udaljenostIzmedjuTacaka(paroviGradova[i], paroviGradova[j]);
        matrica_susjedstva[i][j] = udaljenost;
        matrica_susjedstva[j][i] = udaljenost;
      } else {
        matrica_susjedstva[i][j] = 0;
      }
    }
  }
}

int TezinskiGraf::stepenCvora(int cvor, const vector<pair<int, int>>& rutaGradova) {
  int stepen = 0;
  for (auto par : rutaGradova) {
    if (par.first == cvor || par.second == cvor) {
      stepen++;
    }
  }
  return stepen;
}

bool TezinskiGraf::sadrziCiklus(const vector<pair<int, int>>& rutaGradova, const DisjunktniSkupovi& ds) {
  DisjunktniSkupovi dsKopija = ds;
  for (auto par : rutaGradova) {
    int i = par.first;
    int j = par.second;
    if (dsKopija.Nadji(i) == dsKopija.Nadji(j)) {
      return true;
    }
    dsKopija.Spoji(i, j);
  }
  return false;
}

bool TezinskiGraf::imaCiklusDFS(int trenutni, int roditelj, vector<int>& posjecen, vector<vector<int>>& matricaSusjedstva) {
  posjecen[trenutni] = 1;
  for (int susjed = 0; susjed < n; susjed++) {
    if (matricaSusjedstva[trenutni][susjed] > 0) {
      if (!posjecen[susjed]) {
        if (imaCiklusDFS(susjed, trenutni, posjecen, matricaSusjedstva)) {
          return true;
        }
      } else if (susjed != roditelj) {
        return true;
      }
    }
  }
  posjecen[trenutni] = 0;
  return false;
}

pair<double, vector<pair<int, int>>> TezinskiGraf::TSPVerzijaJedan() {
  pair<double, vector<pair<int, int>>> TSP{0, vector<pair<int, int>>()};
  vector<pair<double, pair<int, int>>> rutaGradova;
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (i != j) {
        double udaljenost = matrica_susjedstva[i][j];
        rutaGradova.push_back({udaljenost, {i, j}});
      }
    }
  }

  sort(rutaGradova.begin(), rutaGradova.end());

  vector<vector<int>> matricaSusjedstva(n, vector<int>(n, 0));

  for (auto par : rutaGradova) {
    int i = par.second.first;
    int j = par.second.second;
    if ((stepenCvora(i, TSP.second) < 2) && (stepenCvora(j, TSP.second) < 2)) {
      matricaSusjedstva[i][j] = 1;
      matricaSusjedstva[j][i] = 1;

      vector<int> posjecen(n, 0);
      if (!imaCiklusDFS(i, i, posjecen, matricaSusjedstva) && !imaCiklusDFS(j, j, posjecen, matricaSusjedstva)) {
        TSP.first += par.first;
        TSP.second.push_back(par.second);
      } else {
        TSP.first -= par.first;
        TSP.second.pop_back();
      }

      matricaSusjedstva[i][j] = 0;
      matricaSusjedstva[j][i] = 0;
    }

    if (TSP.second.size() == n - 1) break;
  }

  int pocetniCvor = TSP.second.front().first;
  int krajnjiCvor = TSP.second.back().second;
  TSP.first += matrica_susjedstva[pocetniCvor][krajnjiCvor];
  TSP.second.push_back({krajnjiCvor, pocetniCvor});

  return TSP;
}

pair<double, vector<pair<int, int>>> TezinskiGraf::TSPVerzijaDva() {
  pair<double, vector<pair<int, int>>> TSP{0, vector<pair<int, int>>()};
  vector<pair<double, pair<int, int>>> rutaGradova;
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (i != j) {
        double udaljenost = matrica_susjedstva[i][j];
        rutaGradova.push_back({udaljenost, {i, j}});
      }
    }
  }

  sort(rutaGradova.begin(), rutaGradova.end());
  DisjunktniSkupovi ds(n);

  for (auto par : rutaGradova) {
    int i = par.second.first;
    int j = par.second.second;

    if ((stepenCvora(i, TSP.second) <= 2) && (stepenCvora(j, TSP.second) <= 2)) {
      if (ds.Nadji(i) != ds.Nadji(j)) {
        ds.Spoji(i, j);
        TSP.first += par.first;
        TSP.second.push_back(par.second);
      } else {
        bool nasaoDruguGranu = false;
        for (auto grana : rutaGradova) {
          int altI = grana.second.first;
          int altJ = grana.second.second;

          if ((stepenCvora(altI, TSP.second) < 2) && (stepenCvora(altJ, TSP.second) < 2) && (ds.Nadji(altI) != ds.Nadji(altJ))) {
            ds.Spoji(altI, altJ);
            TSP.first += grana.first;
            TSP.second.push_back(grana.second);
            nasaoDruguGranu = true;
            break;
          }
        }
        if (!nasaoDruguGranu) {
          ds.Spoji(i, j);
          TSP.first -= par.first;
          TSP.second.pop_back();
        }
      }
    }

    if (TSP.second.size() == n - 1) break;
  }

  int pocetniCvor = TSP.second.front().first;
  int krajnjiCvor = TSP.second.back().second;
  TSP.first += matrica_susjedstva[pocetniCvor][krajnjiCvor];
  TSP.second.push_back({krajnjiCvor, pocetniCvor});

  return TSP;
}

pair<double, vector<pair<int, int>>> TezinskiGraf::TSPVerzijaTri() {
  pair<double, vector<pair<int, int>>> TSP{0, vector<pair<int, int>>()};
  vector<pair<double, pair<int, int>>> rutaGradovi;

  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (i != j) {
        double udaljenost = matrica_susjedstva[i][j];
        rutaGradovi.push_back({udaljenost, {i, j}});
      }
    }
  }

  sort(rutaGradovi.begin(), rutaGradovi.end());
  DisjunktniSkupoviLista ds(n);

  for (auto par : rutaGradovi) {
    int i = par.second.first;
    int j = par.second.second;

    if ((stepenCvora(i, TSP.second) <= 2) && (stepenCvora(j, TSP.second) <= 2)) {
      int skup1 = ds.Nadji(i);
      int skup2 = ds.Nadji(j);

      if (skup1 != skup2) {
        ds.Spoji(skup1, skup2);
        TSP.first += par.first;
        TSP.second.push_back(par.second);
      } else {
        bool nasaoDruguGranu = false;
        for (auto grana : rutaGradovi) {
          int altI = grana.second.first;
          int altJ = grana.second.second;
          int altSkup1 = ds.Nadji(altI);
          int altSkup2 = ds.Nadji(altJ);

          if ((stepenCvora(altI, TSP.second) < 2) && (stepenCvora(altJ, TSP.second) < 2) && (altSkup1 != altSkup2)) {
            ds.Spoji(altSkup1, altSkup2);
            TSP.first += grana.first;
            TSP.second.push_back(grana.second);
            nasaoDruguGranu = true;
            break;
          }
        }

        if (!nasaoDruguGranu) {
          ds.Spoji(skup1, skup2);
          TSP.first -= par.first;
          TSP.second.pop_back();
        }
      }
    }

    if (TSP.second.size() == n - 1) break;
  }

  int pocetniCvor = TSP.second.front().first;
  int krajnjiCvor = TSP.second.back().second;
  TSP.first += matrica_susjedstva[pocetniCvor][krajnjiCvor];
  TSP.second.push_back({krajnjiCvor, pocetniCvor});

  return TSP;
}
