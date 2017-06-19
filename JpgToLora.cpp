#include<iostream>
#include<fstream>
#include<sstream>
#include<stdio.h>

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
			ifstream inFile("invasion.jpg",ios::binary);
			if(!inFile){
				cout<<"frame open error!"<<endl;
				//return 0;
			}
			inFile.seekg(0,ios::beg);
			while(!(outFile.eof())){
				for(i=0;i<254;i++){
					frame.data[i] = inFile;
					frame.num++;
					inFile->next;
					sum+=frame.data[i];
					if(sum >= 0xFF){
						frame.cs = 0xFF;
					}else{
						frame.cs = sum;
					}
					while(frame){
						inFile<<frame;
					}
				}
			}
		}
}; 
