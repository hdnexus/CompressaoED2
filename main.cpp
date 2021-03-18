#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

/* Comentários
1- Professor, antes de tudo, gostaria de falar que utilizei o 0 como primeiro output Index da tabela, vi alguns exemplos que começavam com 1, 
outros com 0, só que não achei nada falando qual era o modelo correto.
2- Mandei um email para o senhor, e você disse que eu poderia ter utilizado o output Index como inteiros (Ex: 0,1,2,3) e não posições seguindo a 
tabela ASCII (Ex: 86, 256, 257). Caso fosse necessário utilizar os valores da tabela ASCII como output Index, não haveriam mudanças tão drásticas.
Eu poderia após preencher um map/table com os elementos da tabela ASCII, pegar a posição dos meus caracteres para fazer o dicionario inicial, e depois
eu criaria uma variável que armazenaria o valor 256, e ia incrementando esse valor a medida que aumentasse os indices do diciónario. 
Ex:  97 a
     99 c
     256 ac
*/

void createInitialTable(string sTotal, vector<string> &dictionary)
{
  //Peguei cada caractere da string, transformei eles em string e dei push no vector dictionary
  for (int i = 0; i < sTotal.size(); i++)
  {
    char c = sTotal[i];
    string sTotal(1, c);
    dictionary.push_back(sTotal);
  }
  /*
  Utilizei o sort para ordenar esses caracteres/strings. O sort já ordena corretamente na ordem dos elementos da tabela ASCII.
  Caso fosse necessário implementar uma função própria sem o sort, o que mudaria é que eu teria que fazer um map ou table,
  contendo as 256 posições da tabela ASCII, contendo indice e valor.
  O erase irá remover os elementos duplicados com o método unique.
  */
  sort(dictionary.begin(), dictionary.end());
  dictionary.erase(unique(dictionary.begin(), dictionary.end()), dictionary.end());
}

void compression(string sTotal, vector<int> &codified, vector<string> &dictionary)
{
  int index;
  char cx = sTotal[0];
  string P(1, cx);
  string C;
  for (int i = 1; i < sTotal.size(); i++)
  {
    C = sTotal[i];
    vector<string>::iterator itr = find(dictionary.begin(), dictionary.end(), P + C);
    if (itr != dictionary.end())
    {
      P = P + C;
    }
    else
    {
      vector<string>::iterator itrP = find(dictionary.begin(), dictionary.end(), P);
      index = distance(dictionary.begin(), itrP);
      codified.push_back(index);
      dictionary.push_back(P + C);
      P = C;
    }
  }
  vector<string>::iterator itrP = find(dictionary.begin(), dictionary.end(), P);
  index = distance(dictionary.begin(), itrP);
  codified.push_back(index);
}

string decompression(vector<string> &dictionary, vector<int> &codified)
{
  string auxString;
  string finalString;
  for (int x = 0; x < codified.size(); x++)
  {
    int indexDecodify = codified[x];
    auxString = dictionary[indexDecodify];
    finalString = finalString + auxString;
  }
  return finalString;
}

void printCodified(vector<int> &codified, ofstream &arq)
{
  arq << "\nCodificacao" << endl;
  for (int n = 0; n < codified.size(); n++)
  {
    arq << codified[n] << "   ";
  }
}

void printDecodify(string finalString, ofstream &arq)
{
  arq << "\n\nDecodificacao\n"
      << finalString << endl;
}

void printInitialTable(vector<string> &dictionary, ofstream &arq)
{
  //Imprimindo a tabela de simbolos inicial
  arq << "\nTabela de Simbolos Inicial" << endl;
  for (int p = 0; p < dictionary.size(); p++)
  {
    arq << p << "   " << dictionary[p] << endl;
  }
}
void printFinalTable(vector<string> &dictionary, ofstream &arq)
{
  arq << "\nTabela de Simbolos Final" << endl;
  for (int p = 0; p < dictionary.size(); p++)
  {
    arq << p << "   " << dictionary[p] << endl;
  }
}

int main()
{
  cout << "Aguarde o processamento do arquivo..." << endl;
  string s;
  string sTotal;
  ifstream in;
  ofstream arq("exit.txt");

  in.open("entry.txt");
  while (!in.eof())
  {
    getline(in, s);
    sTotal += s;
  }
  in.close();

  vector<int> codified;
  vector<string> dictionary;

  arq << "String lida do arquivo \n"
      << sTotal << endl;
  arq << "\nTamanho da string digitada\n"
      << sTotal.size() << endl;

  //Criando o dicionário inicial utilizando a ordem da tabela ASCII
  createInitialTable(sTotal, dictionary);
  printInitialTable(dictionary, arq); //printando a tabela inicial

  //Codificação
  compression(sTotal, codified, dictionary);
  printFinalTable(dictionary, arq); //printando a tabela final
  printCodified(codified, arq); //printando a codificação

  //Decodificação
  string decodified = decompression(dictionary, codified);
  printDecodify(decodified, arq); //printando a decodificação

  arq.close();
  cout << "Arquivo processado, veja o resultado em exit.txt!";
}