#include "TFile.h"
#include "TKey.h"
#include "TClass.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TObject.h"
#include "TROOT.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

std::vector<TH1F*> SortByName( std::vector<TH1F*> HistosToSort );
std::vector<TH2F*> SortByName( std::vector<TH2F*> HistosToSort );
std::vector<TH3F*> SortByName( std::vector<TH3F*> HistosToSort );
int compare( std::string a, std::string b, int iterator);

int main( int argc, char* argv[] )
{
	if( argc != 3 )
	{
		std::cout << "Provide only name of the file with files and output file" << std::endl;
		return 0;
	}
	std::string FileWithNames = argv[1];
	std::string OutputFile = argv[2];
	std::vector<std::string> FileNames;
	std::ifstream data;
	std::string count = "";
	data.open( ( FileWithNames ).c_str() );
	while( data >> count )
	{
		FileNames.push_back( count );
	}
	data.close();
	
	if( FileNames.size() == 0 )
	{
		std::cout << "No FileNames readed in given file" << std::endl;
		return 0;
	}
	
	std::vector<TH1F*> HistogramsToAdd1D;
	std::vector<TH2F*> HistogramsToAdd2D;
	std::vector<TH3F*> HistogramsToAdd3D;
	
	TH1F* temp1D;
	TH2F* temp2D;
	TH3F* temp3D;
	
	TFile* file1 = new TFile( FileNames[0].c_str(), "READ" );
	TDirectory *dir;
	file1->GetObject("EventCategorizer subtask 0 stats",dir);	//Change the name of directory to that with histograms
	
	TIter next( dir->GetListOfKeys() );
	TKey *key;

	while( ( key = (TKey*)next() ) ) 
	{
		TClass *cl = gROOT->GetClass(key->GetClassName());
		if( cl->InheritsFrom("TH1F") )		
		{
			temp1D = (TH1F*) key->ReadObj()->Clone();
			temp1D -> SetDirectory(0);
			HistogramsToAdd1D.push_back( temp1D );
		}
		else if( cl->InheritsFrom("TH2F") )		
		{
			temp2D = (TH2F*) key->ReadObj()->Clone();
			temp2D -> SetDirectory(0);
			HistogramsToAdd2D.push_back( temp2D );
		}
		else if( cl->InheritsFrom("TH3F") )		
		{
			temp3D = (TH3F*) key->ReadObj()->Clone();
			temp3D -> SetDirectory(0);
			HistogramsToAdd3D.push_back( temp3D );
		}
	}
	HistogramsToAdd1D = SortByName( HistogramsToAdd1D );
	HistogramsToAdd2D = SortByName( HistogramsToAdd2D );
	HistogramsToAdd3D = SortByName( HistogramsToAdd3D );	
	file1->Close();
	delete file1;
	
	for( unsigned i=1; i<FileNames.size(); i++ )
	{
		if( i%100 == 0 )
			std::cout << "File Number " << i << " readed" << std::endl;
		TFile* file2 = new TFile( FileNames[i].c_str(), "READ" );
		TDirectory *dir2;
		file2->GetObject("EventCategorizer subtask 0 stats",dir2);
		
		for( unsigned i=0; i<HistogramsToAdd1D.size(); i++ )
		{
			temp1D = (TH1F*) dir2->Get( HistogramsToAdd1D[i]->GetName() );
			if( temp1D )
				HistogramsToAdd1D[i] -> Add( temp1D );
		}
		for( unsigned i=0; i<HistogramsToAdd2D.size(); i++ )
		{
			temp2D = (TH2F*) dir2->Get( HistogramsToAdd2D[i]->GetName() );
			if( temp2D )
				HistogramsToAdd2D[i] -> Add( temp2D );
		}
		for( unsigned i=0; i<HistogramsToAdd3D.size(); i++ )
		{
			temp3D = (TH3F*) dir2->Get( HistogramsToAdd3D[i]->GetName() );
			if( temp3D )
				HistogramsToAdd3D[i] -> Add( temp3D );
		}
		
		file2->Close();
		delete file2;
	}
	
	TFile* file0 = new TFile( OutputFile.c_str(), "RECREATE" );
	for( unsigned i=0; i<HistogramsToAdd1D.size(); i++ )
	{
		HistogramsToAdd1D[i] -> Write();
	}
	for( unsigned i=0; i<HistogramsToAdd2D.size(); i++ )
	{
		HistogramsToAdd2D[i] -> Write();
	}
	for( unsigned i=0; i<HistogramsToAdd3D.size(); i++ )
	{
		HistogramsToAdd3D[i] -> Write();
	}
	file0 ->Close();
	
	delete file0;
	
	return 0;
}

