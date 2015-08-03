#ifndef SUDOKU_FRONT_END_H
#define	SUDOKU_FRONT_END_H

#include <string>
#include <vector>

namespace sudoku
{
    
    class front_end
    {
    public:
        
        front_end(std::string filename);
        
        void solve() const;
        
    private:
        
        std::string file2string(std::string filename) const;
        std::vector<std::vector<std::string> > load(std::string filename) const;
        
        const std::vector<std::vector<std::string> > _grid_strings;
    };
    
}

#endif	/* SUDOKU_FRONT_END_H */
