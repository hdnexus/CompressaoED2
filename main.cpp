#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unordered_map>

using namespace std;

////////////
//OPÇÃO 1//
//////////

void createInitialTableInteger(string sTotal, vector<string> &dictionary)
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
  Removo os elementos duplicados para ter só valores (que estão dentro da tabela ASCII) sem repetições no dicionário inicial.
  O erase irá remover os elementos duplicados junto com o unique.
  */
  sort(dictionary.begin(), dictionary.end());
  dictionary.erase(unique(dictionary.begin(), dictionary.end()), dictionary.end());
}

void compressionInteger(string sTotal, vector<int> &codified, vector<string> &dictionary)
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

string decompressionInteger(vector<string> &dictionary, vector<int> &codified)
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

void printCodifiedInteger(vector<int> &codified, ofstream &arq)
{
  arq << "\nCodificacao" << endl;
  for (int n = 0; n < codified.size(); n++)
  {
    arq << codified[n] << "   ";
  }
}

void printDecodifiedInteger(string finalString, ofstream &arq)
{
  arq << "\n\nDecodificacao\n"
      << finalString << endl;
}

void printInitialTableInteger(vector<string> &dictionary, ofstream &arq)
{
  //Imprimindo a tabela de simbolos inicial
  arq << "\nTabela de Simbolos Inicial" << endl;
  for (int p = 0; p < dictionary.size(); p++)
  {
    arq << p << "   " << dictionary[p] << endl;
  }
}
void printFinalTableInteger(vector<string> &dictionary, ofstream &arq)
{
  arq << "\nTabela de Simbolos Final" << endl;
  for (int p = 0; p < dictionary.size(); p++)
  {
    arq << p << "   " << dictionary[p] << endl;
  }
}

////////////
//OPÇÃO 2//
//////////

void createASCIITable(unordered_map<string, int> &dictionary)
{
  string c;
  for (int i = 0; i < 256; i++)
  {
    c = "";
    c += char(i);
    dictionary[c] = i;
  }
}

//Parecido com a compressão com inteiros
//referência https://www.geeksforgeeks.org/lzw-lempel-ziv-welch-compression-technique/
void compressionASCII(unordered_map<string, int> &dictionary, vector<int> &codified, string sTotal, vector<string> &finalTable)
{
  int index;
  char cx = sTotal[0];
  string P(1, cx);
  string C;
  int newCode = 256; //referência: Slide do jairo
  for (int i = 0; i < sTotal.size() - 1; i++)
  {
    C = sTotal[i + 1];
    if (dictionary.find(P + C) != dictionary.end())
    {
      P = P + C;
    }
    else
    {
      codified.push_back(dictionary[P]);
      dictionary[P + C] = newCode;
      finalTable.push_back(P + C);
      newCode++;
      P = C;
    }
  }
  codified.push_back(dictionary[P]);
}

//referência https://www.geeksforgeeks.org/lzw-lempel-ziv-welch-compression-technique/
void decompressionASCII(vector<int> &codified, ofstream &arq)
{
  unordered_map<int, string> dictionaryASCII;
  string c;
  for (int i = 0; i < 256; i++)
  {
    c = "";
    c += char(i);
    dictionaryASCII[i] = c;
  }
  int oldCode = codified[0];
  int n;
  arq << "\n\nDecodificacao" << endl;
  string s = dictionaryASCII[oldCode];
  c = "";
  c += s[0];
  arq << s;
  int count = 256;
  for (int i = 0; i < codified.size() - 1; i++)
  {
    n = codified[i + 1];
    if (dictionaryASCII.find(n) == dictionaryASCII.end())
    {
      s = dictionaryASCII[oldCode];
      s = s + c;
    }
    else
    {
      s = dictionaryASCII[n];
    }
    arq << s;
    c = "";
    c += s[0];
    dictionaryASCII[count] = dictionaryASCII[oldCode] + c;
    count++;
    oldCode = n;
  }
}

