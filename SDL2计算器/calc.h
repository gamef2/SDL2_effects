#ifndef _CALC_
#define _CALC_

#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cmath>
using namespace std;

const char dict[] = {"mpSsca!~*/%+-lr&|^"};
const char lev[]={0,0,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
// 测试优先级
bool test_prior(char a, char b)
{
	return lev[string(dict).find_first_of(a) ]<lev[string(dict).find_first_of(b)];
	/* int ap=-1,bp=-1; for(int i=0;i<strlen(dict);++i) if(a==dict[i]) { ap=i;
	   break; } for(int i=0;i<strlen(dict);++i) if(b==dict[i]) { bp=i; break; }


	   return ap<=bp; */
}

string Scanner(string & s)
{
	string buf;
	stack<char> oper;
	stack < char >sk;
	int i = 0;
	while (i < s.size())
	{
		if (s[i] >= '0' && s[i] <= '9' || s[i] == '.')
		{
			buf += s[i];
			cout << "输出" << s[i] << endl;
		}
		else if (s[i] == '(')
		{
			sk.push(s[i]);
			oper.push(s[i]);
		}
		else if (s[i] == ')')
		{
 			if (oper.empty())
 				throw runtime_error(u8"缺少左括号");
			oper.pop();
			int parentheses = 0;
			while (sk.top() != '(')
			{
				buf +=
				{
				' ', sk.top()};
				cout << sk.top() << "出栈" << endl;
				sk.pop();
				if (sk.empty())
					throw runtime_error(u8"输入错误");
				parentheses = 1;
			}
// 			if (parentheses==0)
// 				throw runtime_error(u8"没有左括号");
			sk.pop();
		}
		else if (string(dict).find_first_of(s[i]) != string::npos)
		{
			buf += ' ';
			if(i==0||string(dict).find_first_of(s[i-1]) != string::npos)
			{
				if(s[i]=='+')
				s[i]='p';
				if(s[i]=='-')
				s[i]='m';
			}
			while (!sk.empty() && sk.top() != '(' && s[i] != '(' && test_prior(sk.top(), s[i]))
			{
				buf +=
				{
				sk.top(), ' '};
				cout << "遇到" << s[i] << "  " << sk.top() << "出栈并输出" << endl;
				sk.pop();
			}
			cout << s[i] << "入栈" << endl;
			sk.push(s[i]);
		}
		++i;
	}
	if (!oper.empty())
		throw runtime_error(u8"缺少右括号");
	while (!sk.empty())
	{
		cout << sk.top() << "出栈并输出" << endl;
		buf +=
		{
		' ', sk.top()};
		// cout<<sk.top()<<endl;
		sk.pop();
	}
	return buf;
}

template<typename T>
string calc(string & s,T d,T e)
{
	stack < double >sk;
	//double d, e;

	size_t curPos = 0, t = 0;
	while (curPos < s.size())
	{
		if (isdigit(s[curPos]))
		{
			sk.push(stof(&s[curPos], &t));
			curPos +=t-1;
		//	cout<<curPos<<endl;
		}
		else if (string("*/%+-lr&|^").find_first_of(s[curPos]) != string::npos)
		{
			if (!sk.empty())
			{
				e = sk.top();
				sk.pop();
			}
			else
				throw runtime_error(u8"栈为空,没有要处理的数字");
			if (!sk.empty())
			{
				d = sk.top();
				sk.pop();
			}
			else
				throw runtime_error(u8"栈为空,没有要处理的数字");
			switch (s[curPos])
			{
			case '+':
				sk.push(d + e);
				break;
			case '-':
				sk.push(d - e);
				break;
			case '*':
				sk.push(d * e);
				break;
			case '/':
				if (e == 0)
				{
					throw runtime_error(u8"出错,除数为0");
				}
				sk.push(d / e);
				break;
			case '%':
				if (d == 0 && e == 0)
					throw runtime_error(u8"出错,除余为0");
				sk.push((int)d % (int)e);
				break;
				case 'l':
				sk.push((int)d << (int)e);
				break;
				case 'r':
				sk.push((int)d >> (int)e);
				break;
			case '&':
				sk.push((int)d & (int)e);
				break;
			case '|':
				sk.push((int)d | (int)e);
				break;
			case '^':
				sk.push((int)d ^ (int)e);
				break;
			}
		cout<<d<<" "<<s[curPos]<<" "<<e<<"="<<sk.top()<<endl;
		}
		else if (string("mpSsca!~").find_first_of(s[curPos]) != string::npos)
		{
			if (!sk.empty())
			{
				d = sk.top();
				sk.pop();
			}
			else
				throw runtime_error(u8"栈为空,没有要处理的数字");
			switch (s[curPos])
			{
			case 's':
				sk.push(sin(d));
				break;
			case 'c':
				sk.push(cos(d));
				break;
			case 'a':
				sk.push(atan(d));
				break;
			case '!':
				sk.push(!d);
				break;
			case '~':
				sk.push(~(int)d);
				break;
					case 'm':
				sk.push(-d);
				break;
			case 'p':
				sk.push(+d);
				break;
			case 'S':
				sk.push(sqrt(d));
				break;
			}
		cout<<s[curPos]<<" "<<d<<"="<<sk.top()<<endl;
		}
		curPos++;
	}
	if (sk.empty())
		throw runtime_error(u8"输入错误");
		s = to_string(sk.top());
		sk.pop();
	return s;
}

#endif