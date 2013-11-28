// code to demonstrate the interface to the HypnoGadget library
// Copyright 2008 Chris Lomont 
// Visual Studio 2005 C++

// Compile as a console program

#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include <cctype>
#include <conio.h>
#include <time.h> 
#include "HypnoDemo.h" // include helper classes
#include "Gadget.h"    // include this to access the HypnoCube, HypnoSquare, etc.

using namespace std;
using namespace HypnoGadget; // the gadget interface is in this namespace

#pragma comment(lib,"winmm") // includes timeGetTime() from winmm.lib

//convert a vector<string> to vector<int>
vector<int> vecstr_to_vecint(vector<string> vs)
{
    vector<int> ret;
    for(vector<string>::iterator it=vs.begin();it!=vs.end();++it)
    {
        istringstream iss(*it);
        int temp;
        iss >> temp;
        ret.push_back(temp);
    }  
    return ret;
}

// Set a pixel in the square or cube buffer
// red, green, and blue are values in 0-255.
// i,j are 0-7 coordinates
// buffer is a 96 byte buffer to be sent to the gadget
void SetPixelSquare(
		int i, int j,        // coordinates
		unsigned char red,   // colors
		unsigned char green, // colors
		unsigned char blue,  // colors
		uint8 * buffer       // where to draw
		)
	{
	// each pixel on the gadget is RGB values 0-15, and packed
	// so each three bytes contains two pixels as in R1G1 B1R2 G2B2
	if ((i < 0) || (7 < i) || (j < 0) || (7 < j))
		return; // nothing to do

	// find first byte to set
	buffer += 12*j;      // 3/2 bytes per pixel * 8 pixels per row = 12 bytes per row
	buffer +=  3*(i>>1); // 3/2 bytes per pixel
	if (i&1) 
		{ // odd pixel, ..R2 G2B2 above
		++buffer;            // next byte
		*buffer &= 0xF0;     // mask out low bits
		*buffer |= (red>>4); // set high red nibble to low part of byte
		++buffer;            // next byte
		// set high green nibble and high blue nibble
		*buffer  = (green&0xF0)|(blue>>4); 
		}
	else
		{ // even pixel, R1G1 B2.. above
		// set high red nibble and high green nibble
		*buffer  = (red&0xF0)|(green>>4); 
		++buffer;            // next byte
		*buffer &= 0x0F;     // mask out high bits
		*buffer |= (blue&0xF0); // set high blue nibble to high part of byte
		}
	} // SetPixelSquare

// See above comments
void SetPixelCube(
		int i, int j, int k, // coordinates
		unsigned char red,   // colors
		unsigned char green, // colors
		unsigned char blue,  // colors
		uint8 * buffer       // where to draw
		)
	{
	// each pixel on the gadget is RGB values 0-15, and packed
	// so each three bytes contains two pixels as in R1G1 B1R2 G2B2
	if ((i < 0) || (3 < i) || (j < 0) || (3 < j) || (k < 0) || (3 < k))
		return; // nothing to do

	j = 3-j; // reverse j to get right hand coord system used in cube

	buffer += k*24; // 3/2 bytes per pixel * 16 pixels per level  = 24 bytes
	buffer += i*6;  // 3/2 bytes per pixel * 4  pixels per column =  6 bytes
	buffer += 3*(j>>1);   // 3/2 bytes per pixel

	if (j&1)
		{ // odd pixel, ..R2 G2B2 above
		++buffer;            // next byte
		*buffer &= 0xF0;     // mask out low bits
		*buffer |= (red>>4); // set high red nibble to low part of byte
		++buffer;            // next byte
		// set high green nibble and high blue nibble
		*buffer  = (green&0xF0)|(blue>>4); 
		}
	else
		{ // even pixel, R1G1 B2.. above
		// set high red nibble and high green nibble
		*buffer  = (red&0xF0)|(green>>4); 
		++buffer;            // next byte
		*buffer &= 0x0F;     // mask out high bits
		*buffer |= (blue&0xF0); // set high blue nibble to high part of byte
		}			   
	} // SetPixelCube

