#include "AdaptiveHufffman.h"
#include <algorithm>
#include <assert.h>
#include <bitset>
#include <sstream>
using namespace std;

// Hàm dựng mặc định
HuffmanNode::HuffmanNode()
	: value(0), weight(0), order(0), left(nullptr), right(nullptr), parent(nullptr), isNYT(false)
{}
// Hàm dựng 7 tham số
HuffmanNode::HuffmanNode(int value, int weight, int order, HuffmanNode* left, HuffmanNode* right,
	HuffmanNode* parent, bool isNYT)
	:value(value), weight(weight), order(order), left(left), right(right), parent(parent), isNYT(isNYT)
{}

// Hàm dựng mặc định class HuffmanTree
HuffmanTree::HuffmanTree()
	: root(new HuffmanNode(-1, 0, 512, nullptr, nullptr, nullptr, true))
{
	NYTNode = root;
	for (int i = 0; i < 257; i++)
	{
		Table[i] = false;
	}
}

// Thêm ký tự vào cây và cập nhật lại cây
void HuffmanTree::UpdateTree(int symbol)
{
	HuffmanNode* symbolNode = GetSymbolNode(symbol, root);
	if (symbolNode == nullptr)
	{
		NYTNode->isNYT = false;
		int currentMinOrder = NYTNode->order;
		NYTNode->left = new HuffmanNode(-1, 0, currentMinOrder - 2, nullptr, nullptr, NYTNode, true);
		NYTNode->right = new HuffmanNode(symbol, 0, currentMinOrder - 1, nullptr, nullptr, NYTNode, false);
		symbolNode = NYTNode->right;
		NYTNode = NYTNode->left;
	}

	SlideAndIncrement(symbolNode);
}

//string HuffmanTree::Encode(int symbol)
//{
//	HuffmanNode* symbolNode = GetSymbolNode(symbol, root);
//	if (symbolNode != nullptr)
//	{
//		std::string result = GetPathToSymbol(root, symbolNode, "");
//		UpdateTreeModel(symbol);
//		return result;
//	}
//
//	std::stringstream ss;
//	ss << GetPathToSymbol(root, NYTNode, "") << std::bitset<8>(symbol);
//
//	UpdateTreeModel(symbol);
//	return ss.str();
//}
// Mã hóa 1 ký tự
string HuffmanTree::Encode(int symbol)
{
	//HuffmanNode* symbolNode = GetSymbolNode(symbol, root);
	if (Table[symbol]==true)
	{
		std::string result = GetPathToSymbol(root, symbol, "");
		UpdateTree(symbol);
		return result;
	}

	std::stringstream ss;
	ss << GetPathToSymbol(root, NYTNode, "") << std::bitset<8>(symbol);
	Table[symbol] = true;
	UpdateTree(symbol);
	return ss.str();
}

// Giải mã đoạn mã
int HuffmanTree::Decode(string& inputStr)
{
	int result = -1;
	HuffmanNode* crr = root;
	while (result == -1)
	{
		if (crr->value != -1)
		{
			result = crr->value;
			UpdateTree(crr->value);
			crr = root;
		}
		else if (crr->isNYT)
		{
			string temp = inputStr.substr(0, 8);
			inputStr = inputStr.substr(8);
			int number = binary_to_decimal(temp);
			result = number;
			UpdateTree(number);
			crr = root;
		}
		else if (inputStr.front() == '0')
		{
			crr = crr->left;
			inputStr = inputStr.substr(1);
		}
		else {
			crr = crr->right;
			inputStr = inputStr.substr(1);
		}
	}

	return result;
}

HuffmanTree::~HuffmanTree()
{
	DeleteHuffmanTree(root);
}

// Hàm tìm 1 ký tự trong cây
HuffmanNode* HuffmanTree::GetSymbolNode(int symbol, HuffmanNode* crr) const
{
	if (crr == nullptr || crr->value == symbol)
	{
		return crr;
	}

	HuffmanNode* leftResult = GetSymbolNode(symbol, crr->left);

	return leftResult == nullptr ? GetSymbolNode(symbol, crr->right) : leftResult;
}

// Tìm nút xa nút vi phạm nhất
void HuffmanTree::FindBlockLeader(HuffmanNode* crr, HuffmanNode*& crrMax) const
{
	if (crr == nullptr || crrMax == root)
	{
		return;
	}

	if (crr->weight == crrMax->weight && crr != crrMax->parent && crr->order > crrMax->order)
	{
		crrMax = crr;
	}

	FindBlockLeader(crr->left, crrMax);
	FindBlockLeader(crr->right, crrMax);
}

// Hàm đổi chỗ 2 nút
void HuffmanTree::SwapNodes(HuffmanNode* first, HuffmanNode* second)
{
	if (first->parent == nullptr || second->parent == nullptr) return;

	if (first->parent == second || second->parent == first) return;

	HuffmanNode*& firstRef = first->parent->left == first ? first->parent->left : first->parent->right;
	HuffmanNode*& secondRef = second->parent->left == second ? second->parent->left : second->parent->right;

	std::swap(firstRef, secondRef);
	std::swap(firstRef->parent, secondRef->parent);
	std::swap(firstRef->order, secondRef->order);
}

