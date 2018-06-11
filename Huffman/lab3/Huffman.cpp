#define ll long long

#include "Huffman.h"
#include "TrNd.h"
#include "Queue.h"
#include <direct.h>
#include <string>
#include <Windows.h>
//#include <boost/filesystem.hpp>


using namespace std;
using namespace compress;
//namespace fs = std::filesystem;
//using namespace filesystem;

stack<int> filestk;
mutex guard;

void Huffman::buildQueue(int *frequencies, Queue*&q)
{
	for (int i = 0; i < 256; ++i)
		if (frequencies[i])
		{
			TrNd *t = new TrNd(i-128, frequencies[i]);
			q = push(t, q);
		}		
}

//Transform queue into tree
void Huffman::buildTree(Queue *head, TrNd *&tree)
{
	if (!head)
		return;
	if (tree)
		clear(tree);
	while (head->next)
	{
		Queue *p = head;
		TrNd *Node = new TrNd;
		Node->l = p->elem;
		Node->r = p->next->elem;
		Node->freq = Node->l->freq + Node->r->freq;

		Queue *q = p;
		while (q->next && q->elem->freq < Node->freq)
			q = q->next;

		Queue *piece = new Queue(Node);
		if (Node->freq < q->elem->freq)
			insert(q->prev, q, piece);
		else insert(q, q->next, piece);

		head = head->next;
		delete p;
		p = head;
		head = head->next;
		delete p;
		head->prev = nullptr;
	}
	tree = head->elem;
}

// ?
TrNd* Huffman::loadTreeFile(string filename)
{
	ifstream fin(filename, ios::in | ios::binary);	
	if (!fin.is_open())
		throw(exception((filename + " NOT FOUND").c_str()));
	TrNd *tree;
	stack<TrNd*> stck;
	TrNd *node1 = nullptr, *node2 = nullptr;
	char token;
	while (!fin.eof())
	{
		fin.read(&token, sizeof(char));
		if (token == '_')
		{
			stck.push(nullptr);
		}
		if (token == '1')
		{
			fin.read(&token, sizeof(char));
			node1 = new TrNd(token, 0);
			stck.push(node1);
		}
		else if (token == '0' && !fin.eof()) {
			node2 = stck.top();
			stck.pop();
			node1 = stck.top();
			stck.pop();
			node1 = new TrNd(-1, node1, node2);
			stck.push(node1);
		}
		else {}
	}
	tree = stck.top();
	stck.pop();
	fin.close();
	return tree;
}

//+
void Huffman::writeTreeFile(string filename, TrNd *tree)
{
	ofstream fout(filename, ios::out | ios::binary);
	if (!fout.is_open())
		throw(exception(("Cannot open " + filename).c_str()));
	function<void(TrNd *)> write = [&write, filename, &fout](TrNd *t)
	{
		if (!t)
		{
			fout << "_";
			return;
		}
		if (!t->l && !t->r)
		{
			fout << '1';
//			fout.write('1', sizeof(char));
			fout << t->symbol;
		}
		else {
			write(t->l);
			write(t->r);
			fout << '0';
		}
	};
	write(tree);
}

//+++++++++++++++
int Huffman::encodeProc(map<char, vector<bool>> *Keys, char *text, int len, vector<bitset<BYTE_SIZE>> *HuffmanCode)
{
	vector<bool> TmpCode;
	map<char, vector<bool>>::iterator it;
	for (int i = 0; i < len; i++)
	{
		it = Keys->find(text[i]);
		TmpCode.insert(TmpCode.end(), it->second.begin(), it->second.end());
	}

	int offset = (8 - TmpCode.size() % 8) % 8;
	int filesize = (offset + TmpCode.size()) / 8;


	for (int i = 0; i < offset; i++)
	{
		TmpCode.insert(TmpCode.begin(), 0);
	}

	HuffmanCode->resize(filesize);
	for (int i = 0; i < filesize; i++)
		for (int j = 0; j < BYTE_SIZE; j++)
			(*HuffmanCode)[i][BYTE_SIZE - 1 - j] = (TmpCode[8 * i + j] == 1);
	return offset;
}

