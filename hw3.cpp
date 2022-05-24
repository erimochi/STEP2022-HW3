#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm> // copy()
using namespace std;


vector<pair<char, double> > decompose(const string formula);
double calculate( vector<pair<char, double> >  elements);
double calculate_with_brackets( vector<pair<char, double> >  elements);
pair< pair<char, double>, int > read_element(const string formula, int i);
int char_is_number(const char c);


int main(){
	cout << "input the formula:" << endl; 
	string formula;
	getline(cin, formula);
	vector < pair < char, double >  > elements = decompose(formula);
	double ans = calculate_with_brackets(elements); 
	cout << "Answer : " << ans << endl;
	return 0;
}


vector<pair<char, double> > decompose(const string formula){
	int len = formula.length(); 
	vector< pair< char, double > > v; // type of elements and (if need) numerical value
	
	// read each elements
	for (int i = 0; i<len; i++){
		pair< pair<char, double>,  int > p = read_element(formula, i);
		v.push_back(p.first);
		i = p.second;
	}
	
	// memorize companion braket's index as numerical value of pair
	vector < int > bracket_start; //  memorize indices of '(' as a stack
	int start_index = 0;
	int vlen = v.size();
	for(int j=0; j<vlen; j++){
		if(v[j].first == '('){bracket_start.push_back(j);}
		else if (v[j].first == ')'){
			start_index = bracket_start.back(); 
			bracket_start.pop_back(); 
			v[j].second = (double)start_index;
			v[start_index].second = (double)j;
		}
	}
	return v;
}


pair< pair<char, double>, int > read_element(const string formula,  int i){
	/*
	return pair< pair<char, double>, int > p
	 	pair<char, double>:
			char: explanation('n':number, 's':space, '+', '-', '*', '/')
			double: scalar (in case of 'n'), or companion braket's index(in case of '(', ')' )
		int: next index of formula (from formula[0] to formula[i] have read)
	*/
	pair< char, double > p_sub; 
	double scalar = 0; 
	char c = formula[i]; // currently read char
	double c_double = 0; // currently read (double)char
	int is_fractional = 0; // integer or fractopnal part
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
		p_sub = make_pair('n', scalar);
		i --; 
	}else if (c =='+' || '-' || '*' || '/'){
		p_sub = make_pair(c, scalar);
	}else if(c =='(' || ')'){ 
		// numerical value is changed in function decompose()
		p_sub = make_pair(c, scalar);
	}else{
		// Other character (not a number,'+', '-', '*', '/', ' ', ')' or '(' )
		cout << c << " in given formula is invalid character." << endl; exit(1);
	}
	return make_pair(p_sub, i);
}


int char_is_number(const char c){
	if(48 <= (int)c && (int)c <= 57){return (int)c - 48;}
	else{return -1;}
	return -1;
}


double calculate_with_brackets( vector<pair<char, double> >  elements){
	// search brackets and resursively calculate
	double ans_in_brackets = 0;
	int shorten = 0; 
	
	for(int i=0; i<elements.size(); i++){ 
		// cout << "loop:" << i << " in " << elements.size() << " : " <<  elements[i].first << elements[i].second << endl;
		if(elements[i].first == '('){
			// brackets exist
			vector<pair<char, double> >  elements_sub; // formula between brackets
			copy(elements.begin()+i-shorten+1, elements.begin()+(int)elements[i].second-shorten, back_inserter(elements_sub));
			elements.erase(elements.begin()+i-shorten+1, elements.begin()+(int)elements[i].second-shorten+1); // erase ( "~)"
			ans_in_brackets = calculate_with_brackets(elements_sub);
			elements[i] = make_pair('n', ans_in_brackets); // replace '(' with answer in brackets
			shorten += (int)elements[i].second - i;
		}
	}
	
	// no brackets
	return calculate(elements);
}


double calculate( vector<pair<char, double> >  elements){
	double ans = 0;
	int len = elements.size();
	char operation;
	// for (int i=0; i<len; i++){cout <<"calculate: "<< elements[i].first << elements[i].second << endl;} // for check
	
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

/*
疑問点
不正な文字(#など)の入力時、exit(1)に誘導されるはずだがされていない......?

*/