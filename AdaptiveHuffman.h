#pragma once
#include<iostream>
#include <string>
#include<fstream>
using namespace std;

class HuffmanNode
{
	int value;    // Ký tự
	unsigned long long int weight;   //Tần số/trọng số xuất hiện
	int order;    // STT
	HuffmanNode* left;   // Nút con trái
	HuffmanNode* right;  // Nút con phải
	HuffmanNode* parent; // Nút cha
	bool isNYT;   // Có phải nút escape hay không?
	friend class HuffmanTree;
public:
	HuffmanNode(); // Hàm dựng mặc định
	HuffmanNode(int value, int weight, int order, HuffmanNode* left, HuffmanNode* right,
		HuffmanNode* parent, bool isNYT); // Hàm dựng 7 tham số
};

class HuffmanTree
{
private:
	HuffmanNode* NYTNode; // Giữ địa chỉ nút Escape
	HuffmanNode* root;    // Giữ địa chỉ nút gốc
	bool Table[257];      // Bảng xuất hiện các kí tự (true: kí tự đã có trong cây)
public:
	HuffmanTree(); // Hàm dựng mặc định
	void UpdateTree(int); // Thêm nút và chỉnh lại cây(nếu vi phạm tính chất anh em)
	string Encode(int);   // Mã hóa 1 ký tự
	HuffmanNode* GetSymbolNode(int , HuffmanNode*) const; // Tìm địa chỉ ký tự trong cây
	void FindBlockLeader(HuffmanNode*, HuffmanNode*&) const; // Tìm nút xa nút vi phạm nhất
	void SwapNodes(HuffmanNode*, HuffmanNode*); // Hoán đổi 2 nút
	string GetPathToSymbol(HuffmanNode*, HuffmanNode*, string) const; // Lấy mã của 1 nút 
	string GetPathToSymbol(HuffmanNode*, int, std::string) const;     // Lấy mã của ký tự
	void SlideAndIncrement(HuffmanNode*); // Tăng trọng số của nút và chỉnh lại cây
	void DeleteHuffmanTree(HuffmanNode*); // Hàm xóa cây
	int Decode(string&); // Hàm chuyển mã về ký tự
	~HuffmanTree(); // Hàm hủy
	void EncodingFile(); //Nén file
	void DecodingFile(); // Giải nén file
};
string decimal_to_binary(int in);  // Chuyển từ hệ 10 sang hê 2(int->string)
int binary_to_decimal(string& in); // Chuyển từ hệ 2 sang hệ 10(string->int)