void Huffman::encodeThreadFunc(string filename)
{
	//READ FILE
	while (!filestk.empty())
	{
		guard.lock();
		int i = filestk.top();
		filestk.pop();
		guard.unlock();
		string encodedfn = filename + "_compressed//" + to_string(i) + ".bin";
		ifstream fin(encodedfn, ios::in | ios::binary | ios::ate);
		if (!fin.is_open())
			throw runtime_error("Couldn't open " + encodedfn);
		int filesz = fin.tellg();
		fin.seekg(ios::beg);
		char *buff = new char[filesz];
		fin.read(buff, filesz * sizeof(char));
		fin.close();


		//COUNT FREQUENCY OF EVERY SYMBOL
		int *frequencies = new int[256];
		for (size_t i = 0; i < 256; ++i)
			frequencies[i] = 0;
		for (int i = 0; i < filesz; ++i)
			++frequencies[buff[i] + 128];

		//BUILD HUFFMAN TREE
		Queue *q = nullptr;
		TrNd *t = nullptr;
		buildQueue(frequencies, q);
		buildTree(q, t);

		//GET KEYS FROM TREE
		map<char, vector<bool>> Keys;
		for (int i = 0; i < 256; i++)
			Keys.insert(pair<char, vector<bool>>((char)i, vector<bool>()));
		vector<bool> key;
		function<void(TrNd*)> getkeys = [&getkeys, &key, &Keys](TrNd *t)
		{
			if (t)
			{
				if (t->freq > 0 && !t->l && !t->r)
					Keys.find(t->symbol)->second = key;
				key.push_back(0);
				getkeys(t->l);
				key[key.size() - 1] = 1;
				getkeys(t->r);
				key.pop_back();
			}
		};
		if (!t->l && !t->r)
			Keys.find(t->symbol)->second = { 0 };
		else	getkeys(t);

		//ENCODE
		vector<bitset<BYTE_SIZE>> HuffmanCode;
		int offset = encodeProc(&Keys, buff, filesz, &HuffmanCode);

		//WRITE TREE FILE
		string treefilename = filename + "_compressed//" + to_string(i) + "_tr.bin";
		ofstream fout(treefilename, ios::out | ios::binary | ios::trunc);
		if (!fout.is_open())
			throw(exception(("CAN'T OPEN " + treefilename).c_str()));
		writeTreeFile(treefilename, t);
		fout.close();

		//WRITE ENCODED FILE
		

		fout.open(encodedfn, ios::out | ios::binary | ios::trunc);
		if (!fout.is_open())
			throw(exception(("CAN'T OPEN " + encodedfn).c_str()));
		char off = '0' + offset;
		fout.write(&off, sizeof(char));
		for (size_t i = 0; i < HuffmanCode.size(); i++)
		{
			char c = static_cast<char>(HuffmanCode[i].to_ulong());
			fout.write(&c, sizeof(char));
		}
		fout.close();

		ifstream fin1(encodedfn, ios::in | ios::binary | ios::ate);
		int sz = fin1.tellg();
		if (sz != HuffmanCode.size() + 1) {
			int k = 0;
		}



		fin1.close();

		delete[] buff;
		delete[] frequencies;
	}
}

void Huffman::decodeThreadFunc(string filename, string treeFilename, string * ResOfDecoding)
{

	//CHECK SIZE

	ifstream fin_tmp(filename, ios::in | ios::binary | ios::ate);
	if (!fin_tmp.is_open())
		throw runtime_error((filename + " NOT FOUND").c_str());

	int sz = fin_tmp.tellg();

	fin_tmp.close();

	//READ FILE
	ifstream fin(filename, ios::in | ios::binary);
	ifstream trfin(treeFilename, ios::in | ios::binary);

	if (!trfin.is_open())
		throw runtime_error((treeFilename + " NOT FOUND").c_str());

	trfin.close();

	vector<bitset<BYTE_SIZE>> filedata = {};

	char it;

	fin.read(&it, sizeof(char));
	
	int offset = it - '0';

//	fin.seekg(1);

	for (int i=0; i < sz; ++i)
	{
		fin.read(&it, sizeof(char));
		if (!fin.eof())
			filedata.push_back((bitset<BYTE_SIZE>)it);
	}
	string HuffmanCode;
	for (size_t i = 0; i < filedata.size(); ++i)
		HuffmanCode = HuffmanCode + filedata[i].to_string();
	fin.close();
	
	//??? if (offset != 0)
	HuffmanCode.erase(HuffmanCode.begin(), HuffmanCode.begin() + offset);

	//READ TREE FILE
	TrNd *tree = loadTreeFile(treeFilename);
	//DECODE

	decodeProc(tree, &HuffmanCode, ResOfDecoding);

	clear(tree);
}

