#include "sudoku_front_end.h"
#include "sudoku_guesser.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>

using namespace std;

sudoku::front_end::front_end(std::string filename)
    : _grid_strings(this->load(filename))
{
}

void sudoku::front_end::solve() const
{
    for (int i=0; i<_grid_strings.size(); i++)
    {
        cout << endl;
        cout << "#########" << endl;
        cout << "SUDOKU " << i << endl;
        cout << "#########" << endl;
        sudoku::guesser sdku(_grid_strings[i]);
        sdku.solve();
    }    
}

vector<vector<string> > sudoku::front_end::load(string filename) const
{
    // Read text file into string
    string sudoku_string = file2string(filename); 

    // Vector of all grids
    vector< vector<string> > grids;
    
    // Temporary grid storage
    vector<string> tmp_grid(9);
    
    // Extract grids from text file
    size_t start_idx = 0, stop_idx = 0;
    int row_idx = 8;
    while(stop_idx != std::string::npos)
    {       
        stop_idx = sudoku_string.find("\n", start_idx);      
        std::string row = sudoku_string.substr(start_idx, stop_idx-start_idx);    
        start_idx = stop_idx+1;
        
        // Handle index wrapping (and title line skipping)
        row_idx++;
        if (row_idx == 9)
        {
            row_idx = -1;
            continue;
        }

        // Safety checks
        assert(row.size() == 9);        
        assert(row_idx >= 0 && row_idx < 9);
        
        // Write this row into temporary grid
        tmp_grid[row_idx] = row;
        
        // Store grid when last row is written
        if (row_idx == 8)
            grids.push_back(tmp_grid);
    }    
    
    return grids;
}

string sudoku::front_end::file2string(string filename) const
{
    ifstream textfile(filename.c_str());
    if (textfile.is_open())
    {
        stringstream buffer;
        buffer << textfile.rdbuf();
        return buffer.str(); 
    }
    else
    {
        cout << "Failed to open file: " << filename << endl;
        return("");
    } 
}