// Draw a frame of animation on the gadget
void DrawFrame(GadgetControl & gadget, const string & device, int num)
	{
	static int pos = 0; // position of pixel 0-63 - this drives this animation
	static int pos2 = 3;
	static int count = 0; // count for animation 2
	static int R = 255;
	static int G,B = 0; // color choie for animation 6

	uint8 image[96]; // RGB buffer, 4 bits per color, packed

	// clear screen by setting all values to 0
	memset(image,0,sizeof(image));
	
	if(num == 1){
	// Animation 1
	// draw a scrolling random colored pixel
	int x,y,z;  // coordinates
	x = pos/16; // convert pos 0-63 to x,y,z in [0,3]x[0,3]x[0,3]
	y = (pos/4)&3;
	z = pos&3;	


	/*SetPixelCube(2,2,z,rand(),rand(),rand(),image);
	SetPixelCube(1,1,z,rand(),rand(),rand(),image);
	SetPixelCube(1,2,z,rand(),rand(),rand(),image);
	SetPixelCube(2,1,z,rand(),rand(),rand(),image);
	*/

	for(int i = 0; i < 4; ++i){	
		if(x==0)
			SetPixelCube(z,0,i,0,0,255,image);
		else if(x==1){
			SetPixelCube(z,3,i,0,0,255,image);			
		}
		else if(x==2){
			SetPixelCube(0,z,i,0,0,255,image);
		}
		else{
			SetPixelCube(3,z,i,0,0,255,image);
		}
	}
	// next pixel for next frame of animation
	pos = (pos+1)&63; // count 0-63 and repeat
	// animation 1 ends
	}
	// animation 2
	else if(num == 2){
		if(count%2 == 0){
			for(int i = 1; i < 3; ++i){
				for(int j = 1; j < 3; ++j){
					for(int k = 1; k < 3; ++k)
						SetPixelCube(i,j,k,rand(),rand(),rand(),image);
				}
			}
		}
		else{
			for(int i = 0; i < 4; ++i)
				for(int j = 0; j < 4; ++j)
					for(int k = 0; k < 4; ++k)
					{
						if((i==1)&&(j==1)&&(k==1)||(i==1)&&(j==1)&&(k==2)||(i==1)&&(j==2)&&(k==1)
							||(i==1)&&(j==2)&&(k==2)||(i==2)&&(j==1)&&(k==1)||(i==2)&&(j==1)&&(k==2)
							||(i==2)&&(j==2)&&(k==1)||(i==2)&&(j==2)&&(k==2)){
							SetPixelCube(i,j,k,0,0,0,image);
						}
						else
							SetPixelCube(i,j,k,rand(),rand(),rand(),image);
					}
		}

		count++;
	}
	// animation 2 ends
	// animation 3
	else if(num == 3){
		srand(time(NULL));
		int i = rand() % 4;
		int j = rand() % 4;
		int k = rand() % 4;
		SetPixelCube(i,j,k,rand(),rand(),rand(),image);
		SetPixelCube(j,k,i,rand(),rand(),rand(),image);
		SetPixelCube(k,i,j,rand(),rand(),rand(),image);
		SetPixelCube(i,k,j,rand(),rand(),rand(),image);
		SetPixelCube(j,i,k,rand(),rand(),rand(),image);
	} // animation 3 ends
	// animation 4
	else if(num == 4){
		int x,y,z;  // coordinates
		x = pos/16; // convert pos 0-63 to x,y,z in [0,3]x[0,3]x[0,3]
		y = (pos/4)&3;
		z = pos&3;	
		
		for(int i = 0; i < 4; ++i){
			for(int j = 0; j < 4; ++j)
				SetPixelCube(z,j,i,rand(),rand(),rand(),image);
		}		
		pos = (pos+1)&63;
		
		int xi, yi, zi;
		xi = pos2/16;
		yi = (pos2/4)&3;
		zi = pos2&3;
		for(int i = 0; i < 4; ++i){
			for(int j = 3; j >= 0; --j)
				SetPixelCube(zi,j,i,rand(),rand(),rand(),image);
		}		
		pos2 = (pos-1)&63;

	}
	// animation 4 ends
	// animation 5
	else if(num == 5){
		int loc;
		loc = pos;
			for(int i = 0; i <= loc; ++i){
				for(int j = 0; j <= loc; ++j)
					for(int k = 0; k <= loc; ++k)
						SetPixelCube(i,j,k,rand(),rand(),rand(),image);
			}
			pos++;	
	}

	//animation 5 ends
	//animation 6
	else if(num == 6){
		if(R == 255){
			B = 0;
			R = 0;
			G = 255;
		}
		else if(G == 255){
			R = 0;
			G = 0;
			B = 255;
		}else{
			B = 0;
			R = 255;
			G = 0;
		}

		if((count%2)==0){
			for(int i = 0; i < 4; ++i)
				for(int j = 0; j < 4; ++j)
					for(int k = 0; k < 4; ++k)
						SetPixelCube(i,j,k,R,G,B,image);
		}else{
			for(int i = 0; i < 4; ++i)
				for(int j = 0; j < 4; ++j)
					for(int k = 0; k < 4; ++k)
						SetPixelCube(i,j,k,0,0,0,image);
		}
		count++;
	}


	//animation 7
	//searchnav
	else if(num == 7){
		for(int i = 0; i < 4; ++i)
			{
				for (int j =0; j<4; j++)
				{

					for (int k = 0; k<4; k++)
					{
						if(count<4){
							SetPixelCube(i,j-count%4,k,255,100,0,image);
						}
						else if(count<8){
							SetPixelCube(i,j-(4-count%4),k,0,255,100,image);
						}
						else{count=0;}
					}
				}
			}
		count++;
	}	
	//animation 8
	else if(num ==8){
		for(int i = 0; i < 4; ++i)
			{
				for (int j =0; j<4; j++)
				{

					for (int k = 0; k<4; k++)
					{
						if(count<4){
							SetPixelCube(i,j,k-count%4,0,0,255,image);
						}
						else if(count<8){
							SetPixelCube(i,j,k-(4-count%4),0,0,255,image);
						}
						else{count=0;}
					}
				}
			}
		count++;
	}
	//animation 9
	else if(num ==9){
		for(int i = 0; i < 4; ++i)
			{
				for (int j =0; j<4; j++)
				{

					for (int k = 0; k<4; k++)
					{
						if(count<4){
							SetPixelCube(i,j-count%4,k,0,255,0,image);
						}
						else if(count<8){
							SetPixelCube(i,j-(4-count%4),k,0,255,0,image);
						}
						else{count=0;}
					}
				}
			}
		count++;
	}
	//animation 10
	else if(num ==10){
		for(int i = 0; i < 4; ++i)
			{
				for (int j =0; j<4; j++)
				{

					for (int k = 0; k<4; k++)
					{
						if(count<4){
							SetPixelCube(i-count%4,j,k,255,0,0,image);
						}
						else if(count<8){
							SetPixelCube(i-(4-count%4),j,k,255,0,0,image);
						}
						else{count=0;}
					}
				}
			}
		count++;
	}	
	//animation 11
	//searchnav
	else if(num ==11){
		for(int i = 0; i < 4; ++i)
			{
				for (int j =0; j<4; j++)
				{

					for (int k = 0; k<4; k++)
					{
						if(count<4){
							SetPixelCube(i-count%4,j,k,255,0,0,image);
						}
						else if(count<8){
							SetPixelCube(i,j,k-(4-count%4),255,0,0,image);
						}
						else{count=0;}
					}
				}
			}
		count++;
	}	


	// for debugging
	else if(num == 100){
		for(int i = 0; i < 4; ++i)
			{
					SetPixelCube(i,0,0,255,0,0,image);
					SetPixelCube(0,i,0,0,255,0,image);
					SetPixelCube(0,0,i,0,0,255,image);
			}


	}


	// send the image
	gadget.SetFrame(image);

	// show the image
	gadget.FlipFrame();

} // DrawFrame