void printInitialTableASCII(vector<int> &codified, ofstream &arq)
{
  arq << "\nTabela de Simbolos Inicial" << endl;
  vector<int> listIntegers;
  for (int p = 0; p < codified.size(); p++)
  {
    if (codified[p] < 256)
    {
      listIntegers.push_back(codified[p]);
    }
  }

  sort(listIntegers.begin(), listIntegers.end());
  listIntegers.erase(unique(listIntegers.begin(), listIntegers.end()), listIntegers.end());

  for (int i = 0; i < listIntegers.size(); i++)
  {
    arq << listIntegers[i] << "   " << char(listIntegers[i]) << endl;
  }
}

void printCodifiedASCII(vector<int> &codified, ofstream &arq)
{
  arq << "\nCodificacao" << endl;
  for (int n = 0; n < codified.size(); n++)
  {
    arq << codified[n] << "   ";
  }
}

void printFinalTableASCII(vector<int> &codified, ofstream &arq, vector<string> &finalTable)
{
  arq << "\nTabela de Simbolos Final" << endl;
  vector<int> listIntegers;
  int value = 0;

  for (int p = 0; p < codified.size(); p++)
  {
    listIntegers.push_back(codified[p]);
  }

  sort(listIntegers.begin(), listIntegers.end());
  listIntegers.erase(unique(listIntegers.begin(), listIntegers.end()), listIntegers.end());

  for (int i = 0; i < listIntegers.size(); i++)
  {
    if (listIntegers[i] < 256)
    {
      arq << listIntegers[i] << "   " << char(listIntegers[i]) << endl;
    }
    else
    {
      arq << listIntegers[i] << "   " << finalTable[value] << endl;
      value++;
    }
  }
}

int main(int argc, char **argv)
{
  string s;
  string sTotal;
  vector<int> codified;
  vector<string> dictionary;
  vector<string> finalTable;
  vector<int> codifiedASCII;
  unordered_map<string, int> dictionaryASCII;
  int choice;

  ifstream in(argv[1]);

  ofstream arq("exit.txt");

  if (in.is_open())
  {
    while (!in.eof())
    {
      getline(in, s);
      sTotal += s;
    }
  }
  in.close();

  cout << "Atividade ED2 - Compressao LZW" << endl;
  cout << "Digite a opcao desejada para comprimir seu arquivo txt" << endl;
  cout << "[1] Para que os indices de codificadacao sejam uma ordem de valores inteiros" << endl;
  cout << "[2] Para que os indices de codificadacao sejam as posicoes dos elementos na tabela ASCII" << endl;
  cout << "Sua escolha: ";
  cin >> choice;

  while (choice < 1 || choice > 2)
  {
    cout << "Valor incorreto, digite uma opcao valida!" << endl;
    cout << "Escolha novamente: ";
    cin >> choice;
  }

  cout << "Aguarde o processamento do arquivo..." << endl;

  if (choice == 1)
  {
    arq << "------Metodo de numeros inteiros------" << endl;
    arq << "\nString lida do arquivo \n"
        << sTotal << endl;
    arq << "\nTamanho da string digitada\n"
        << sTotal.size() << endl;

    //Criando o dicionário inicial
    createInitialTableInteger(sTotal, dictionary);
    printInitialTableInteger(dictionary, arq); //printando a tabela inicial

    //Codificação
    compressionInteger(sTotal, codified, dictionary);
    printFinalTableInteger(dictionary, arq); //printando a tabela final
    printCodifiedInteger(codified, arq);     //printando a codificação

    //Decodificação
    string decodified = decompressionInteger(dictionary, codified);
    printDecodifiedInteger(decodified, arq); //printando a decodificação
  }

  if (choice == 2)
  {
    arq << "------Metodo de posicoes da tabela ASCII------" << endl;
    arq << "\nString lida do arquivo \n"
        << sTotal << endl;
    arq << "\nTamanho da string digitada\n"
        << sTotal.size() << endl;

    //Criando tabela ASCII, não utilizei dicionário inicial aqui.
    createASCIITable(dictionaryASCII);

    //Codificação
    compressionASCII(dictionaryASCII, codifiedASCII, sTotal, finalTable);
    printInitialTableASCII(codifiedASCII, arq);
    printFinalTableASCII(codifiedASCII, arq, finalTable);
    printCodifiedASCII(codifiedASCII, arq); //printando a codificação

    //Decodificação
    decompressionASCII(codifiedASCII, arq); //printando a decodificação
  }

  arq.close();
  cout << "Arquivo processado, veja o resultado em exit.txt!";
}