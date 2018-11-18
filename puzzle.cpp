#include <SFML/Window.hpp> 
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#include "windows.h"

#define TILESIZE 128
using namespace std;


void location(sf::Sprite &item, int y, int n1){	
	item.setPosition((y%n1) * TILESIZE, (y/n1) * TILESIZE);
}

void size_pic(int &n1, int &n2, char arr2[]){
    unsigned short int d, pic_height, pic_width;
    int i = 1;
    while(i){
  		cout << "Input" << endl;
  		char arr[256];
  		char arr3[256] = "resources/";
  		cin >> arr;
  		strcat(arr3, arr);
    	std::ifstream ifs(arr3, std::ifstream::binary);
    	if (ifs){ 
			ifs.read((char *)&d, 2);
   			if (d != 0xD8FF){ 
				printf("ERROR_2\n"); 
				return;
			}
    		while (d != 0xC0FF && d != 0xC1FF && d != 0xC2FF && !ifs.eof()){
	    	    ifs.read((char *)&d, 2);
 	
    		    if (d == 0xD9FF) break;
 	
        		if (d >= 0xD0FF && d <= 0xD7FF) continue;
 
        		if (d >= 0xE0FF && d <= 0xEFFF){
	    	        ifs.read((char *)&d + 1, 1);
    	    	    ifs.read((char *)&d, 1);
           			ifs.ignore(d - 2);
           			continue;
        		}
 
    	    switch (d)
    	    {
    	    case 0xDDFF:
    	        ifs.ignore(4);
    	        break;
   		    case 0xC4FF:
    	    case 0xDBFF:
    	    case 0xDAFF:
    	    case 0xFEFF:
    	        ifs.read((char *)&d + 1, 1);
    	        ifs.read((char *)&d, 1);
    	        ifs.ignore(d - 2);
    	    }
    	}
 
 	    	switch (d)
 	    	{
 	    	case 0xC0FF:
 	    	case 0xC1FF:
 	   		case 0xC2FF:
 	    	   ifs.ignore(2 + 1);
 	    	   ifs.read((char *)&pic_height + 1, 1);
 	    	   ifs.read((char *)&pic_height, 1);
 	    	   ifs.read((char *)&pic_width + 1, 1);
 	    	   ifs.read((char *)&pic_width, 1);
 	   		    break;
 	   		}
 	    	n2 = pic_height;
 	    	n1 = pic_width;
 	    	ifs.close();
 	    	cout << n1 << "   " << n2 << endl;
 	    	if(n1 <= 2000 && n2 <= 1000){
 	    		for(int j = 0; j < 256; j++)
	 	    		arr2[j] = arr3[j];
				i = 0;
			 }
		}
	}
    
}

int main(){
	
	//скрыть консоль
	//HWND hWnd = GetConsoleWindow();
    //ShowWindow(hWnd, SW_HIDE);
    
  	char arr2[256];
	int n1, n2;
	size_pic(n1, n2, arr2);
	if(n1 % TILESIZE > TILESIZE/2)
		n1 = n1/TILESIZE + 1;
	else
		n1 = n1/TILESIZE;
	if(n2 % TILESIZE > TILESIZE/2)
		n2 = n2/TILESIZE + 1;
	else
		n2 = n2/TILESIZE;
	int N = n1*n2;
	
    // создать окно
    sf::RenderWindow window(sf::VideoMode(n1 * TILESIZE, n2 * TILESIZE), "Puzzle");
	window.setKeyRepeatEnabled(false);
    int level[N];
    sf::Texture figur[N];  
	sf::Sprite figu[N]; 
    for(int i = 0; i < N; i++){
    	if (!figur[i].loadFromFile(arr2, sf::IntRect((i%n1)*TILESIZE, (i/n1)*TILESIZE, TILESIZE, TILESIZE)))
        	return -1;	
        figu[i].setTexture(figur[i]);
	}
	
    for(int i = 0; i < N; i++){
    	level[i] = i;
	}
	srand(time(NULL));
	for(int i = 0; i < N*2; i++){
		int r = rand()%N;
		int r1 = rand()%N;
		swap(level[r], level[r1]);
	}
	int game = 1;
	int press = 0;
	int j,j1, l,l1;
    // run the main loop
    int end = 0;
    
	window.clear();  
	for(int i = 0; i < N; i++){
		location(figu[i], level[i], n1);
		window.draw(figu[i]);
	}		
    window.display();
    while (window.isOpen()){
    	
        sf::Event event;
        while (window.pollEvent(event)){
        	if(event.type == sf::Event::Closed)
                window.close();	  
     
       		if(event.type == sf::Event::MouseButtonPressed){
       			if(game == 1){ 
        			if(press == 0){
        				j = event.mouseButton.x/TILESIZE  + event.mouseButton.y/TILESIZE*n1;
        				for(int k = 0; k < N; k++){
        					if(level[k] == j){
        						l = k;
        						break;
							}
						}
        				press = 1;
					}else{
        				j1 = event.mouseButton.x/TILESIZE  + event.mouseButton.y/TILESIZE*n1;
        				for(int k = 0; k < N; k++){
        					if(level[k] == j1){
        						l1 = k;
        						break;
							}
						}
						swap(level[l],level[l1]); 	
						press = 0;
						end = 1;
						for(int i = 1; i < N; i++){
							if(level[i] < level[i-1]){
								end = 0;
							}
    					} 
						if(end == 1){
       						 	printf("You Win");
       						 	game = 0;
						}	
					}
				}  	
				window.clear();  
				for(int i = 0; i < N; i++){
					location(figu[i], level[i], n1);
					window.draw(figu[i]);
				}		
      			window.display();
			}
		}
		        	
		
   		
	} 
    	
    return 0;
}
