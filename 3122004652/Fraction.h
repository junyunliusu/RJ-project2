#pragma once
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <fstream>
using namespace std;
typedef long long ll;
const int operatorLimit = 3; //根据需要可以调整算式运算符限制
int numLimit, countLimit;//控制台参数:数字大小限制，输出算式个数
class Fraction {
private:
	ll numerator, denominator;//用假分数存储
	void reduction() {//通分
		if (denominator == 0) return;//分母为零调整失败
		ll numsign = 1, densign = 1;
		if (numerator < 0 && denominator < 0)//将分数变为绝对值
			numerator *= -1, denominator *= -1;
		else if (numerator < 0) {
			numsign *= -1;	numerator *= -1;
		}
		else if (denominator < 0) {
			densign *= -1;  denominator *= -1;
		}
		ll gcd = numerator, tmp, tmpdenominator = denominator;
		while (tmpdenominator != 0) {//迭代求gcd,特殊情况:gcd(0,x) = x//辗转相除法
			tmp = gcd;
			gcd = tmpdenominator;
			tmpdenominator = tmp % tmpdenominator;
		}
		numerator /= gcd; denominator /= gcd;
		numerator *= numsign; denominator *= densign;
	}
public:
	Fraction() {//无参构造初始化全0
		numerator = 0; denominator = 0;
	}
	Fraction(ll tmpnumerator, ll tmpdenominator) {//有参构造
		numerator = tmpnumerator; denominator = tmpdenominator;
		reduction();
	}
	Fraction(const Fraction& x) {//拷贝构造
		numerator = x.numerator; denominator = x.denominator;
	}
	void random_init() {
		denominator = rand() % numLimit + 1;//[1 ~ 10]
		numerator = rand() % (denominator * numLimit) + 1;//分子/分母 (0 ~ 10]
		reduction();
	}
	bool error_check() {
		return denominator == 0;
	}
	string write() {//数据输出
		if (error_check()) return "NaN";//无意义
		string ans;
		ll de = numerator / denominator, tmp = numerator - de * denominator;
		if (numerator >= denominator) {
			ans += to_string(de);
		}
		if (de == 0 && tmp == 0) {
			ans += "0";
		}
		else if (de != 0 && tmp != 0) {
			ans += "'" + to_string(tmp) + '/' + to_string(denominator);
		}
		else if (de == 0 && tmp != 0) {
			ans += to_string(tmp) + '/' + to_string(denominator);
		}
		return ans;
	}
	Fraction operator +(Fraction x) {
		Fraction ans;
		ans.denominator = denominator * x.denominator;
		ans.numerator = numerator * x.denominator + x.numerator * denominator;
		ans.reduction();
		return ans;
	}
	Fraction operator -(Fraction x) {
		Fraction ans;
		ans.denominator = denominator * x.denominator;
		ans.numerator = numerator * x.denominator - x.numerator * denominator;
		ans.reduction();
		return ans;
	}
	Fraction operator *(Fraction x) {
		Fraction ans;
		ans.numerator = numerator * x.numerator;
		ans.denominator = denominator * x.denominator;
		ans.reduction();
		return ans;
	}
	Fraction operator /(Fraction x) {
		Fraction ans;
		ans.numerator = numerator * x.denominator;
		ans.denominator = denominator * x.numerator;
		ans.reduction();
		return ans;
	}
	bool operator <(Fraction x) {
		return numerator * x.denominator < x.numerator * denominator;
	}
	bool operator <(ll x) {
		return numerator < x * denominator;
	}
};

Fraction stringToFraction(string s) {//不支持负数
	size_t i = 0, j = 0;
	ll integer = 0, numerator = -1, denominator;
	for (; i < s.size(); i++) {//取数据整数部分
		if (s[i] == '\'') {
			integer = stoll(s.substr(0, i));
			break;
		}
		if (s[i] == '/') {//取数据分数分子
			numerator = stoll(s.substr(0, i));
			break;
		}
	}
	if (numerator == -1 && i < s.size()) {//若无分数，向后寻找
		j++;
		for (; i + j < s.size(); j++) {
			if (s[i + j] == '/') {
				numerator = stoll(s.substr(i + 1, j - 1));
				break;
			}
		}
		denominator = stoll(s.substr(i + j + 1));
	}
	else if (numerator == -1 && i == s.size()) {
		integer = stoll(s.substr(0));
		numerator = 0;
		denominator = 1;
	}
	else {
		denominator = stoll(s.substr(i + j + 1));
	}
	return Fraction(integer * denominator + numerator, denominator);//读取数据
}

Fraction autoCal(Fraction x, string op, Fraction y) {//运算
	if (op == "+") return x + y;
	else if (op == "-") return x - y;
	else if (op == "*") return x * y;
	else if (op == "÷") return x / y;
	else return Fraction();//运算符错误,返回空分数类
}

int getPriority(string x) {//得到运算符优先级
	if (x == "+") return 1;
	else if (x == "-") return 1;
	else if (x == "*") return 2;
	else if (x == "÷") return 2;
	else return 0;//未知字符默认优先级最低
}