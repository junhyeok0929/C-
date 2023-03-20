#include<iostream>
#include<cstring>
using namespace std;

int main() {
	char a[100] = "안녕하세요?";
	char b[100] = "날씨가 참 좋군요.";
	char c[100] = "안녕히 계세요!";

	cout << "함수 사용" << endl;
	cout << strlen(a) << " " << strlen(b) << " " << strlen(c) << endl;
	cout << "반복문으로 계산" << endl;

	int count1 = 0, count2 = 0, count3 = 0;
	count1 = 0;
	while (a[count1] != NULL){
		count1++;
	}
	count2 = 0;
	while (b[count2] != NULL){
		count2++;
	}
	count3 = 0;
	while (c[count3] != NULL){
		count3++;
	}
	cout << count1 << " " << count2 << " " << count3;
	
	return 0;

}
