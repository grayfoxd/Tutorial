#include<iostream>
#include<fstream>
#include<sstream>
#include<stdio.h>
//#include<assert.h>

using namespace std;

class JpgToLora{
	public:
		struct Frame{
			char head;
			char length;
			char comand;
			short num;
			char data[254];
			char cs;
			char tail;
		}frame;
		
		
		void addJpgtoFrame(){
			int i;
			int sum =0;
			frame.head = 0xA5;
			frame.tail = 0x5A;
			frame.num = 0;
			
			ofstream outFile("frame",ios::binary);
			if(!outFile){
				cout<<"file open error!"<<endl;
				//return 0;
			}
			//
			ifstream inFile("invasion.jpg",ios::binary);
			if(!inFile){
				cout<<"frame open error!"<<endl;
				//return 0;
			}
			inFile.seekg(0,ios::beg);
			//outFile.seekg(0,ios::beg);
			streampos pos = inFile.tellg();
			if(!inFile.eof()){
				frame.num++;
				inFile.read(frame.data,sizeof(frame.data));
				frame.length = 0xFF;
				outFile.write(reinterpret_cast<char *>( &frame), sizeof(frame));
			}else{
				frame.num++;
				frame.length = pos%254;
				inFile.read(frame.data,sizeof(frame.length));
				outFile.write(reinterpret_cast<char *>( &frame),sizeof(frame.data)+7);
			}
			inFile.close();
			outFile.close();
		}
		int main(){
			JpgToLora();
			return 0;
		}
}; 
