#include <iostream>
#include <string>
#include <algorithm>
#include <time.h>
#include <stdio.h>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <stack>
#include <list>
#include <Windows.h>
#include <WinSock2.h>
#include <hash_map>
#include <hash_set>
#include <map>
#include <thread>
namespace Node//All Data Need Request It
{
	/*
	WARNING:
	First Value Of Vector Must Be Title For Search!
	Even you can enable search FULL
	but it will slow.....very slow
	*/
    using namespace std;//use std
	//constexpr auto LineBegin = "BA7nlaFTmgxu6wds";//random string to show is begin
	auto EchoOfNode = 0;
	vector<string>b;//Use it to push data，Clear It after Use!
	vector<vector<string>>Data;
	void AddNodeVector(vector<string> &c)
	{
		Data.push_back(c);
	}
	void AddNode(int DataNumber/*How Much Data Need Input*/)
	{
		cout << "Please Input " << DataNumber << " Data,format is text\n";
		vector<string>c;
		for (int i = 0; i < DataNumber; i++)
		{
			cin >> c[i];
		}
		AddNodeVector(c);
		++EchoOfNode;//NextNode
	}
	void EraseNode(int th)
	{
		Data.erase(Data.begin() + th);
	}
	map<string, int>namemap;
	namespace InitMap
	{
		void InitMap()//排序/查找之前触发,单线程模型
		{
			namemap.clear();//clear map and rebuild
			for (int i = 0; i < EchoOfNode; i++)
			{
				namemap.insert(pair<string, int>(Data[i][0], i));
			}
		}
		DWORD WINAPI ThreadProc(LPVOID IpParameter)
		{
			InitMap();
			return 0L;
		}
		void InitMapMulitThread()//多线程模型
		{
			HANDLE thread = CreateThread(NULL, 0, ThreadProc, 0, NULL, NULL);
			CloseHandle(thread);
		}
	}
	bool comp(const vector<string>& c, const vector<string>& d)
	{
		return c[0] < d[0];
	}
	void SortNode()
	{
		sort(Data.begin(), Data.end(), comp);
		InitMap::InitMapMulitThread();//排完序后必须重建哈希表
	}
	int SearchNodeZero(string& a)
	{
		map<string, int>::iterator it;
		it = namemap.find(a);
		if (it != namemap.end())
		{
			return it->second;
		}
		else
			return -1;//unfind
	}
	vector<string> GetTheNode(int th)
	{
		return Data[th];
	}
	vector<int> stringToVecInt(const string& str)
	{
		union 
		{
			char c[2];
			int  i;
		} convert;
		convert.i = 0;
		vector<int> vec;
		for (unsigned i = 0; i < str.length(); i++)
		{
			if ((unsigned)str[i] > 0x80) 
			{
				convert.c[1] = str[i];
				convert.c[0] = str[i + 1];
				vec.push_back(convert.i);
				i++;
			}
			else
				vec.push_back(str[i]);
		}
		return vec;
	}
	bool include(const string& str, const string& msg)
	{
		auto sour = stringToVecInt(str);
		auto find = stringToVecInt(msg);
		return search(sour.begin(), sour.end(), find.begin(), find.end()) != sour.end();
	}
	int UnfullSearchNode(string& keyword)
	{
		for (int i = 0; i < EchoOfNode; i++)
		{
			if (include(Data[i][0], keyword))
				return i;
		}
		return -1;//unable to find
	}
	int SearchNode(string& keyword)
	{
		int a = 0;
		a = SearchNodeZero(keyword);//快速搜索
		if (a == -1)
		{
			a = UnfullSearchNode(keyword);//完整搜索
		}
		return a;
	}
	/*void TestFunc()
	{
		vector<string>::iterator it;
		for(it=Data[0].begin();)
	}*/
	void PrintNode(int th)
	{
		vector<string>::iterator it;
		for (it = Data[th].begin(); it != Data[th].end(); th++)
			cout << *it << " ";
	}
	vector<string>& splitUntemp(const string& str, char delim, vector<string>& elems, bool skip_empty = true) 
	{
		istringstream iss(str);
		for (string item; getline(iss, item, delim); )
			if (skip_empty && item.empty()) continue;
			else elems.push_back(item);
		return elems;
	}
	template <class _CharT, class _CharTraits, class _Alloc, class _OutputIter>
	void _split_worker(const basic_string<_CharT, _CharTraits, _Alloc>& __str,
		const basic_string<_CharT, _CharTraits, _Alloc>& __delims,
		_OutputIter   __first,
		bool          __skip_empty) noexcept
	{
		string::size_type __pos, __prev = 0;
		while ((__pos = __str.find_first_of(__delims, __prev)) != string::npos)
		{
			if (__pos > __prev) 
			{
				if (__skip_empty && __pos == 1 + __prev)
					break;
				*__first++ = __str.substr(__prev, __pos - __prev);
			}
			__prev = __pos + 1;
		}
		if (__prev < __str.size())
			*__first++ = __str.substr(__prev);
	}
	template <class _CharT, class _CharTraits, class _Alloc, class _OutputIter>
	void split(const basic_string<_CharT, _CharTraits, _Alloc>& __str,
		const basic_string<_CharT, _CharTraits, _Alloc>& __delims,
		_OutputIter                                      __first) noexcept
	{
		_split_worker(__str, __delims, __first, true);
	}
	template <class _CharT, class _CharTraits, class _Alloc, class _OutputIter>
	void split_with_no_skip_empty(const basic_string
		<
		_CharT, _CharTraits, _Alloc
		>& __str,
		const basic_string
		<
		_CharT, _CharTraits, _Alloc
		>& __delims,
		_OutputIter   __first) noexcept
	{
		_split_worker(__str, __delims, __first, false);
	}
	void ReadFileToMen(string FileName)
	{
		ifstream readf;
		readf.open(FileName.c_str());//OpenFile
		int a; readf >> a;//First Line Is The Number Of Data
		for (int i = 0; i < a&&readf.good()/*防止越界*/; i++)
		{
			string tmp;
			getline(readf, tmp);
			vector<string> result;
			splitUntemp(tmp, ' ', result);
			Data[i] = result;
		}
		readf.close();
	}
	void WriteDataToFIle(string FileName)
	{
		ofstream wfile;
		wfile.open(FileName.c_str());
		for (int i = 0; i < EchoOfNode; i++)
		{
			vector<string>::iterator it;
			it = Data[i].begin();
			while (it != Data[i].end())
			{
				wfile << *it++ << " ";
			}
			wfile << "\r\n";
		}
		wfile.close();
	}
}