bool Huffman::encodeParallel(int threadsCount, string filename)
{
	if (threadsCount <= 0)
		return false;

	ifstream fin(filename, ios::binary | ios::ate);
	if ((int)fin.tellg() < threadsCount)
		return false;

	int cnum = splitFile(filename, threadsCount);

	//PARSE FILENAME INTO NAME AND EXTENTION
	int k = filename.size() - 1;
	while (filename[k] != '.')
		--k;


	for (int i = 0; i < cnum; ++i)
		filestk.push(i);

	vector<unique_ptr<thread>> threads;

	threads.resize(threadsCount);
	for (int i = 0; i < threadsCount; ++i)
	{
		threads[i].reset(new thread(encodeThreadFunc, filename));
		threads[i]->join();
	}

	return true;
}

void Huffman::decodeProc(TrNd *tree, string *Code, string *ResOfDecoding)
{
	*ResOfDecoding = "";
	TrNd *t = tree;
	for (size_t i = 0; i < Code->size(); i++)
	{
		if (!t->l && !t->r)
		{
			ResOfDecoding->push_back(t->symbol);
			t = tree;
		}
		if ((*Code)[i] == '0')
			t = t->l;
		else t = t->r;
	}
	if (!t->l && !t->r)
		ResOfDecoding->push_back(t->symbol);
}

void Huffman::decodeParallel(string sourceDir, string destFile)
{
	string treeSuffix = "_tr";
	vector<string> files;
	vector<string> trees;

	//GET ALL FILES IN PATH
	HANDLE dir;
	WIN32_FIND_DATA file_data;
	if ( (dir = FindFirstFile(LPCSTR((sourceDir + "//*").c_str()), &file_data) ) == INVALID_HANDLE_VALUE)
	{
		throw runtime_error("INVALID DIRECTORY " + sourceDir);
	}

	string curfile;
	while (FindNextFile(dir, &file_data)) 
	{	
		if (strcmp(file_data.cFileName, ".") != 0 && strcmp(file_data.cFileName, "..") != 0)
		{
			curfile = file_data.cFileName;
			
			int k = curfile.size() - 4;
			if (string(curfile.begin() + k, curfile.end()) == ".bin")
			{
				bool isTreeFile = true;
				for (int i = 0; i < treeSuffix.size(); ++i)
					if (curfile[k - i - 1] != treeSuffix[treeSuffix.size() - 1 - i])
					{
						isTreeFile = false;
						files.push_back(sourceDir + "//" + curfile);
						break;
					}
				if (isTreeFile)
					trees.push_back(sourceDir + "//" + curfile);
			}
		}
	}

	//CHECK IF FOR EVERY FILE EXISTS TREE
	if (files.size() != trees.size())
		throw(exception("Lost some files in directory"));

	//FIND PARENT DIRECTORY PATH
	while (sourceDir[sourceDir.size() - 1] == '/')
		sourceDir.resize(sourceDir.size() - 1);

	int threadsNum = files.size();
	vector<unique_ptr<thread>> threads(threadsNum);
	string *data = new string[threadsNum];
	for (int i = 0; i < threadsNum; ++i)
	{
		threads[i].reset(new thread(decodeThreadFunc, files[i], trees[i], &data[i]));
		threads[i]->join();
	}

	ofstream fout(destFile, ios::out | ios::binary | ios::trunc);
	for (int i = 0; i < threadsNum; ++i) {
		int len = data[i].length();
		fout.write(data[i].c_str(), data[i].length() * sizeof(char));
	}
	delete[] data;
	fout.close();
}