std::vector<TH1F*> SortByName( std::vector<TH1F*> HistosToSort ) //Bubble sort
{
	std::vector<TH1F*> SortedHistos = HistosToSort;
	TH1F* tempHisto;
	//std::string tempName;
	double NumberOfSorts = 0;
	int test = 1;
	if( HistosToSort.size() == 0 )
	{
		return SortedHistos;
	}
	while( test )
	{
		for( unsigned j=0; j<SortedHistos.size()-1; j++ )
		{
			if( compare( SortedHistos[j]->GetName(), SortedHistos[j+1]->GetName(), 0 ) < 0 )
			{
				tempHisto = SortedHistos[j];
				SortedHistos[j] = SortedHistos[j+1];
				SortedHistos[j+1] = tempHisto;
				
				NumberOfSorts++;
			}
		}
		if( NumberOfSorts == 0 )
		{
			test = 0;
		}
		NumberOfSorts = 0;
	}
	return SortedHistos;
}

std::vector<TH2F*> SortByName( std::vector<TH2F*> HistosToSort ) //Bubble sort
{
	std::vector<TH2F*> SortedHistos = HistosToSort;
	TH2F* tempHisto;
	//std::string tempName;
	double NumberOfSorts = 0;
	int test = 1;
	if( HistosToSort.size() == 0 )
	{
		return SortedHistos;
	}
	while( test )
	{
		for( unsigned j=0; j<SortedHistos.size()-1; j++ )
		{
			if( compare( SortedHistos[j]->GetName(), SortedHistos[j+1]->GetName(), 0 ) < 0 )
			{
				tempHisto = SortedHistos[j];
				SortedHistos[j] = SortedHistos[j+1];
				SortedHistos[j+1] = tempHisto;
				
				NumberOfSorts++;
			}
		}
		if( NumberOfSorts == 0 )
		{
			test = 0;
		}
		NumberOfSorts = 0;
	}
	return SortedHistos;
}

std::vector<TH3F*> SortByName( std::vector<TH3F*> HistosToSort ) //Bubble sort
{
	std::vector<TH3F*> SortedHistos = HistosToSort;
	TH3F* tempHisto;
	//std::string tempName;
	double NumberOfSorts = 0;
	int test = 1;
	if( HistosToSort.size() == 0 )
	{
		return SortedHistos;
	}
	while( test )
	{
		for( unsigned j=0; j<SortedHistos.size()-1; j++ )
		{
			if( compare( SortedHistos[j]->GetName(), SortedHistos[j+1]->GetName(), 0 ) < 0 )
			{  
				tempHisto = SortedHistos[j];
				SortedHistos[j] = SortedHistos[j+1];
				SortedHistos[j+1] = tempHisto;
				
				NumberOfSorts++;
			}
		}
		if( NumberOfSorts == 0 )
		{
			test = 0;
		}
		NumberOfSorts = 0;
	}
	return SortedHistos;
}

int compare( std::string a, std::string b, int iterator)
{
	if( int( a[iterator] ) < int( b[iterator] ) )
	{
		return 1;
	}
	else if( int( a[iterator] ) > int( b[iterator] ) )
	{
		return -1;
	}
	else
		return compare( a, b, iterator+1 );
}