// Hàm lấy mã của 1 nút (Qua trái: 0, qua phải: 1)
string HuffmanTree::GetPathToSymbol(HuffmanNode* crr, HuffmanNode* result, string currentPath) const
{
	if (crr == result)
	{
		return currentPath;
	}
	if (crr == nullptr)
	{
		return "";
	}

	string left = GetPathToSymbol(crr->left, result, currentPath + "0");
	return left != "" ? left : GetPathToSymbol(crr->right, result, currentPath + "1");
}

// Hàm lấy mã của 1 ký tự (Qua trái: 0, qua phải: 1)
string HuffmanTree::GetPathToSymbol(HuffmanNode* crr, int result, std::string currentPath) const
{
	
	if (crr == nullptr)
	{
		return "";
	}
	if (crr->value == result)
	{
		return currentPath;
	}
	std::string left = GetPathToSymbol(crr->left, result, currentPath + "0");
	return left != "" ? left : GetPathToSymbol(crr->right, result, currentPath + "1");
}

//Hàm tăng trọng số của nút, điều chỉnh lại cây
void HuffmanTree::SlideAndIncrement(HuffmanNode* node)
{
	if (node == nullptr)
	{
		return;
	}

	HuffmanNode* blockLeader = node;
	FindBlockLeader(root, blockLeader);
	if (blockLeader != node)
	{
		SwapNodes(blockLeader, node);
	}

	++node->weight;
	SlideAndIncrement(node->parent);
}

// Hàm delete cây
void HuffmanTree::DeleteHuffmanTree(HuffmanNode* crrNode)
{
	if (crrNode == nullptr) return;

	DeleteHuffmanTree(crrNode->left);
	DeleteHuffmanTree(crrNode->right);
	delete crrNode;
}

// Hàm nét 1 file
void HuffmanTree::EncodingFile() {
	HuffmanTree encodeTree;
	string bits;
	string infile_name;
	string encode;
	unsigned char temp;
	char custom;
	cout << "Nhap ten file can nen: ";
	rewind(stdin);
	cin >> infile_name;
	ifstream infile;
	ofstream outfile;
	infile.open(infile_name);
	if (infile.fail()) {
		cout << "Khong the mo file!" << endl;
		return;
	}
	string outfile_name = infile_name;
	outfile_name.replace(outfile_name.find("."), outfile_name.size() - outfile_name.find("."), ".huf");
	outfile.open(outfile_name, ios::out | ios::binary);
	if (!outfile.good())
	{
		perror("Error:\t");
		exit(-1);
	}
	temp = infile.get();
	while (!infile.eof()) {
		encode += encodeTree.Encode(temp);
		temp = infile.get();
	}
	cout << "Sap xong roi^^" << endl;
	custom = encode.size() % 8;
	outfile.write(reinterpret_cast<const char*>(&custom), sizeof(char));
	if (custom > 0) {
		for (int i = custom; i < 8; i++) {
			encode.push_back('0');
		}
	}
	while (encode.size() > 0) {
		bits = encode.substr(0, 8);
		encode = encode.substr(8);
		temp = binary_to_decimal(bits);
		outfile.write(reinterpret_cast<const char*>(&temp), sizeof(char));
	};
	
	infile.close();
	outfile.close();
	cout << "Thanh cong! Ten file nen: " << outfile_name << endl;
}
// Hàm giải nén 1 file
void HuffmanTree::DecodingFile() {
	HuffmanTree decodeTree;
	string infile_name;
	string outfile_name;
	string decode;
	string temp;
	string encode;
	unsigned char symbol;
	char custom;
	rewind(stdin);
	cout << "Nhap ten file can giai nen: ";
	rewind(stdin);
	getline(cin, infile_name);
	ifstream infile(infile_name, ios::in | ios::binary);
	ofstream outfile;
	if (infile.fail()) {
		cout << "File khong ton tai hoac khong the mo file!" << endl;
		return;
	};
	//Lấy giá trị custom(Số bit bị thừa)
	infile.read(reinterpret_cast<char*>(&custom), sizeof(char));

	infile.read(reinterpret_cast<char*>(&symbol), sizeof(char));
	while (!infile.eof()) {
		encode += decimal_to_binary(symbol);
		infile.read(reinterpret_cast<char*>(&symbol), sizeof(char));
	}
	infile.close();
	for (int i = custom; i < 8; i++) {
		encode.pop_back();
	}
	outfile_name = infile_name;
	outfile_name.replace(outfile_name.find("."), outfile_name.size() - outfile_name.find("."), "(encode).txt");
	outfile.open(outfile_name);
	if (!outfile.good())
	{
		perror("Error:\t");
		exit(-1);
	}
	while (encode.size()>0)
	{
		char symbolAscii = decodeTree.Decode(encode);
		outfile << symbolAscii;
		
	};
	outfile.close();
	cout << "Thanh cong! Ten file: " << outfile_name << endl;
}

// Chuyển từ hệ 10 sang hê 2(int->string)
string decimal_to_binary(int in)
{
	string temp = "";
	string result = "";
	while (in)
	{
		temp += ('0' + in % 2);
		in /= 2;
	}
	result.append(8 - temp.size(), '0');
	for (int i = temp.size() - 1; i >= 0; i--)
	{
		result += temp[i];
	}
	return result;
}
// Chuyển từ hệ 2 sang hệ 10(string->int)
int binary_to_decimal(string& in)
{
	int result = 0;
	for (int i = 0; i < in.size(); i++)
		result = result * 2 + in[i] - '0';
	return result;
}