void Huffman::demo()
{
	string filename;
	bool needtogen;
	cout << "FILENAME : ";
	getline(cin, filename);
	cin.ignore(cin.rdbuf()->in_avail());

	cout << "NEED TO GEN : ";
	cin >> needtogen;

	if (needtogen)
	{
		int size;
		cout << "SIZE : ";
		cin >> size;
		genRandFile(size, filename);
	}

	int k = filename.size() - 1;
	while (filename[k] != '.')
		--k;
	string ext = string(filename.begin() + k, filename.end());

	unsigned int threadsCount;
	cout << "THREADS COUNT : ";
	cin >> threadsCount;
	if (!encodeParallel(threadsCount, filename)) {
		cout << "Can't encode";
		return;
	}
	decodeParallel(filename + "_compressed", filename+ext);
	if (fileCmp(filename, filename + ext))
		cout << "FILES ARE EQUAL." << endl;
	else cout << "FILES ARE NOT EQUAL" << endl;
}


string compress::Huffman::genRandFile(int size, string filename)
{
	ofstream fout(filename, ios::out | ios::binary | ios::trunc);
	string buff;
	for (int i = 0; i < size; ++i)
		buff += (char)rand();
	fout.write(buff.c_str(), size);
	fout.close();
	return buff;
}

bool compress::Huffman::fileCmp(string file1, string file2) const
{
	char buff1, buff2;
	ifstream fin1(file1, ios::in | ios::binary);
	ifstream fin2(file2, ios::in | ios::binary);

	if (!fin1.is_open() || !fin2.is_open())
		return false;

	int sz1 = fin1.tellg();
	fin1.seekg(0, ios::end);
	sz1 = (int)fin1.tellg() - sz1;

	int sz2 = fin2.tellg();
	fin2.seekg(0, ios::end);
	sz2 = (int)fin2.tellg() - sz2;

	if (sz1 != sz2)
		return false;

	fin1.seekg(0);
	fin2.seekg(0);

	for (int i = 0; i < sz1; ++i)
	{
		fin1.read(&buff1, sizeof(char));
		fin2.read(&buff2, sizeof(char));
		if (buff1 != buff2)
		{
			cout << endl;
			cout << i << endl;
			return 0;
		}
	}

	fin1.close();
	fin2.close();
	return 1;
}

//return chunksNum of chunkSize
int Huffman::splitFile(string filename, int num)
{
	int k = filename.size() - 1;
	while (k > 0 && filename[k] != '/')
		--k;
	string fname = string(filename.begin() + k, filename.end());

	ifstream fin(filename, ios::in | ios::binary);
	if (!fin.is_open())
		throw runtime_error("Couldn't open file.");
	int sz = fin.tellg();
	fin.seekg(0, ios::end);
	sz = (int)fin.tellg() - sz;

	int chunkSize = sz / num;
	int chunksPerThread = 1;

	while (chunkSize > 256)
	{
		chunkSize >>= 1;
		chunksPerThread <<= 1;
	}

	int chunksNum = num*chunksPerThread;

	fin.seekg(0, ios::beg);
	char *buff = new char[chunkSize+1];
	string cwd = getCD();
	CreateDirectoryA((filename+"_compressed").c_str(), 0);
	
	for (int i = 0; i < chunksNum-1; ++i)
	{
		fin.read(buff, chunkSize*sizeof(char));
		ofstream file(filename + "_compressed//" + to_string(i) + ".bin", ios::out | ios::binary);
		file.write(buff, chunkSize*sizeof(char));
		file.close();
	}
	
	char t;
	ofstream file(filename + "_compressed//" + to_string(chunksNum - 1) + ".bin", ios::out | ios::binary);
	int last_sz = sz % chunkSize;
	if (last_sz == 0)
		last_sz = chunkSize;
	for (int i = 0; i < last_sz; ++i)
	{
		fin.read(&t, sizeof(char));
		file.write(&t, sizeof(char));
	}
	delete[] buff;
	file.close();
	fin.close();
	return chunksNum;
}

string Huffman::getCD()
{
	wchar_t *w = nullptr;
	wstring ws = _wgetcwd(w, _MAX_PATH);
	return string(ws.begin(), ws.end());
}
