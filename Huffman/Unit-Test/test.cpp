#include "pch.h"
#include <string>
#include <random>
#include <fstream>
#include <functional>
#include <algorithm>
#include "../lab3/Huffman.h"

using namespace std;

namespace compress {
	class HuffmanTest : public ::testing::Test {
	public:
		HuffmanTest() {};
		virtual ~HuffmanTest() {
			// Can use GetParam() here.
		};
		virtual void TearDown() {
			// Can use GetParam() here.
		};
		virtual void SetUp() {
			// Can use GetParam() here.
		};

	protected:

		Huffman huffman;

	public:
		bool fileCmp(string file1, string file2) const { return huffman.fileCmp(file1, file2); };
		bool encodeParallel(int threadsCount, string filename) { return huffman.encodeParallel(threadsCount, filename); }
		void decodeParallel(string sourceDir, string destFile) { return huffman.decodeParallel(sourceDir, destFile); }
		string genRandFile(int size, string filename) { return huffman.genRandFile(size, filename); }
	};

	string random_string(size_t length)
	{
		vector<char> ch_set = { '0','1','2','3','4',
			'5','6','7','8','9',
			'A','B','C','D','E','F',
			'G','H','I','J','K',
			'L','M','N','O','P',
			'Q','R','S','T','U',
			'V','W','X','Y','Z',
			'a','b','c','d','e','f',
			'g','h','i','j','k',
			'l','m','n','o','p',
			'q','r','s','t','u',
			'v','w','x','y','z'
		};

		std::default_random_engine rng(std::random_device{}());

		//2) create a random number "shaper" that will give
		//   us uniformly distributed indices into the character set
		std::uniform_int_distribution<> dist(0, ch_set.size() - 1);

		//3) create a function that ties them together, to get:
		//   a non-deterministic uniform distribution from the 
		//   character set of your choice.
		auto randchar = [ch_set, &dist, &rng]() {return ch_set[dist(rng)]; };
		string str(length, 0);
		std::generate_n(str.begin(), length, randchar);
		return str;
	}

	string getExt(string filename) {
		int k = filename.size() - 1;
		while (k > 0 && filename[k] != '.')
			--k;
		return string(filename.begin() + k, filename.end());
	}

	void createFile(string filename, string content) {
		ofstream f(filename, ios::trunc);
		f << content;
		f.close();
	}

	void removeFile(string filename) {
		string command = "remove " + filename;
		system(command.c_str());
	}

	TEST_F(HuffmanTest, genRandFileWorks) {
		string filename = "tmp.txt";
		int size = 10000;
		genRandFile(size, filename);
		ifstream f(filename, ios::binary | ios::ate);
		EXPECT_EQ((int)f.tellg(), size);
		f.close();
		removeFile(filename);
	}

	TEST_F(HuffmanTest, fileCmpWorks) {
		string filename1 = "tmp1.txt", filename2 = "tmp2.txt";
		string str = random_string(100);

		createFile(filename1, str);
		createFile(filename2, str);

		EXPECT_EQ(fileCmp(filename1, filename2), true);
		removeFile(filename1);
		removeFile(filename2);
	}

	TEST_F(HuffmanTest, EncodeDecodeWorks) {
		//test on empty file
		int threadsCount = 8;
		string file = "m.bin";

		int errors = 0;
		int testSize = 1;
		string ext = getExt(file);
		removeFile(file + ext);
		system(("rmdir " + file + "_compressed /s").c_str());

		for (size_t i = 0; i < testSize; ++i) {
			string content = genRandFile(256*8, file);
			bool success = encodeParallel(threadsCount, file);
			if (success) {
				decodeParallel(file + "_compressed", file + ext);
				bool success = fileCmp(file, file + ext);
				EXPECT_EQ(success, true);		//test on
				if (!success)
					++errors;
			}
			else {
				FAIL();
				++errors;
			}
			removeFile(file);
			removeFile(file + ext);
			system(("rmdir " + file + "_compressed /s").c_str());
		}
		cout << "Errors: " << errors << endl;
	}
}