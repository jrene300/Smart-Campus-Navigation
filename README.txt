Project 2: Smart Campus Navigation and Facility Analytics System
---------------------------------------------------------------------------------
Course: CSCE2110 Foundation of Data Structures		Milestone 1 submission

Team members: John Kwitonda (Team member 1), Soyog Karki (Team member 2), Ronnie Tienda (Team member 3)
---------------------------------------------------------------------------------
Purpose:
A C++ program that loads campus data from four text files and lets a user locate facilities by ID, explore the campus
graph and run BFS reachability, and browse/modify a department hierarchy.
M1 provides menu options 1-9 and 10 (Exit).

Build:
Project2/ root directory:

    g++ -std=c++11 -Wall -Iinclude -DMILESTONE1 src/*.cpp -o campus

The -DMILESTONE1 flag selects the 10-item Milestone 1 menu. Building
without it produces the Final 18-item menu.

Run commands:
Default data directory:

    ./campus

-Optional data-directory argument (NFR-10), for example our test sets:

    ./campus testdata/collision
    ./campus testdata/messy
    ./campus testdata/empty
    ./campus testdata/missing
    ./campus testdata/tie


Layout:
Files loaded in order: buildings.txt, connections.txt, facilities.txt, departments.txt. 
Enter 0 or 10 to exit.

M1 MENU

 1. View all facilities
 2. Search facility by ID
 3. View campus graph
 4. Display graph counts and degrees
 5. Find reachable buildings (BFS)
 6. Add department
 7. Search department
 8. Display department hierarchy
 9. Run preorder traversal
10. Exit

Data-directory:
Project2/
  include/            //Header files
    Utilities.h         //trim, case, split, prompt helpers
    DataLoader.h        //loads and validates the four files
    Facility.h         // Facility record and ID-format check
    HashTable.h         //custom hash table (separate chaining)
    FacilityStore.h     //facility list (file order) + hash table
    CampusGraph.h       //undirected graph (adjacency list) and BFS
    DepartmentTree.h    //general tree of departments
  src/                //Implementation files (main.cpp = init, load, menu)
  data/               //The four instructor starter files (unchanged)
  testdata/           //Our own test sets (collision, messy, empty, missing, tie), loaded via the data-dir argument
  README.txt
  CONTRIBUTIONS.txt
  design_document.txt
  test_results.txt
  .gitignore
