# Max Connected Colors Analyzer (MCCA) v1.1.0

Max Connected Colors Analyzer (MCCA) is a C++ console application designed to solve and analyze connected regions of color codes within a grid or matrix.  
It offers flexibility in choosing different algorithms (DFS, Union-Find) and provides visualizations and random matrix file generation options for further analysis.  

This README provides instructions on how to use MCCA and its available features.  

## Features

- Analyze max connected color regions in a grid (matrix).
- Support for three graph algorithms: DFS, Union-Find, or both.
- Visualization of Union-Find Root using Graphviz.
- Option to paint regions or save results as a matrix file.
- Matrix file generator for creating random or customized matrices.


### Main Application

Usage: 

       MCCA [--root_dir <rootDir>] [--paint] [--no_color] [--algo <algo>]
            [--crop][--visualizer][--matrix <matString>] [--cond] [--help] [/?].

Options:  
**--algo [algo]**: Select the graph algorithm to use:

      UF: Union-Find algorithm. This option is set by default.
      DFS: Depth-First Search algorithm.
      BOTH: Use both DFS and Union-Find algorithms.

**--paint**: Paint the max connected color regions.

**--no_color**: Paint in black and white using a unique symbol-map.  
If neither **--paint** nor **--no_color** is specified, results will be saved under  root_dir directory.  

**--root_dir [rootDir]**: Specify the root directory for processing matrix files.

**--matrix [matString]**: Specify matrices (C/C++ format) as a string.  
Values allowed: integers within [1, 9]. 
        
        Example: **--matrix "{{1, 2, 1}, {2, 3, 2}, {1, 2, 1}}"**  
         
**--visualizer**: Enable the Union-Find Root visualizer.  
This feature is limited by design to small matrices (maximum dimensions of VIS_MAT_THR).  
[Graphviz](https://graphviz.org/download/) must be installed and added to the system environment path for this feature to work.

**--crop**: Only save the max connected color regions into files.

**--help** or __/?__: Display the help menu.

**--cond**: Display conditions.

**--ver**: Display title & version. 

Usage Example:  

        MCCA --root_dir C:/MCCA/Data/matrix_shapes --paint --algo dfs

### Matrix Files Generator:  
Usage:   
    
        MCCA filegen [--sqmat] [--root_dir <rootDir>] [--confirm] [--ext <ext>]
                     [--ovr] [--minrows <min_rows>] [--maxrows <max_rows>]
                     [--row_inc <row_inc>] [--mincols <min_cols>] [--maxcols <max_cols>]
                     [--col_inc <col_inc>] [--minv <min_v>] [--maxv <max_v>]


Options:   
**--sqmat**: Generate square matrix files, skipping column-related parameters.  
**--confirm**: Automatically confirm saving files to disk.  
**--ext <ext>**: Specify the file extension for saved matrices (e.g., csv).  
**--minrows <min_rows>**: min_rows <= 10000. 
**--maxrows <max_rows>**: max_rows <= 10000.  
**--row_inc <row_inc>**: row_inc <= 9999.  
**--mincols <min_cols>**: min_cols <= 1. 
**--maxcols <max_cols>**: max_cols <= 10000. 
**--col_inc <col_inc>**: col_inc <= 9999.  
**--minv <min_v>**: min_v within [1, 9]. 
**--maxv <max_v>**: max_v within [1, 9]. 

Notes:  
**--sqmat** cannot be used with column parameters.

Usage Examples:   

        MCCA filegen --maxrows 750 --maxcols 400 --inc_col 20  
    
        MCCA filegen --sqmat --minrows 4000 --maxrows 5000 --row_inc 500 --ext "csv"


Requirements:  
Compiler: A C++20-compatible compiler (e.g., GCC, Clang, MSVC).  
Graphviz: For visualizing Union-Find roots (optional).  
Operating System: Windows (cross-platform support might be considered in the future).  

## Contributing: 
- Fork the repository.
- Create a new branch for your feature or fix.
- Make your changes.
- Commit and push your changes.
- Create a pull request with a description of the changes.

## License and Attribution

This project is licensed under the GNU General Public License v3.0 (GPLv3).

**Attribution Requirement**: 
* The original author, **Yaron Dayan**, must be credited in all derivative works, modifications, or redistributions of this software.

For more details, refer to the full text of the GPLv3 [here](https://www.gnu.org/licenses/gpl-3.0.html).

## Contributing

By contributing to this project, you agree to the terms of the **GNU General Public License v3.0**.

**Attribution**: 
* When contributing code or making modifications, ensure that the original author, **Yaron Dayan**, is clearly credited in the appropriate sections of the codebase.