// DrawFrame (Overload) for mode 4
void DrawFrame(GadgetControl & gadget, const string & device, int num, vector<int>amp, vector<int>beat)
{
	uint8 image[96]; // RGB buffer, 4 bits per color, packed
	static int ampPos = 0;
	// clear screen by setting all values to 0
	memset(image,0,sizeof(image));
	int R,G,B;

	if(beat[ampPos] > 0 && beat[ampPos] < 6){
		R = 0;
		G = beat[ampPos]*51;
		B = 255;
	}
	else if(beat[ampPos]>=6 && beat[ampPos]<10){
		R = 255;
		G = (10-beat[ampPos])*51;
		B = 0;
	}
	else if(beat[ampPos]==0){
		R = 0;
		G = 0;
		B = 255;
	}
	else{
		R = 255;
		G = 0;
		B = 0;
	}
	
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			for(int k = 0; k <= amp[ampPos]; k++)
				if(amp[ampPos]==0)
					SetPixelCube(i,j,k,0,0,0,image);
				else
					SetPixelCube(i,j,k-1,R,G,B,image);

	if(ampPos<amp.size()-1)
		ampPos++;
	else
		ampPos = 0;
	// send the image
	gadget.SetFrame(image);

	// show the image
	gadget.FlipFrame();

}
// end of DrawFrame (overload)


// attempt to login to a HypnoGadget.
// Return true on success, else return false
bool Login(GadgetControl & gadget)
	{
	// todo - try logging out and back in until works?
	gadget.Login();
	for (int pos = 0; pos < 100; ++pos)
		{
		gadget.Update(); // we need to do this ourselves since we are not multithreaded
		if (GadgetControl::LoggedIn == gadget.GetState())
			return true;
		Sleep(5);
		}

	return false;
	} // Login

