#include <iostream>
#include <vector>
#include <cmath>
using namespace std;


vector<pair<char, double> > decompose(const string formula);
double calculate( vector<pair<char, double> >  elements);
pair< pair<char, double>, int > read_element(const string formula, int i);
int char_is_number(const char c);

int main(){
	cout << "input the formula:" << endl; 
	string formula;
	getline(cin, formula);
	vector < pair < char, double >  > elements = decompose(formula);
	double ans = calculate(elements); 
	cout << "Answer : " << ans << endl;
	return 0;
}


vector<pair<char, double> > decompose(const string formula){
	int len = formula.length(); 
	vector< pair< char, double > > v; 
	for (int i = 0; i<len; i++){
		pair< pair< char, double >, int > p = read_element(formula, i);
		v.push_back(p.first);
		i = p.second;
	}
	return v;
}

pair< pair<char, double>, int > read_element(const string formula, int i){
	/*
	return pair< pair<char, double>, int > p
		pair<char, double>:
			char: explanation('n':number, 's':space, '+', '-', '*', '/')
			double: scalar (in case of 'n')
		int: next index of formula  (from formula[0] to formula[i] have read)
	*/

	pair< char, double > p_sub;
	double scalar = 0;
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
	return make_pair(p_sub, i);	
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

/*
疑問点
"3 + 5    "(最後スペース)でも普通に計算できている
(→不正な文字のエラーに分岐していない？)
*/