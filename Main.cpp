#include"AdaptiveHufffman.h"
using namespace std;
void main()
{
	int m;
	HuffmanTree encodeTree;
	HuffmanTree decodeTree;
	cout << "=======NEN HUFFMAN=======" << endl;
	do {
		cout << "1. Nen file" << endl;
		cout << "2. Giai nen file" << endl;
		cout << "0. Thoat" << endl;
		cout << "--->Chon chuc nang: ";
		cin >> m;
		switch (m)
		{
		case 1:
			encodeTree.EncodingFile();
			break;
		case 2:
			decodeTree.DecodingFile();
			break;
		case 0:
			break;
		default:
			cout << "Khong hop le!";
			break;
		}
	} while (m != 0);
	
	
	
}
