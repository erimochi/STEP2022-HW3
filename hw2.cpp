#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

vector<pair<string, double> > init_testcase();
int run_test(const vector<pair<string, double> > testcase);

vector<pair<char, double> > decompose(const string formula);
double calculate( vector<pair<char, double> >  elements);
pair< pair<char, double>, int > read_element(const string formula, int i);
int char_is_number(const char c);


int main(){
	vector<pair<string, double> > testcase = init_testcase();
	int perfect = run_test(testcase);
	if(perfect){cout << "perfect!" << endl;}
	return 0;
}


vector<pair<string, double> > init_testcase(){
	vector<pair<string, double> > testcase;
	testcase.push_back(make_pair("2*3", 6.0));
	testcase.push_back(make_pair("4*7*1/6+5+13", 22.6667));
	testcase.push_back(make_pair("1.0", 1.0));
	testcase.push_back(make_pair("0.3/1.4", 0.2143));
	return testcase;
}


int run_test(const vector<pair<string, double> > testcase){
	int perfect = 1;
	int len = testcase.size();
	double ans = 0;
	string formula;
	double ans_expect = 0;
	double error = 1e-3;
	for(int i=0; i<len; i++){
		formula = testcase[i].first;
		ans_expect = testcase[i].second;
		double ans = calculate(decompose(formula)); 
		cout << formula << " = " << ans <<" (expected: " << ans_expect << ")" << endl;
		if(abs(ans_expect-ans) > error){perfect = 0;}
	}
	return perfect;
}

//        ===== copy from hw1.cpp =====

vector<pair<char, double> > decompose(const string formula){
	int len = formula.length(); 
	vector< pair< char, double > > v;
	for (int i = 0; i<len; i++){
		pair< pair< char, double >, int > p = read_element(formula, i);
		v.push_back(p.first); 
		i = p.second; //change the index
	}
	return v;
}

pair< pair<char, double>, int > read_element(const string formula, int i){
	/*
	return pair< pair<char, double>, int > p
		char: explanation('n':number, 's':space, '+', '-', '*', '/')
		double: scalar (in case of 'n')_
		int: next index of formula
	*/
	pair< pair< char, double >, int > p;
	pair< char, double > p_sub;
	double scalar = 0;
	p_sub.first = 's';
	char c = formula[i];
	double c_double = 0;
	int is_fractional = 0;
	while(c==' ' && i< formula.length() ){i++;c = formula[i];} // pass spaces
	c_double = (double)char_is_number(c);
	if(c_double!= -1){
		// read number
		is_fractional = 0;
		while(c_double!= -1 || c=='.'){
			
			if(is_fractional){
				// fractional part
				scalar += c_double/pow(10,is_fractional); is_fractional++;	
			}else if(c=='.'){
				is_fractional =1;
			}else{
				// integer part
				scalar *= 10; scalar += c_double;
			}
			i++; c = formula[i]; c_double = (double)char_is_number(c);
		}
		p_sub.first = 'n';
		p_sub.second = scalar;
		i --; 
		
	}else if (c =='+' || '-' || '*' || '/'){
		p_sub.first = c;
		p_sub.second = scalar;
	}else{
		// Other character (not a number,'+', '-', '*', '/', ' ')
		cout << c << " in given formula is invalid character." << endl; exit(1);
	}
	
	p.first = p_sub;
	p.second = i;
	return p;
	
}

int char_is_number(const char c){
	if(48 <= (int)c && (int)c <= 57){return (int)c - 48;}
	else{return -1;}
	return -1;
}


double calculate( vector<pair<char, double> >  elements){
	double ans = 0;
	int len = elements.size();
	char operation;
	// for (int i=0; i<len; i++){cout << elements[i].first << elements[i].second << endl;} // for check
	
	// calc '*', '/'
	for (int i=0; i<len; i++){
		operation = elements[i].first;
			if(operation == '*' ){
				elements[i-1].second = elements[i-1].second * elements[i+1].second;
				elements.erase(elements.begin()+i); // delete elements[i];
				elements.erase(elements.begin()+i); // delete elements[i+1];
				i--; len = len - 2;
			}else if(operation == '/'){
				elements[i-1].second = elements[i-1].second / elements[i+1].second;
				elements.erase(elements.begin()+i); // delete elements[i];
				elements.erase(elements.begin()+i); // delete elements[i+1];
				i--; len = len-2;
			}
	}

	//for (int i=0; i<len; i++){cout << elements[i].first << elements[i].second << endl;} // for check

	//calc '+', '-'
	int plus = 1; // add or subtract
	for (int i=0; i<len; i++){
		operation = elements[i].first;
		if(operation == '+' ){plus = 1;}
		else if(operation == '-' ){plus = 0;}
		else{
			if(plus){ans +=  elements[i].second;}
			else {ans -=  elements[i].second;}
		}
	}
	
	return ans;
}