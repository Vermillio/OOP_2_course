#ifndef HUFFMAN_H
#define HUFFMAN_H
#define BYTE_SIZE 8

#include <stack>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iterator>
#include <utility>
#include <algorithm>
#include <vector>
#include "TrNd.h"
#include "Queue.h"
#include <bitset>
#include <thread>
#include <mutex>


using namespace std;

namespace compress
{
	class Huffman
	{
	private:

		string treeSuffix = "_tr";
//		string encodeSuffix = "_compressed";

		//ENCODING
		int splitFile(string filename, int num);
		static void writeTreeFile(string filename, TrNd *tree);
		static void buildQueue(int *frequencies, Queue *&q);
		static void buildTree(Queue *q, TrNd *&tree);
		static int encodeProc(map<char, vector<bool>> *Keys, char *text, int len, vector<bitset<BYTE_SIZE>> *HuffmanCode);
		static void encodeThreadFunc(string filename);

		//DECODING
		static TrNd *loadTreeFile(string filename);
		static void decodeProc(TrNd *tree, string *Code, string *ResOfDecoding);
		static void decodeThreadFunc(string filename, string treeFilename, string *ResOfDecoding);

		void GENRANDFILE(int size, string filename);
		bool fileCmp(string file1, string file2);
		void ErrorMessage(int n);
		string getCD();
	public:
		Huffman() {};
		~Huffman() {};
		void encodeParallel(int threadsCount, string filename);
		void decodeParallel(string sourceDir, string destFile);
		void demo();

	};
}






#endif