// Run the gadget demo
// Assumes port is a string like COMx where x is a value
void RunDemo(const string & port, const string & device, int mode)
	{
	// Two classes we need to feed to the gadget control
	DemoGadgetIO     ioObj;	  // handles COM bytes
	DemoGadgetLock   lockObj; // handles threads

	// 1. Create a gadget
	GadgetControl gadget(ioObj,lockObj);

	// 2. Open the connection
	if (false == ioObj.Open(port))
		{
		cerr << "Error opening port " << port << endl;
		return;
		}

	// 3. Login to the gadget
	if (false == Login(gadget))
		{
		cerr << "Error: could not login to the gadget. Make sure you have the correct COM port.";
		ioObj.Close();
		return;
		}

	// 4. While no keys pressed, draw images


//read in the array
	
		
	vector<string> text_file;
	vector<int> beatData;
	ifstream infile1( "output.txt" );
	string temp1;

	if(infile1.is_open()){
		while( getline( infile1, temp1 ) )
			text_file.push_back( temp1 );
		infile1.close();

		beatData = vecstr_to_vecint(text_file);
	}
	else{
		perror("Read File");
	}
	
	

	vector<string> text_file2;
	vector<int> ampData;
	ifstream infile2( "amplitudes.txt" );
	string temp2;

	if(infile2.is_open()){
		while( getline( infile2, temp2 ) )
			text_file2.push_back( temp2);
		infile2.close();

		ampData = vecstr_to_vecint(text_file2);
	}
	else{
		perror("Read File");
	}
	


// searchnav
	// do not go faster than about 30 frames per second, 
	// or the device may lock up, so....
	int num;
	num = 0;

	//cout << "Running...\n";
	
	ifstream infile3( "songname.txt" );
	string songname;
	if(infile3.is_open()){
		getline(infile3,songname);
		infile3.close();
	}
	else{
		perror("Read File");
	}



	PlaySoundA(songname.c_str(), NULL, SND_FILENAME|SND_ASYNC);
		for (int j = 0; j < beatData.size(); j++){
			//cout<<"start"<<endl;

			switch(mode){
				case 1: num = rand()%12; break;
				case 2: num = beatData[j]; break;
				case 3: num = 0; break;
				default: num = 1; break;
			}

			for (int i = 0; i < beatData[j]; i++)
			{
			// be sure to call this often to process serial bytes
				if(num==0){
					DrawFrame(gadget,device,num,ampData,beatData);
				}
				else
				DrawFrame(gadget,device, num);
				gadget.Update(); 
				Sleep(1000/beatData[j]/2);

			}

			//cout<<"end"<<endl;
		}
	PlaySound(NULL, 0, 0); //stop sound


	//while (_kbhit()) {
	//	_getch(); // eat any keypresses
	//	cout << "Ending...\n";
	//}
	// 5. Logout
	gadget.Logout();  // we assume it logs out
	for (int pos = 0; pos < 10; ++pos)
		{
		gadget.Update();  // we must call this to process bytes
		Sleep(100);        // slight delay for 
		}

	// 6. Close the connection

	ioObj.Close();
	Sleep(100);       // slight delay
	} // RunDemo

// show the usage for the command line parameters
void ShowUsage(const string & programName)
	{
	cerr << "Usage: " << programName << " COMx [MODE]\n";
	cerr << " Where COMx is the COM port with the gadget attached,\n";
	cerr << " and [MODE] is the mode (1-3) you want to use.\n";
	cerr << "Example: " << programName << " COM4 2\n";
	} // ShowUsage

// The program starts executing here (obviously...)
int main(int argc, char ** argv)
	{
	cout << "CSIT5110 Music-to-3D Lights\n";
	cout << "Using Hypnocube SDK\n";
	
	if (3 != argc)
		{ // not enough command line parameters
		ShowUsage(argv[0]);
		exit(-1);
		}
	
	// get parameters
	string port(argv[1]);
	string device("cube");

	stringstream str;    
    str << argv[2];
    int mode;
    str >> mode;


	// clean them
	transform(port.begin(), port.end(), port.begin(), toupper);       // uppercase
	transform(device.begin(), device.end(), device.begin(), toupper); // uppercase

	// sanity check the arguments
	bool validDevice = ("CUBE" == device); // test accepted devices
	bool validMode = (mode > 0 && mode < 4);
	if ((4 != port.length()) || ("COM" != port.substr(0,3)) || 
		(false == isdigit(port[3])) || (false == validDevice) ||(false == validMode))
		{ // wrong command line parameters
		ShowUsage(argv[0]);
		exit(-2);
		}

	
	// finally - run the demo!
	RunDemo(port,device, mode);


	return 0;
	} // main

// end - HypnoDemo.cpp