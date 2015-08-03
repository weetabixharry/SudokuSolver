#include "sudoku_grid.h"

#include <iostream>
#include <bitset>

using namespace std;

sudoku::grid::grid(vector<string> grid) : vector<string>(grid)
{
    init_ptrs();
}

sudoku::grid::grid(const grid& grid) : vector<string>(grid)
{
    init_ptrs();
}  

void sudoku::grid::init_ptrs()
{
    // Initialise column pointers
    columns = vector< vector<char*> >(9, vector<char*>(9));
    for (size_t i=0; i<this->size(); i++)
        for (size_t c=0; c<(*this)[i].size(); c++)
            columns[c][i] = &((*this)[i][c]);

    // Initialise block pointers
    blocks = vector< vector<char*> >(9, vector<char*>(9));
    for (size_t i=0; i<this->size(); i++)
        for (size_t j=0; j<(*this)[i].size(); j++)
        {
            int idx = (j%3) + 3*(i%3);
            int blk = (j/3) + 3*(i/3);
            blocks[blk][idx] = &((*this)[i][j]);
        }
}

vector<pair<int, int> > sudoku::grid::get_block_coords(int blk) const
{
    int mod = blk % 3;
    int div = blk / 3;
    
    // Construct unique set of coords
    vector<int> ii(3), jj(3);
    for (int k=0; k<3; k++)
    {
        ii[k] = k + 3*mod;
        jj[k] = k + 3*div;
    }
    
    // Take all possible combinations of unique coords
    vector<pair<int, int> > coords(9);
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++)
            coords[i+3*j] = pair<int,int>(ii[i],jj[j]);

    return coords;
}

sudoku::grid::state sudoku::grid::calc_grid_state() const
{
    bool solved = true;

    // Check across all rows
    for (int i=0; i<9; i++)
    {
        bitset<10> row_mask;
        for (int q=0; q<9; q++)
        {
            int val = (*this)[i][q] - '0';
            if (val == 0)
                solved = false;            
            else if (row_mask[val])
            {
//                cout << "Error in row: " << i << " at position " << q << endl;
                return sudoku::grid::invalid_state;
            }
            else
                row_mask.set(val, true);
        }
    }  
    
    // Check down all columns
    for (int i=0; i<9; i++)
    {        
        bitset<10> col_mask;
        for (int q=0; q<9; q++)
        {
            int val = (*this)[q][i] - '0';
            if (val == 0)
                solved = false;            
            else if (col_mask[val])
            {
//                cout << "Error in col: " << i << " at position " << q << endl;
                return sudoku::grid::invalid_state;
            }
            else
                col_mask.set(val, true);
        }
    }     
    
    // Check all blocks
    for (int i=0; i<9; i++)
    {        
        bitset<10> blk_mask;
        vector< pair<int, int> > coords = get_block_coords(i);
        for (int q=0; q<9; q++)
        {
            int val = (*this)[coords[q].first][coords[q].second] - '0';
            if (val == 0)
                solved = false;            
            else if (blk_mask[val])
            {
//                cout << "Error in block: " << i << " at position " << q << endl;
                return sudoku::grid::invalid_state;
            }
            else
                blk_mask.set(val, true);
        }
    }     
    
    return solved ? sudoku::grid::solved_state : sudoku::grid::valid_state;
}

void sudoku::grid::print() const
{
    for (size_t i=0; i<this->size(); i++)
        cout << (*this)[i] << endl;
}

void sudoku::grid::print_ptrs(const vector< vector<char*> > grid) const
{
    for (size_t i=0; i<grid.size(); i++)
    {
        for (size_t j=0; j<grid[i].size(); j++)
            cout << *grid[i][j];
        cout << endl;
    }
}
